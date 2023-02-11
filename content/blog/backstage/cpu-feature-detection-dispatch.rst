Convenient CPU feature detection and dispatch
#############################################

:date: 2022-08-02
:category: Backstage
:tags: Android, ARM, C++, Eigen, Emscripten, Linux, SIMD, singles, WebAssembly,
    x86
:cover: {static}/img/blog/backstage/cpu-feature-detection-dispatch.jpg
:summary: A new Magnum feature provides efficient compile-time and runtime CPU
    detection and dispatch on x86, ARM and WebAssembly. The core idea behind
    allows adding new variants without having to write *any* dispatching code.

.. role:: cpp(code)
    :language: c++
.. role:: doc-link(link)
    :class: m-flat
.. role:: text-dim
    :class: m-text m-dim
.. role:: s
    :class: m-text m-s

.. |o| replace:: ·

Among key aspects differentiating between "performance is a *joke* for this
project!" and "wow they really mean it" --- besides favoring data-oriented
design over abstract factory proxy manager delegates --- is use of SIMD
instructions such as AVX or NEON, and ultimately detecting and picking the best
optimized implementation for given hardware at runtime.

For over a decade, Magnum could afford to not bother, partly because the actual
heavy lifting such as :dox:`physics <BulletIntegration>` or
:dox:`texture compression <Trade::BasisImageConverter>` was offloaded to 3rd
party libraries, and partly because if you really needed to perform things like
fast multiplication of large matrices, you could always
:dox:`seamlessly delegate to Eigen <EigenIntegration>`. But as all new Magnum
APIs are data-oriented, designed with batch operations in mind, I just couldn't
ignore the extra performance that SIMD instructions could bring.

`Core idea --- inheritance-based overload resolution`_
======================================================

What triggered this whole process was that I realized I could make use of C++
inheritance to pick among candidate overloads. Loosely citing the
`C++ reference on Overload Resolution <https://en.cppreference.com/w/cpp/language/overload_resolution>`_:

    F1 is determined to be a better function than F2 if :text-dim:`[...]` there
    is at least one argument of F1 whose implicit conversion is *better* than
    the corresponding implicit conversion for that argument of F2.

    If two conversion sequences are indistinguishable because they have the
    same rank, the following additional rules apply:

    -   If ``Mid`` is derived (directly or indirectly) from ``Base``, and
        ``Derived`` is derived (directly or indirectly) from ``Mid``, then
        ``Derived`` to ``Mid`` is better than ``Derived`` to ``Base``.

For a practical example, let's assume we're about to implement a
:doc-link:`memrchr() <https://man.archlinux.org/man/memrchr.3>` function,
because, compared to :dox:`std::memchr()`, it's for some reason not a widely
available API and we want to have a fast implementation everywhere. Taking just
x86 into account for simplicity, we'll have an AVX2 variant, a slightly slower
SSE2 variant and a scalar fallback, distinguished from each other not by a name
but by a *tag*, similarly to what tags like :dox:`NoInit` or :dox:`ValueInit`
do in :dox:`Containers::Array` constructors:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [declare]
    :end-before: // [/declare]

Then, an appropriate tag would be :cpp:`typedef`\ 'd depending on what
particular :dox:`CORRADE_TARGET_* <CORRADE_TARGET_SSE2>` preprocessor variables
are defined, with an instance of that tag exposed through a :cpp:`constexpr`
variable for easy use:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [typedef]
    :end-before: // [/typedef]

Ultimately, to pick the right overload at compile time, the ``DefaultCpu`` tag
gets passed alongside other parameters. On an AVX-512 machine the AVX2
implementation gets chosen, as ``Avx2T`` is the closest available base of
``Avx512T``. On an AVX machine, the SSE2 implementation gets chosen instead ---
``Avx2T`` is a subclass of ``AvxT``, so it's ruled out, and the closest base
is ``Sse2T``. In the rare scenario where not even SSE2 is present, it falls
back to the ``ScalarT`` variant.

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [use]
    :end-before: // [/use]

Pretty neat, huh? This makes compile-time dispatch extremely easy to perform,
and what I especially like about it is that adding a new variant is literally
just one more overload. No redundant boilerplate, no manually managed dispatch
tables, no surface for bugs to creep in.

Now, what's left is "just" the runtime dispatch.

`A year passed by ...`_
=======================

... during which I basically abandoned the whole idea.\ [1]_ Reason is that the
very feature that makes it work at compile time --- different function
signatures --- is what makes runtime dispatch *really painful*. For every such
function, one would have to manually write a snippet like this, adding
significant maintenance and runtime overhead. Didn't I want to avoid
*exactly this* in the first place?

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [runtime-wrong]
    :end-before: // [/runtime-wrong]

Also, originally I planned to use CPU feature dispatch for *significant* chunks
of code like calculating mesh normals or resizing images, to minimize the
impact of such dispatch overhead. But ended up here, wanting to use it for a
:cpp:`memrchr()` implementation! Which means that *any* sort of overhead that's
bigger than a regular function call is not going to cut it. Especially not a
giant :cpp:`if` cascade with a potentially expensive argument passthrough.

`How the grownups do it`_
=========================

Fortunately, during recent perf investigations and code profiling sessions I
discovered the `GNU IFUNC attribute <https://sourceware.org/glibc/wiki/GNU_IFUNC>`_,
and found out that it's even *The Solution*
:gh:`used by glibc itself <bminor/glibc/blob/78fb88827362fbd2cc8aa32892ae5b015106e25c/sysdeps/x86_64/multiarch/ifunc-memcmp.h>`
to dispatch to architecture-specific variants of :dox:`std::memchr()` or
:dox:`std::memcmp()`. Can't really do much better than that, right?

Which led me to a conclusion that :abbr:`the ideal way <not slower than glibc>`
to perform runtime CPU feature dispatch would be to:

1.  Declare a function (pointer) in the header, with the actual complexity and
    architecture-specific code hidden away into a source file. To minimize function call overhead, avoid passing heavy classes with redundant state
    --- ideally just builtin types. In case of our example, it would be
    :cpp:`extern const char*(*memrchr)(const char*, std::size_t, char)`.
2.  If the function is meant to be called from within a higher-level API and
    not directly (such as in this case, where it'd be exposed through
    :dox:`Containers::StringView::findLast(char) <Containers::BasicStringView::findLast()>`),
    make that API the smallest possible wrapper that's inlined in the header.
    That way the compiler can inline the wrapper on the call site, turning it
    into just a single call to the function (pointer), instead of two nested
    calls.

Meaning, if I want to have a fast dispatch, I have to find a solution that
doesn't involve expensive argument passthrough. Out of desperation I even
thought of Embracing the Darkness and :cpp:`reinterpret_cast<>`\ 'ing the
variants to a common function pointer type, but discarded that idea upon
discovering that `WebAssembly checks that a function is only called with a matching type <https://emscripten.org/docs/porting/guidelines/function_pointer_issues.html>`_,
preventing such hack from working there. Not to mention the public humiliation,
address sanitizer and static analysis complaints.

.. transition:: # # #

`Function currying to the rescue`_
==================================

The *Heureka Moment* came to me when I was checking
`if C++ could do function currying <https://vittorioromeo.info/index/blog/cpp17_curry.html>`_,
and the solution isn't that much more complex than the original idea. First let
me show how the :cpp:`memrchr()` example from the top would be rewritten in a
way that *actually works* with both a compile-time and a runtime dispatch, and
uses an *actual* :dox:`Corrade::Cpu` library:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [final-declare]
    :end-before: // [/final-declare]

The only difference compared to the previous attempt is that the
architecture-specific variants are now returning a lambda\ [2]_ that contains
the actual code instead ... and then there's an opaque macro. Since
non-capturing lambdas are just like regular functions, there isn't any extra
overhead from putting the code there instead. The wrapper function, taking the
tag, is however marked as :dox:`CORRADE_ALWAYS_INLINE`, meaning it optimizes
down to accessing a function pointer directly. Thus performing

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [final-use-implementation]
    :end-before: // [/final-use-implementation]

--- where :dox:`Cpu::DefaultBase` is an alias to the highest base instruction
set enabled at compile time --- is equivalent to the previous attempt, but with
the important difference that it's possible to separate the compile-time
dispatch from the actual function call.

Which is what makes possible to implement a :abbr:`zero-overhead <again, not slower than glibc>`
*runtime* dispatch as well. And that's what the
:dox:`CORRADE_CPU_DISPATCHER_BASE()` macro is for --- here is the x86 variant
of it. It uses :dox:`Cpu::Features`, which I didn't talk about yet, but suffice
to say it's similar to a :dox:`Containers::EnumSet`, converting the
compile-time tags to a bitfield-like value that can be operated with at
runtime:

.. code:: c++

    #define CORRADE_CPU_DISPATCHER_BASE(function)                               \
        auto function(Cpu::Features features) {                                 \
            if(features & Cpu::Avx512f)                                         \
                return function(Cpu::Avx512f);                                  \
            if(features & Cpu::Avx2)                                            \
                return function(Cpu::Avx2);                                     \
            …                                                                   \
            if(features & Cpu::Sse2)                                            \
                return function(Cpu::Sse2);                                     \
            return function(Cpu::Scalar);                                       \
        }

The macro just stamps out checks for all possible CPU features, from most
advanced to least, and then calls the function with *each* of those tags. Thus
--- like before --- :dox:`Cpu::Avx2` and above branches will resolve to
:cpp:`memrchrImplementation(Cpu::Avx2T)`, all branches below including
:dox:`Cpu::Sse2` will resolve to :cpp:`memrchrImplementation(Cpu::Sse2T)`, and
the :cpp:`memrchrImplementation(Cpu::ScalarT)` fallback gets used if nothing
else matches.

Quite a lot of branching, eh? Not really. Because of the
:dox:`CORRADE_ALWAYS_INLINE`, this compiles down to returning a set of function
pointers, and with even the lowest optimizations enabled the compiler sees that
several branches return the same pointer and collapses them together. To proof
such a wild claim, here's the assembly of the
:cpp:`memrchrImplementation(Cpu::Features)` function that this macro generated,
with GCC and ``-O1``:

.. code:: c-objdump

    0x00001131:  lea    0x175(%rip),%rax <memrchrImplementation(Corrade::Cpu::Avx2T)…>
    0x00001138:  test   $0xc0,%dil
    0x0000113c:  je     0x113f
    0x0000113e:  ret
    0x0000113f:  test   $0x3f,%dil
    0x00001143:  lea    0x15f(%rip),%rax <memrchrImplementation(Corrade::Cpu::Sse2T)…>
    0x0000114a:  lea    0x154(%rip),%rdx <memrchrImplementation(Corrade::Cpu::ScalarT)…>
    0x00001151:  cmove  %rdx,%rax
    0x00001155:  jmp    0x113e

Clang does the same, MSVC is different in that the :dox:`CORRADE_ALWAYS_INLINE`
won't work when optimizations are disabled, leading to the :cpp:`if` cascade
indeed being a bunch of sad function calls. Yet *even that* isn't a problem, as
the dispatcher function is meant to only be called once in a program lifetime.
But I'm skipping ahead.

`The CPU usually doesn't change under our hands`_
=================================================

Now comes the actual runtime CPU feature detection. Which, for x86, is done via
the `CPUID <https://en.wikipedia.org/wiki/CPUID>`_ instruction, and exposed
through :dox:`Cpu::runtimeFeatures()`. There's not much else to say, except
that *a lot of cursing* went into making that code portable. An important
assumption is that the set of CPU features doesn't change during program
lifetime,\ [3]_ and so we can query them and dispatch just once, caching the
result. Which is what :dox:`CORRADE_CPU_DISPATCHED_POINTER()` is for:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [final-dispatched-pointer]
    :end-before: // [/final-dispatched-pointer]

Internally, it simply assigns the result of a call to
:cpp:`memrchrImplementation(Cpu::Features)`, defined with the
:dox:`CORRADE_CPU_DISPATCHER_BASE()` macro above, to a ``memrchr`` function
pointer:

.. code:: c++

    #define CORRADE_CPU_DISPATCHED_POINTER(dispatcher, ...)                     \
        __VA_ARGS__ = dispatcher(Corrade::Cpu::runtimeFeatures());

Since this all happens in a global constructor, the :cpp:`memrchr` function
pointer is ready for use without any explicit setup call. All that's needed is
exposing the pointer in some public header as an :cpp:`extern`:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [final-use]
    :end-before: // [/final-use]

`The magic of IFUNC`_
---------------------

This sounds pretty much ideal already, so what does
`IFUNC <https://sourceware.org/glibc/wiki/GNU_IFUNC>`_ actually bring to the
table? In short, it turns the pointer into a regular function. Which means,
instead of first having to load the value of the function pointer from
*somewhere* and only then calling it, it's like any other dynamic library
function call.\ [4]_

Usage-wise it's not much different from the function pointer approach. A
dispatcher function is associated with a function prototype, which the dynamic
loader then uses to assign the prototype a concrete function pointer. This all
happens during early startup, so the dispatcher code can't really do much ---
especially not calling into external libraries, which may not even be there yet
at that point. Practically it means :dox:`Cpu::runtimeFeatures()` has to be
fully inlined in order to be usable in this context.

Here's how the above would look with :dox:`CORRADE_CPU_DISPATCHED_IFUNC()`
instead:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [final-dispatched-ifunc]
    :end-before: // [/final-dispatched-ifunc]

In the macro implementation, a function is annotated with
:cpp:`__attribute__((ifunc))` carrying a name of the dispatcher function. The
dispatcher function gets called with no arguments, so the macro creates a
:cpp:`memrchrImplementation()` wrapper that delegates to
:cpp:`memrchrImplementation(Cpu::Features)`. What the compiler manual doesn't
say is that the dispatcher function has to have C linkage in order to be found.

.. code:: c++

    #define CORRADE_CPU_DISPATCHED_IFUNC(dispatcher, ...)                       \
        extern "C" { static auto dispatcher() {                                 \
            return dispatcher(Corrade::Cpu::runtimeFeatures());                 \
        }}                                                                      \
        __VA_ARGS__ __attribute__((ifunc(#dispatcher)));

The only downside of this approach is that it's a glibc-specific feature, thus
mainly just Linux (and Android, as I'll detail later). Apart from low-level
glibc code using it, this is also the backbone of GCC's and Clang's
`function multi-versioning <https://lwn.net/Articles/691932/>`_. So far I'm not
aware of anything similarly convenient on macOS or Windows, except maybe for
the ``cpu_dispatch`` attribute in `Clang <https://clang.llvm.org/docs/AttributeReference.html#cpu-dispatch>`_
and `ICC <https://www.intel.com/content/www/us/en/develop/documentation/cpp-compiler-developer-guide-and-reference/top/compiler-reference/attributes/cpu-dispatch-cpu-specific.html>`_.
But that one, as far as I can tell, dispatches on every call, and at least in
case of ICC is `limited only to Intel processors <https://www.agner.org/optimize/blog/read.php?i=49>`_.
No ARM but no AMD either.

`Even less overhead, please?`_
------------------------------

In certain cases it may be desirable to not go through a dynamically dispatched
function at all in order to get benefits from interprocedural optimizations and
LTO. In that case, the dispatcher can select an overload at compile time using
:dox:`Cpu::DefaultBase`, similarly to what the very first example was showing:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [final-dispatched-compile-time]
    :end-before: // [/final-dispatched-compile-time]

In my experiments at least, with compiler optimizations enabled, the whole
returned lambda gets inlined here, removing any remaining argument-passing
overhead. Thus being identical to the ideal case where the high-level function
would directly contain optimized AVX or SSE code.

`Overhead comparison`_
----------------------

The following plot compares the three approaches. Apart from the weird outlier
with a function pointer in a dynamic library that I can't explain, it shows
that a regular function call is the clear winner in case the code can be
compiled directly for the hardware the it will run on. IFUNC ultimately isn't
any faster than regular pointers, but isn't really slower either, in this
microbenchmark at least. I suppose in real-world scenarios it could benefit at
least from cache locality with other dynamic functions.

.. plot:: Dispatch overhead, Linux x86-64
    :type: barh
    :labels:
        Regular function
        Function pointer
        IFUNC function
        Regular function
        Function pointer
        IFUNC function
        Dispatch on every call
    :labels_extra:
        ..
        ..
        ..
        in a dynamic library
        in a dynamic library
        in a dynamic library
        in a dynamic library
    :units: ns
    :values: 1.26 1.51 1.52 1.52 1.25 1.50 2.50
    :errors: 0.06 0.06 0.08 0.07 0.05 0.06 0.08
    :colors: success info info info dim info danger

..  BENCH [35]   1.26 ± 0.06   ns benchmarkTagDispatchedCompileTime()@999x1000000 (wall time)
..  BENCH [36]   1.51 ± 0.06   ns benchmarkTagDispatchedPointer()@999x1000000 (wall time)
..  BENCH [37]   1.52 ± 0.08   ns benchmarkTagDispatchedIfunc()@999x1000000 (wall time)
..  BENCH [38]   1.52 ± 0.07   ns benchmarkTagDispatchedExternalLibraryCompileTime()@999x1000000 (wall time)
..  BENCH [39]   1.25 ± 0.05   ns benchmarkTagDispatchedExternalLibraryPointer()@999x1000000 (wall time)
..  BENCH [40]   1.50 ± 0.06   ns benchmarkTagDispatchedExternalLibraryIfunc()@999x1000000 (wall time)
..  BENCH [41]   2.50 ± 0.08   ns benchmarkTagDispatchedExternalLibraryEveryCall()@999x1000000 (wall time)

.. block-success:: Testing considerations

    There's one hidden benefit with the function pointer approach --- being
    able to change the pointer later at runtime. Together with publicly
    exposing the :cpp:`memrchrImplementation(Cpu::Features)` dispatcher this is
    very useful for testing, as a test can go through all variants and verify
    each without having to recompile for a different target. Not only that,
    it's even possible for a benchmark to supply alternative implementations to
    compare against the builtin ones. This is how a test case verifying all
    three alternatives could look like with Corrade's
    :dox:`TestSuite <TestSuite::Tester>`:

    .. include:: cpu-feature-detection-dispatch/main.cpp
        :code: c++
        :start-after: // [testing]
        :end-before: // [/testing]

`Compiling different functions for different targets`_
======================================================

In order to use intrinsics for a particular CPU instruction set, GCC and Clang
require the code to be compiled with a corresponding target option, such as
``-mavx2`` for AVX2. Such requirement makes sense, as it allows the compiler to
perform its own optimizations on top of the explicit intrinsics calls. Having a
separate file for every variant would be quite impractical though, fortunately
one can use :cpp:`__attribute__((target))` to enable instruction sets just on
particular functions, allowing all variants to live in the same file.\ [5]_

This is exposed via macros such as :dox:`CORRADE_ENABLE_AVX2`, and additionally
each macro is defined only if compiling for a matching architecture and the
compiler supports given instruction set. Which can be conveniently used to
guard the code to be only compiled where it makes sense:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [enable-macros]
    :end-before: // [/enable-macros]

MSVC, on the other hand, doesn't require any option in order to use any
intrinsics, so there the macros are empty. It however also means that it will
only apply the baseline optimizations, so for example extracting all AVX+ code
to a file with ``/arch:AVX`` enabled might have some perf benefits.\ [5]_

.. transition:: ^ v ^

`And then the reality hits`_
============================

So far, to make the code shine, I was holding back on a certain important
aspect of the instruction set landscape. In that it's *definitely not* a linear
sequence of instruction sets where each is a clear superset of the previous
one. Rather, it looks more like this:

.. container:: m-row m-container-inflate

    .. container:: m-col-m-12

        .. digraph:: x86 instruction family tree

            rankdir=RL
            ranksep=0.25

            node [shape=circle style=filled margin="0.01,0.01" width=1]

            FMA [width=0.6 fontsize=13]
            F16C [width=0.6 fontsize=13]
            LZCNT [width=0.6 fontsize=13]
            POPCNT [width=0.6 fontsize=11]
            BMI1 [width=0.6 fontsize=13]

            SSE2 [class="m-default"]
            SSE3 [class="m-default"]
            SSSE3 [class="m-default"]
            SSE41 [class="m-default" label="SSE4.1"]
            SSE42 [class="m-default" label="SSE4.2"]

            POPCNT -> SSE3 [style=invis constraint=false]
            LZCNT -> SSE3 [style=invis]
            LZCNT -> POPCNT [style=invis]
            BMI1 -> LZCNT [style=invis]

            FMA -> AVX
            F16C -> AVX

            FMA -> F16C [style=invis]

            AVX [class="m-default"]
            AVX2 [class="m-default"]
            AVX512F [class="m-default" label="AVX512F\nAVX512CD" fontsize=13]
            AVX512VLDQBW [label="AVX512VL\nAVX512DQ\nAVX512BW" fontsize=11]
            VBMI2 [label="VBMI2\nBITALG\nVPCLMULQDQ\nGFNI\nVAES" fontsize=8 with=1 fixedwith=true]

            AVX512VLDQBW -> AVX512F -> AVX2 -> AVX -> SSE42 -> SSE41 -> SSSE3 -> SSE3 -> SSE2 [class="m-default"]

            SSE4a [width=0.6 fontsize=13]
            SSE5 [width=0.6 fontsize=13]
            XOP [width=0.6 fontsize=13]
            FMA4 [width=0.6 fontsize=13]

            AVX512ERPF [label="AVX512ER\nAVX512PF" fontsize=13]
            FMAPS [label="4FMAPS\n4VNNIW\nVPOPCNTDQ" fontsize=10]
            AVX512ERPF -> AVX512F
            FMAPS -> AVX512ERPF
            VBMI2 -> AVX512VLDQBW

            SSE4a -> SSE3
            SSE5 -> SSSE3
            FMA4 -> AVX
            XOP -> AVX

            XOP -> FMA4 [style=invis]

        .. class:: m-text m-small m-dim m-text-center

            Please note there's *many* instruction sets still omitted and the hierarchy
            likely contains severe errors.

With the passing of time many of these branches fortunately went abandoned,
however there's still many cases where any ordering is just impossible. For
example, there are AVX CPUs that have `F16C <https://en.wikipedia.org/wiki/F16C>`_
but not `FMA <https://en.wikipedia.org/wiki/FMA_instruction_set>`_, there are
AVX CPUs with FMA but no F16C, and there's apparently even a
`VIA CPU with AVX2 but no FMA <https://stackoverflow.com/a/50829580>`_. To
account for all these cases, I ended up differentiating between a
*base instruction set* that has clear ordering and *extra instruction sets*
that have no relations whatsoever. For AVX-512 I'm not 100% sure yet, but
I'm hoping it'll `eventually converge as well <https://news.ycombinator.com/item?id=31533314>`_.

.. container:: m-row m-container-inflate

    .. container:: m-col-m-12

        .. digraph:: x86 instruction family tree

            rankdir=RL
            ranksep=0.25

            node [shape=circle style=filled margin="0.01,0.01" width=1]

            FMA [width=0.6 fontsize=13 class="m-primary"]
            F16C [width=0.6 fontsize=13 class="m-primary"]
            LZCNT [width=0.6 fontsize=13 class="m-primary"]
            POPCNT [width=0.6 fontsize=11 class="m-primary"]
            BMI1 [width=0.6 fontsize=13 class="m-primary"]

            SSE2 [class="m-success"]
            SSE3 [class="m-success"]
            SSSE3 [class="m-success"]
            SSE41 [class="m-success" label="SSE4.1"]
            SSE42 [class="m-success" label="SSE4.2"]
            AVX [class="m-success"]
            AVX2 [class="m-success"]
            AVX512F [class="m-success" label="AVX512F\nAVX512CD" fontsize=13]
            AVX512VLDQBW [label="AVX512VL\nAVX512DQ\nAVX512BW" fontsize=11]
            VBMI2 [label="VBMI2\nBITALG\nVPCLMULQDQ\nGFNI\nVAES" fontsize=8 with=1 fixedwith=true]

            POPCNT -> SSE3 [style=invis constraint=false]
            LZCNT -> SSE3 [style=invis]
            LZCNT -> POPCNT [style=invis]
            BMI1 -> LZCNT [style=invis]

            AVX512VLDQBW -> AVX512F [weight=1]

            FMA -> AVX [class="m-primary"]
            F16C -> AVX [class="m-primary"]

            FMA -> F16C [style=invis]

            AVX512F -> AVX2 -> AVX -> SSE42 -> SSE41 -> SSSE3 -> SSE3 -> SSE2 [class="m-success" weight=1]

            SSE4a [width=0.6 fontsize=13 class="m-dim"]
            SSE5 [width=0.6 fontsize=13 class="m-dim"]
            XOP [width=0.6 fontsize=13 class="m-dim"]
            FMA4 [width=0.6 fontsize=13 class="m-dim"]

            AVX512ERPF [label="AVX512ER\nAVX512PF" fontsize=13 class="m-dim"]
            FMAPS [label="4FMAPS\n4VNNIW\nVPOPCNTDQ" fontsize=10 class="m-dim"]
            AVX512ERPF -> AVX512F [class="m-dim"]
            FMAPS -> AVX512ERPF [class="m-dim"]
            VBMI2 -> AVX512VLDQBW

            SSE4a -> SSE3 [class="m-dim"]
            SSE5 -> SSSE3 [class="m-dim"]
            FMA4 -> AVX [class="m-dim"]
            XOP -> AVX [class="m-dim"]

            XOP -> FMA4 [style=invis]

        .. class:: m-text m-small m-text-center m-noindent

            :label-success:`base sets` |o| :label-default:`expected continuation` |o| :label-primary:`extra sets` |o| :label-dim:`abandoned sets`

To make this work, the dispatch has to select among variants with both the base
instruction set and all extra sets supported, and these two priority rules:

-   If one variant has the base instruction set more advanced than another,
    it's preferred (which is same as before). For example, if there's an
    AVX-512 variant and an AVX+FMA variant, AVX-512 gets picked.
-   Otherwise, if both variants have the same base instruction set, a variant
    that uses more extra instruction sets is preferred. For example, if there's
    a plain AVX variant and an AVX+FMA variant, AVX+FMA gets picked.

There's deliberately no ordering defined among the extra instruction sets. Thus
having for example a SSE2+POPCNT and a SSE2+LZCNT variant would lead to an
ambiguity if the machine supported both `POPCNT <https://en.wikipedia.org/wiki/X86_Bit_manipulation_instruction_set#ABM_(Advanced_Bit_Manipulation)>`_
and `LZCNT <https://en.wikipedia.org/wiki/X86_Bit_manipulation_instruction_set#ABM_(Advanced_Bit_Manipulation)>`_.
This can only be resolved by the implementation itself, by providing a
SSE2+POPCNT+LZCNT variant and delegating to whichever is the better option in
that case.

`Now comes the pile of nasty templates`_
========================================

Let's say there's a :cpp:`Tag<bits>` type that stores the bitmask of both the
base instruction set and the extra sets. An *extra* instruction set is always
represented by just a single unique bit, while a *base* instruction set
contains one unique bit for itself plus also all bits for the instruction sets
it's based on. Thus, for example, SSE2 would be :cpp:`Tag<0b00000001>`, SSE3
:cpp:`Tag<0b00000011>`, SSSE3 :cpp:`Tag<0b00000111>` and so on, while POPCNT
would be :cpp:`Tag<0b01000000>` and LZCNT :cpp:`Tag<0b10000000>`. Then,
combined, SSE3+POPCNT would be :cpp:`Tag<0b01000011>`. Finally, the class
provides a conversion operator to another :cpp:`Tag<>` that's enabled only if
the resulting bit mask is a subset:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [tag-conversion-operator]
    :end-before: // [/tag-conversion-operator]

The priority ordering is defined primarily by the index of the base instruction
set, and secondarily by the count of extra instruction sets. Which, given an
upper bound on the count of extra instruction sets, can be represented with a
single number --- for example, SSE2 would have a priority number :cpp:`100`,
SSE3 :cpp:`200`, SSSE3 :cpp:`300` and so on, SSE2+LZCNT+POPCNT would be
:cpp:`102` and SSE3+POPCNT :cpp:`201`. Here's where the core idea of
inheritance hierarchy gets reused again, just in a calculated manner:

.. code-figure::

    .. include:: cpu-feature-detection-dispatch/main.cpp
        :code: c++
        :start-after: // [calculated-priority]
        :end-before: // [/calculated-priority]

    .. class:: m-text m-small m-dim m-noindent

    I claim no invention here --- credit goes to :gh:`sthalik` who
    `suggested the priority tag idea to me <https://gist.github.com/sthalik/0828be17146ddad36f4a3e11b47cbbcc>`_.

To make these work together, the dispatch has to use two function arguments ---
one is where the candidate filtering happens, and the other orders by priority.
In the following example, calling with :cpp:`Tag<0b11000011>` and
:cpp:`Priority<202>` (SSE3+LZCNT+POPCNT), would pick the SSE3+POPCNT overload:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [tag-priority-declaration]
    :end-before: // [/tag-priority-declaration]

Because I was unable to figure out a way that wouldn't involve using two
parameters, the user-facing API hides this whole process behind two macros ---
:dox:`CORRADE_CPU_DECLARE()` and :dox:`CORRADE_CPU_SELECT()`. Together with an
ability to combine the :dox:`Cpu` tags with bitwise operations, the process of
declaration and compile-time dispatch looks like this:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [use-extra]
    :end-before: // [/use-extra]

Finally, the runtime dispatcher then has to take the extra instruction sets
into account as well. Implicitly considering all possible combinations would
however lead to a lot of work for the compiler. As a tradeoff, since in
practice the variants usually need at most one or two extra sets, the
:dox:`CORRADE_CPU_DISPATCHER()` macro requires you to list them explicitly:

.. include:: cpu-feature-detection-dispatch/main.cpp
    :code: c++
    :start-after: // [dispatcher-extra]
    :end-before: // [/dispatcher-extra]

This macro produces a :cpp:`foo(Cpu::Features)` which can be again used with
:dox:`CORRADE_CPU_DISPATCHED_POINTER()` or :dox:`CORRADE_CPU_DISPATCHED_IFUNC()`.
For brevity I glossed over the nastier implementation details, if you're
interested please :gh:`dive into the source <mosra/corrade/blob/simd/src/Corrade/Cpu.h>`.

.. transition:: ~ ~ ~

`Practical usability verification`_
===================================

To ensure sanity of this design, I proceeded with using the :dox:`Cpu` APIs in
a practical scenario --- implementing a :dox:`std::memchr()` alternative to use
in :dox:`Containers::StringView::find() <Containers::BasicStringView::find()>`.
Turns out I was lucky, having not really any prior experience writing
intrinsics for any architecture, I managed to pick a sufficiently hard problem
where

-   compiler autovectorization doesn't do a "good enough" job,
-   the standard implementation is well optimized and not easy to beat,
-   the algorithm needs to have specialized handling for small and large inputs
    in order to be fast,
-   I get to use also the extra instruction sets such as `BMI1 <https://en.wikipedia.org/wiki/X86_Bit_manipulation_instruction_set#BMI1_(Bit_Manipulation_Instruction_Set_1>`_,
-   and the ideal way is *significantly* different between x86, ARM and
    WebAssembly.

Apart from having the API usability confirmed (and a ton of ugly compiler bugs
discovered), I realized that in order to really make the most of every
platform, it doesn't make sense to try to come up with an instruction-level
abstraction API like is for example in :gh:`SIMD everywhere <simd-everywhere/simde>`.
So that's something Magnum will not have. The building blocks need to be much
more high level.

Since I'm very new to all this, I won't embarrass myself further by trying to
pretend I know what I'm talking about. Hopefully in a later post --- here's
just a sneak peek at the :gh:`code that's now in Corrade master <mosra/corrade/blob/c17fc86e4234bfab592a11fd40c2493f41c38f60/src/Corrade/Containers/StringView.cpp#L157-L703>`:

.. plot:: Character lookup
    :type: barh
    :labels:
        std::memchr()
        StringView::find()
        StringView::find()
        StringView::find()
        StringView::find()
    :labels_extra:
        baseline
        x86 SSE2+BMI1 (Linux)
        x86 AVX2+BMI1 (Linux)
        ARM NEON (Apple M1)
        WASM SIMD128 (Node.js)
    :units: time relative to std::memchr()
    :values: 1.0 1.2098 0.8908 0.7013 0.1369
    :errors: 0.0 0.0518 0.0720 0.0065 0.0274
    :colors: info warning success success success

.. x86:
..
..  BENCH [13]   1.09 ± 0.19   µs findCharacterRare(Sse2|Bmi1)@99999x100 (wall time)
..  BENCH [14] 802.54 ± 136.76 ns findCharacterRare(Avx2|Bmi1)@99999x100 (wall time)
..  BENCH [16] 900.96 ± 154.93 ns findCharacterRareMemchr()@99999x100 (wall time)
..
.. M1:
..
..  BENCH [07]   1.08 ± 0.01   µs findCharacterRare(Neon)@99x100 (wall time)
..  BENCH [09]   1.54 ± 0.03   µs findCharacterRareMemchr()@99x100 (wall time)
..
.. WASM (x86)
..
..  BENCH [07]   1.25 ± 0.25   µs findCharacterRare(Simd128)@9999x100 (wall time)
..  BENCH [09]   9.13 ± 1.18   µs findCharacterRareMemchr()@9999x100 (wall time)

`CPU features and dispatch on ARM platforms`_
=============================================

Instruction-set-wise, ARM has the basically ubiquitous NEON. Together with a
few recognized extensions, it's detected as :dox:`Cpu::Neon`,
:dox:`Cpu::NeonFma` and :dox:`Cpu::NeonFp16`. Apart from NEON the main
instructions sets are SVE and SVE2 and Apple's own proprietary
`AMX <https://gist.github.com/dougallj/7a75a3be1ec69ca550e7c36dc75e0d6f>`_
(which doesn't even have any publicly available compiler support yet). I'll
wire up detection for these once they become more common.

Compared to the x86 CPUID instruction, runtime detection of ARM features has to
rely on OS-specific calls such as :doc-link:`getauxval() <https://man.archlinux.org/man/getauxval.3>`
on Linux or :doc-link:`sysctlbyname() <https://developer.apple.com/documentation/kernel/1387446-sysctlbyname>`
on Apple platforms. What's nice is that both Linux and Android (API 18+)
support IFUNC dispatch on ARM as well. It's complicated by the fact that
``getauxval()`` is a call to an external library that's not available at the
time IFUNC pointers get resolved so instead it's fed to the resolver from
outside. Android however adopted such behavior only since API 30 (Android 11).
To account for this, the :dox:`CORRADE_CPU_DISPATCHED_IFUNC()` macro is
special-cased for ARM and enabled only on glibc and Android 30+. Overhead-wise,
IFUNCs on ARM are comparable to x86.

Practical usage of ARM NEON intrinsics highlighted two things. First, it's a
very different instruction set from x86, so naively trying to emulate
instructions like `movemask <https://www.felixcloutier.com/x86/pmovmskb>`_
is not going to be fast. Instead, `using features unique to NEON <https://twitter.com/Danlark1/status/1539344281336422400>`_
can yield significant speedups. Second, in my experiments at least, GCC seems
to be significantly worse than Clang `in dealing with ARM intrinsics <https://stackoverflow.com/questions/50028458/how-to-treat-result-of-vaddv-u8-in-arm64-as-a-neon-register#comment87123343_50028458>`_.
The standard :dox:`std::memchr()` implementation is usually written in plain
assembly, and while I was able to get my intrinsics to a comparable speed using
Clang, it was plain impossible with GCC. I wonder whether it's the cause or the
consequence of Android and macOS/iOS, the major ARM platforms, nowadays
exclusively using Clang. Or maybe I'm just doing something wrong, again this is
all new to me.

`WebAssembly 128-bit SIMD`_
===========================

WebAssembly currently provides a single 128-bit SIMD instruction set, detected
as :dox:`Cpu::Simd128`. The situation is a bit specific, as security on the
web will always have a priority over performance. WebAssembly modules are
statically verified upfront and if they fail the validation --- for example
because an unknown instruction was encountered --- they're rejected. Since
WebAssembly SIMD is relatively new, this means that a binary can either use
any SIMD instructions anywhere, or nowhere at all, to pass validation on VMs
that don't know about SIMD yet.

While that makes any sort of runtime dispatch rather useless, perhaps the more
concerning issue is that runtime dispatch is *prohibitively expensive* ---
going through an arbitrary function pointer has a *fourty times* bigger
overhead than calling a function directly. The following numbers are from
Node.js, but both Chromium and Firefox showed a similar disparity.

.. plot:: Dispatch overhead, Emscripten and WebAssembly
    :class: m-nopadb
    :type: barh
    :labels:
        Function
        Function pointer
        Function
        Function pointer
        Dispatch on every call
    :labels_extra:
        ..
        ..
        in a library
        in a library
        in a library
    :units: ns
    :values: 0.30 0.31 0.50 21.43 21.76
    :errors: 0.04 0.05 0.03 0.51 0.64
    :colors: success success info danger danger

.. class:: m-text m-small m-dim m-text-center

    Honestly I hope I just forgot to enable some optimization. This is too
    much.

..  BENCH [35]   0.30 ± 0.04   ns benchmarkTagDispatchedCompileTime()@499x1000000 (wall time)
..  BENCH [36]   0.31 ± 0.05   ns benchmarkTagDispatchedPointer()@499x1000000 (wall time)
..  BENCH [38]   0.50 ± 0.03   ns benchmarkTagDispatchedExternalLibraryCompileTime()@499x1000000 (wall time)
..  BENCH [39]  21.43 ± 0.51   ns benchmarkTagDispatchedExternalLibraryPointer()@499x1000000 (wall time)
..  BENCH [41]  21.76 ± 0.64   ns benchmarkTagDispatchedExternalLibraryEveryCall()@499x1000000 (wall time)

While there's currently no way to perform runtime detection of supported CPU
features, there's a `Feature Detection proposal <https://github.com/WebAssembly/feature-detection/blob/main/proposals/feature-detection/Overview.md>`_
aiming to cover this gap. But even then, unless the overhead situation
improves, runtime dispatch will only be useful for heavier chunks of code ---
definitely not for things like :cpp:`memchr()`.

Practical usage of WebAssembly SIMD128 intrinsics only further emphasised that
the difference between x86 and ARM isn't something to be ignored. I have two
variants of my code where using :doc-link:`wasm_i8x16_any_true() <https://github.com/WebAssembly/simd/blob/main/proposals/simd/SIMD.md#boolean-horizontal-reductions>`
instead of :doc-link:`wasm_i8x16_bitmask() <https://github.com/WebAssembly/simd/blob/main/proposals/simd/SIMD.md#bitmask-extraction>`
makes code 20% faster on ARM, while at the same time making it almost 30%
slower on x86. I could use `a hack to distinguish between x86 and ARM <https://github.com/mozilla/standards-positions/issues/651#issuecomment-1157847992>`_
and then choose an appropriate variant at runtime, but again, considering the
overhead of runtime dispatch this would be far worse than having just a single
non-dispatched variant.

For now at least, given there are `users with 99% of their audience on mobile platforms <https://wonderlandengine.com>`_,
I'm considering providing a compile-time option to prefer ARM-friendly
variants. Which would reuse the concept of extra instruction sets described
above, for example as :cpp:`Cpu::Simd128|Cpu::WasmArm`. And once the overhead
situation improves, the variants can directly work with a runtime dispatch.

`POWER, RISC-V`_
================

I ... have no idea, sorry. There's :dox:`CORRADE_TARGET_POWERPC` but that's
about it. It doesn't however mean the code will stop compiling on these
platforms --- since none of the ``CORRADE_ENABLE_*`` macros is defined there,
the code will fall back to :dox:`Cpu::Scalar` variants, behaving just like
before the CPU dispatch was introduced.

`Next steps`_
=============

While the :dox:`Cpu` library can be considered stable and ready for production
use, Corrade itself has runtime CPU dispatch enabled only on Linux if IFUNC is
detected to be available. Other platforms have it currently off by default,
until I'm 100% confident about the overhead. In any case, you can use the
:dox:`CORRADE_BUILD_CPU_RUNTIME_DISPATCH` and :dox:`CORRADE_CPU_USE_IFUNC`
CMake options to toggle this functionality at build time.

For the actual SIMD-enabled algorithms, of course the usefulness of *narrowly*
beating :dox:`std::memchr()` is dubious. But the code can now be trivially
modified to implement the above-mentioned
:doc-link:`memrchr() <https://man.archlinux.org/man/memrchr.3>` as well as
making `optimized two- and more-character variants <https://docs.rs/memchr/2.3.0/memchr/fn.memchr3.html>`_.
That's where the *actual* gains will be, as multi-character lookup is a common
bottleneck in JSON (glTF) or OBJ parsing, and standard libraries don't really
have a dedicated tool for that.

Among other features that :s:`were planned to use SIMD someday` were
desperately waiting for the CPU dispatch to get ready are --- hierarchical
transformation calculations, batch frustum culling and other bulk processing
utilities to make use in your ECS workflows. Exciting times ahead!

`Single-header implementation`_
===============================

Like with various :dox:`Containers` implementations, this feature is
self-contained enough to make sense as a standalone library. To make it
possible to use it without relying on the whole of Corrade, it's provided as a
single-header library in the :gh:`Magnum Singles <mosra/magnum-singles>`
repository. To use it, simply  download the file and :cpp:`#include` it your
project:

.. |n| replace:: :label-success:`new`

.. class:: m-table m-fullwidth

========================= ==== ================ ===============================
Library                   LoC  Preprocessed LoC Description
========================= ==== ================ ===============================
`CorradeCpu.hpp`_ |n|     1646 2085             See the :dox:`Cpu` namespace docs
========================= ==== ================ ===============================

.. _CorradeCpu.hpp: https://github.com/mosra/magnum-singles/blob/master/CorradeCpu.hpp

To put the header size into perspective, just :cpp:`#include <immintrin.h>` for
all AVX intrinsics yields *34 thousand* preprocessed lines on GCC. Compared to
when `I was bitching about <memory> being heavy <{filename}/blog/backstage/lightweight-stl-compatible-unique-pointer.rst>`_,
this is a whole other level. And honestly I don't know what could fix this,
there's simply *a lot* in AVX-512.

.. transition:: . : .

.. note-info::

    Questions? *Complaints?* Express your disapproval on social networks:
    `Twitter <https://twitter.com/czmosra/status/1554485603206569986>`_,
    `Reddit r/cpp <https://www.reddit.com/r/cpp/comments/weegza/convenient_cpu_feature_detection_and_dispatch_in/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=32319750>`_

.. [1] The original prototype appeared in :gh:`mosra/corrade#115` in March
    2021, which was a spin-off of :gh:`mosra/magnum#306`, dating back to
    January 2019. Yup, certain features *take time* to brew.
.. [2] The :cpp:`+` before the lambda "decays" it into a regular function
    pointer, and that's what the function returns. I used C++14 for brevity,
    but the :dox:`Cpu` library works in plain C++11 as well, just with a bit
    more typing instead of the :cpp:`auto` return type. I'm happy being the
    caveman that's still using C++11.
.. [3] Except for :gh:`that one case <golang/go#28431>` where ARM big.LITTLE
    cores each reported a slightly different instruction set, leading to
    crashes when you queried CPU features on the more advanced core and ended
    up running advanced instructions on the core that didn't have them.
.. [4] The explanation is grossly oversimplified, but *dynamic library function
    call* is the key point here. Calls into dynamic libraries are indirect,
    meaning they're kinda like a function pointer as well. The difference is,
    however, that they're resolved at load time, constant for the application
    lifetime and all contained in a single place, which has the potential of
    reducing overhead compared to having to fetch a function pointer value from
    a random location before every call.
.. [5] Again I'm just scratching the surface here. Enabling AVX2 for the whole
    file may cause AVX2 to be used also in functions where you use just SSE2
    intrinsics and thus crashing on machines without AVX2 support. That is
    rather obvious, but similar cases `could happen also in case LTO combines code from multiple files <https://www.reddit.com/r/cpp/comments/eneib0/detecting_sse_features_at_runtime/fdzqbqg?utm_source=share&utm_medium=web2x&context=3>`_
    or `when a single template function gets compiled for different instruction sets and then the linker deduplicates it <https://www.reddit.com/r/cpp/comments/eneib0/detecting_sse_features_at_runtime/fdzqbqg?utm_source=share&utm_medium=web2x&context=3>`_.
    Computers are hard, apparently, and I hope most of this got solved since.
