Lightweight but still STL-compatible unique pointer
###################################################

:date: 2019-01-16
:modified: 2019-01-28
:category: Backstage
:tags: C++, singles
:summary: Magnum got a new unique pointer implementation that's much more
    lightweight with better debug performance and compile times, but is still
    fully compatible with :dox:`std::unique_ptr`.

.. role:: cpp(code)
    :language: c++

.. note-success:: Content care: Jan 28, 2019

    Links to single-header libs mentioned at the end of the article were
    updated to match the current state.

Magnum is currently undergoing an optimization pass for shorter compilation
times and smaller binary sizes, further improving on
`what was done back in 2013 <{filename}reducing-cpp-compilation-time-in-magnum-code-optimizations.rst>`_.
Back then I managed to reduce amount of template instantiations and remove use
of the then-most-heavy :cpp:`#include`\ s such as ``<iostream>`` or
``<algorithm>`` from headers, while also banning several others such as
``<regex>`` or ``<random>`` from ever leaking there. Back then, with a 2013
hardware and GCC 4.8, that resulted in compile times being down from 5:00 to
2:59, which was already a significant improvement.

Nowadays Magnum compiles *with all tests* in 80 seconds. The codebase got
considerably bigger during the past five years, but Moore's law is also still
in effect, so one could say that the best solution for improving compile times
is to just *wait a bit*. (Similarly as the best way to cure insomnia is to get
more sleep.) But, especially after seeing
`what's possible with plain C <https://floooh.github.io/2018/05/01/cpp-to-c-size-reduction.html>`_,
I'm not completely happy with current state and I think I could do better.

.. transition:: ~ ~ ~

The problem with removing the most significant causes of slowdown is that
something else steps up to become the most problematic. Now things like
``<vector>`` or ``<string>`` are among the top offenders and, apart from
replacing :dox:`std::vector` with Magnum's lightweight :dox:`Containers::Array`
where possible, the most efficient cure is to `PIMPL <https://en.wikipedia.org/wiki/Opaque_pointer>`_
the class internals to remove them from class definitions. That works for
almost everything. Except for :dox:`std::unique_ptr`, because that one is often
used to wrap the PIMPL itself since you definitely *do not want* to implement
copy/move constructors for each PIMPL'd class instead.

To my great surprise, the ``<memory>`` header is quite a beast, twice as big
as ``<vector>`` (which, well, has to handle all the complex move-aware
reallocations) and it only gets worse with newer C++ standards. It's actually
even slightly bigger than ``<iostream>`` which I banned for this very reason!

Below is a graph of preprocessed line count for each header, generated using
the following command with GCC 8.2. Note the use of ``-P``, which removes
unnecessary :cpp:`#line` statements from the preprocessor output, making the
resulting line count more corresponding to the amount of actual code. The last
line in the plot, for comparison, is using Clang 7.0 with libc++. While
preprocessed line count is not the only factor affecting compile times, it
correlates with it pretty well, especially in template-heavy C++ code.

.. code:: sh

    echo "#include <memory>" | gcc -std=c++11 -P -E -x c++ - | wc -l

.. plot:: Preprocessed line count
    :type: barh
    :labels:
        <vector>
        <vector> + <string>
        <iostream>
        <memory>
        <memory>
        <memory>
    :labels_extra:
        ..
        ..
        ..
        ..
        libstdc++, C++17
        libc++, C++2a
    :units: lines
    :values: 8608 14652 17839 17863 20995 16736
    :colors: primary primary info warning warning warning

`Let's step back a bit and try again`_
======================================

Imposing the burden of 17k lines on every user of the class would absolutely
destroy any benefits of PIMPLing away the ``<vector>`` and ``<string>``
includes, as the ``<memory>`` header alone is bigger than those two combined.
The crazy part is that it's just *a move-only wrapper over a pointer*.

The new :dox:`Containers::Pointer` is also just that, but in a reasonably-sized
package. Unlike :dox:`std::unique_ptr` it doesn't support arrays (Magnum has
:dox:`Containers::Array` for that) and at the moment it doesn't have custom
deleters, as there was no immediate need for this feature. On the other hand,
it provides an equivalent to :cpp:`std::make_unique()` *without* forcing you to
use C++14. It's named just ``Pointer``, because I already have an ``Array`` and
I don't ever plan on implementing an alternative to :dox:`std::shared_ptr`,
because, in my opinion, the only purpose of that type is making coding crimes
easier to commit.

Let's look at it again:

.. plot:: Preprocessed line count
    :type: barh
    :labels:
        <Containers/Pointer.h>
        <Containers/Pointer.h>
        <memory>
        <memory>
    :labels_extra:
        C++11
        C++2a
        C++11
        C++2a
    :units: lines
    :values: 2311 2769 17863 21014
    :colors: success success warning warning

It could be smaller, but I needed ``<type_traits>`` to do some convenience
compile-time checks (one of them is forbidding its use on :cpp:`T[]`). And for
in-place construction using :dox:`Containers::pointer()`, I needed
:dox:`std::forward()` from ``<utility>``. I
`could have used <https://twitter.com/czmosra/status/1085195308881985536>`_
:cpp:`static_cast` instead and saved myself ~700 lines of code, but the header
is so essential that you'll be including yourself sooner or later anyway.

`Compile times and debug performance`_
======================================

For a "microbenchmark" of compile times, I created the following two code
snippets and compiled each with GCC 8.2. For better sense of scale, there's
also a baseline time, which is from compiling just :cpp:`int main() {}` with no
:cpp:`#include` at all.

.. container:: m-row

    .. container:: m-col-m-6 m-nopadr m-nopadt

        .. code:: c++
            :class: m-inverted
            :hl_lines: 1 5 6 7 8

            #include <Corrade/Containers/Pointer.h>

            using namespace Corrade;

            int main() {
              Containers::Pointer<int> a{new int{}};
              return *a;
            }

    .. container:: m-col-m-6 m-nopadt

        .. code:: c++

            #include <memory>

            int main() {
              std::unique_ptr<int> a{new int{}};
              return *a;
            }

By default, :dox:`Containers::Pointer` has a convenience printer for
:dox:`Utility::Debug` and also provides human-readable assertions using the
same utility. To make the comparison more balanced, I opted-out of debug
printing and switched to standard C :cpp:`assert()` by defining
:cpp:`CORRADE_NO_DEBUG` and :dox:`CORRADE_STANDARD_ASSERT` on the compiler
command line. The resulting times are below:

.. code:: sh

    g++ main.cpp -DCORRADE_NO_DEBUG -DCORRADE_STANDARD_ASSERT -std=c++11 # or c++2a

.. plot:: Compilation time, GCC 8.2
    :type: barh
    :labels:
        baseline
        <Containers/Pointer.h>
        <Containers/Pointer.h>
        <memory>
        <memory>
    :labels_extra:
        int main() {}
        C++11
        C++2a
        C++11
        C++2a
    :units: ms
    :values: 49.97 69.74 71.41 205.19 249.01
    :errors: 0.54 3.04 0.84 1.05 4.72
    :colors: dim success success warning warning

.. Starting CompileTimeBenchmark with 9 test cases...
..  BENCH [1]  49.97 ± 0.54   ms baseline()@9x1 (wall time)
..  BENCH [2]  69.74 ± 3.04   ms pointer11()@9x1 (wall time)
..  BENCH [3]  71.41 ± 0.84   ms pointer2a()@9x1 (wall time)
..  BENCH [4]  64.66 ± 3.49   ms reference11()@9x1 (wall time)
..  BENCH [5]  66.29 ± 4.87   ms reference2a()@9x1 (wall time)
..  BENCH [6] 205.19 ± 1.05   ms uniquePtr11()@4x1 (wall time)
..  BENCH [7] 249.01 ± 4.72   ms uniquePtr2a()@4x1 (wall time)
..  BENCH [8] 173.60 ± 7.38   ms referenceWrapper11()@4x1 (wall time)
..  BENCH [9] 308.80 ± 7.76   ms referenceWrapper2a()@4x1 (wall time)
.. Finished CompileTimeBenchmark with 0 errors out of 70 checks.

Regarding debug performance, `checking on Compiler Explorer <https://gcc.godbolt.org/z/ybadJT>`_,
:dox:`std::unique_ptr` resulted in roughly four times as many instructions as
for :dox:`Containers::Pointer` in a non-optimized version on both Clang and
GCC. GCC with ``-O1`` and higher was able to reduce the above snippet to a pair
of :cpp:`new` and :cpp:`delete`, Clang with ``-O1`` shortened the code to
roughly half for both (but still with 3x difference) and Clang ``-O2`` and up
managed to get rid of the allocation altogherher in both cases, which is nice.

`What if my library already uses std::unique_ptr?`_
===================================================

.. TODO: doxygen ugh why the absolute paths

Magnum will be gradually switching to the new type in all APIs, but because I
don't want to make your life miserable, the type is able to *implicitly* morph
from and back into :dox:`std::unique_ptr`. A similar trick is already used in
the Magnum Math library for example for the :dox:`GLM math library integration <GlmIntegration>`.
The conversion is provided in a separate
:dox:`Corrade/Containers/PointerStl.h </home/mosra/Code/corrade/src/Corrade/Containers/PointerStl.h>`
header because, well, doing it directly in the class itself would require me to
:cpp:`#include <memory>` --- which I wanted to avoid in the first place. As a
side-effect of this, it also allows you to have an equivalent of
:cpp:`std::make_unique()` in C++11 --- :dox:`Containers::pointer()`:

.. code:: c++
    :class: m-inverted
    :hl_lines: 1 5 6 7 8 9 10

    #include <Corrade/Containers/PointerStl.h>

    using namespace Corrade;

    int main() {
        std::unique_ptr<int> a{new int{42}};
        Containers::Pointer<int> b = std::move(a);

        std::unique_ptr<int> c = Containers::pointer<int>(1337);
    }

This conversion behaves like any other usual move --- the original instance
gets :dox:`release() <Containers::Pointer::release()>`\ d, becoming :cpp:`nullptr`,
and the ownership moves to the other.

`The case of std::reference_wrapper`_
=====================================

I... I'm not even mad anymore. Just disappointed. Main use of this standard
type in Magnum APIs is to allow storing references (or non-nullable pointers)
in various containers. The :dox:`std::reference_wrapper` is *even simpler* than
:dox:`std::unique_ptr`, yet it's shoveled into the ``<functional>`` header,
which, while it was not exactly slim to begin with, it managed to gain an
*insane* amount of weight due to (I assume) the introduction of searchers in
C++17. Like, why not put these in ``<search>`` instead?! So I made my own
:dox:`Containers::Reference`, too (and it's also convertible to/from the STL
equivalent in a similar way).

.. plot:: Preprocessed line count
    :type: barh
    :labels:
        <Containers/Reference.h>
        <Containers/Reference.h>
        <functional>
        <functional>
    :labels_extra:
        C++11
        C++2a
        C++11
        C++2a
    :units: lines
    :values: 1646 2015 14540 31353
    :colors: success success warning danger

In this case I didn't even need ``<utility>``, so the header is just 1646
preprocessed lines under C++11. To wrap it up, here are compile times of the
following snippets, again with the baseline comparison for better scale:

.. container:: m-row

    .. container:: m-col-m-6 m-nopadr m-nopadt

        .. code:: c++
            :class: m-inverted
            :hl_lines: 1 5 6 7 8 9

            #include <Corrade/Containers/Reference.h>

            using namespace Corrade;

            int main() {
                int a{};
                Containers::Reference<int> b = a;
                return b;
            }

    .. container:: m-col-m-6 m-nopadt

        .. code:: c++

            #include <functional>

            int main() {
                int a{};
                std::reference_wrapper<int> b = a;
                return b;
            }

.. plot:: Compilation time, GCC 8.2
    :type: barh
    :labels:
        baseline
        <Containers/Reference.h>
        <Containers/Reference.h>
        <functional>
        <functional>
    :labels_extra:
        int main() {}
        C++11
        C++2a
        C++11
        C++2a
    :units: ms
    :values: 49.97 64.66 66.29 173.60 308.80
    :errors: 0.54 3.49 4.87 7.38 7.76
    :colors: dim success success warning danger

`But, but, ... modules?`_
=========================

The Modules work is running for half a decade already and many of the header
bloat concerns are being handwaved away with "modules will solve that". I
looked at the proposals back in 2016, but didn't have a chance to check back
since, so I was excited to see the progress.

TL;DR: *no, we're not there yet.*

While Modules are said to be on track for C++20 (I hope that's stil possible),
I was not able to find *any* real-world example that would work for me. After
much struggling, I managed to come up with this command-line:

.. code:: sh

    clang++ -std=c++17 -stdlib=libc++ -fmodules-ts -fimplicit-modules \
        -fmodule-map-file=/usr/include/c++/v1/module.modulemap main.cpp

And, after installing both ``libc++`` *and* ``libc++-experimental`` from
`AUR <https://aur.archlinux.org/packages/libc%2B%2B/>`_, the following snippet
compiled correctly. Various examples told me that I could :cpp:`import std.memory;`,
but that only greeted me with an ungoogleable error.

.. code:: c++

    import std;

    int main() {

        std::unique_ptr<int> a{new int{}};
        return *a;
    }

The measured compile times are below, but note the very first run takes almost
two seconds --- it's compiling the module file, resulting in *17 megabytes*
of various binaries in your temp directory. And you get a different set of
these for different flags, enabling ``-O3`` generates another set of binaries.
That ... feels pretty much like precompiled headers. Not sure if happy. *(I
didn't like those at all.)*

.. plot:: Compilation time, Clang 7.0 -std=c++17
    :type: barh
    :labels:
        baseline
        Containers::Pointer
        std::unique_ptr
        std::unique_ptr
    :labels_extra:
        int main() {}
        <Containers/Pointer.h>
        <memory>
        import std
    :units: ms
    :values: 82.93 108.01 279.79 90.86
    :errors: 0.78 4.51 4.15 4.14
    :colors: dim success warning info

.. Starting CompileTimeBenchmark with 4 test cases...
..  BENCH [10]  82.93 ± 0.78   ms clangBaseline()@9x1 (wall time)
..  BENCH [11] 108.01 ± 4.51   ms clangPointer()@9x1 (wall time)
..  BENCH [12] 279.79 ± 4.15   ms clangMemory()@9x1 (wall time)
..  BENCH [13]  90.86 ± 4.14   ms modules()@9x1 (wall time)
.. Finished CompileTimeBenchmark with 0 errors out of 40 checks.

I was looking forward to C++ modules to simplify library linking to the point
where you just say "this is the library I want to link to" on the command line
and it will feed both the linker with correct object code and the compiler with
correct imported definitions. Wishful thinking.

This is nowhere near that and the speed gains are not *that* significant
compared to responsible header hygiene. People with bigger codebases are
reporting `even smaller gains, around 10% <https://www.reddit.com/r/cpp/comments/640szr/trying_out_clang_5_modules_in_a_70k_loc_project/>`_,
which makes me wonder if this is worth bothering with, in the current state of
things. And using modules will not magically improve debug performance of STL
containers anyway.

What's worse is that the implementation is nowhere properly documented (Clang
Modules documentation is *not* about Modules TS, but their own different thing)
and there's no support in tools or IDEs (not to mention buildsystems), so at
the moment it's very painful to work with. I think I'll check back in another
five years.

`Take it, it's just a single file!`_
====================================

If you are already using Magnum, simply :cpp:`#include` these files and you're
ready to take back the control over your compile times. If not, these two
containers, along with :dox:`Containers::Optional`, are available through a
freshly created :gh:`magnum-singles <mosra/magnum-singles>` repository. Each is
a self-contained *tiny* header file, meant to be bundled into your project:

.. class:: m-table m-fullwidth

====================== === ================ ======================================
Library                LoC Preprocessed LoC Description
====================== === ================ ======================================
`CorradeOptional.h`_   328 2742             See :dox:`Containers::Optional` docs
`CorradePointer.h`_    259 2321             See :dox:`Containers::Pointer` docs
`CorradeReference.h`_  115 1639             See :dox:`Containers::Reference` docs
`CorradeScopeGuard.h`_ 108 26               See :dox:`Containers::ScopeGuard` docs
====================== === ================ ======================================

This repository will be receiving more libraries as Magnum will get gradually
slimmed down. You can already look forward to a math library that fits under
10k *preprocessed* lines :)

.. transition:: * * *

Questions? Complaints? Share your opinion on social networks:

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/1085545586860978178>`_,
    Reddit `r/cpp <https://www.reddit.com/r/cpp/comments/aglx82/lightweight_but_still_stlcompatible_unique/>`_,
    `r/gamedev <https://www.reddit.com/r/gamedev/comments/aglxgq/lightweight_but_still_stlcompatible_c_unique/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=18921349>`_

.. _CorradeOptional.h: https://github.com/mosra/magnum-singles/blob/master/CorradeOptional.h
.. _CorradePointer.h: https://github.com/mosra/magnum-singles/blob/master/CorradePointer.h
.. _CorradeReference.h: https://github.com/mosra/magnum-singles/blob/master/CorradeReference.h
.. _CorradeScopeGuard.h: https://github.com/mosra/magnum-singles/blob/master/CorradeScopeGuard.h
