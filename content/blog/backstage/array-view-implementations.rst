Array view implementations in Magnum
####################################

:date: 2019-02-18
:modified: 2019-02-19
:category: Backstage
:tags: C++, Corrade, Eigen, GLM, ImGui, Vulkan, singles
:summary: Similarly to the pointer and reference wrappers
    `described in the last article <{filename}lightweight-stl-compatible-unique-pointer.rst>`_,
    Magnum's array views recently received STL compatibility as well. Let's
    take that as an opportunity to compare them with the standard
    implementation in |span|.

.. role:: cpp(code)
    :language: c++
.. role:: s
    :class: m-text m-s

.. TODO: replace with :dox:`span` when the tag file has it
.. role:: link-flat(link)
    :class: m-flat
.. |span| replace:: :link-flat:`std::span <https://en.cppreference.com/w/cpp/container/span>`
.. |subspan| replace:: :link-flat:`std::span::subspan() <https://en.cppreference.com/w/cpp/container/span/subspan>``
.. |as_bytes| replace:: :link-flat:`std::as_bytes() <https://en.cppreference.com/w/cpp/container/span/as_bytes>`

This was meant to be a short blog post showing the new STL compatibility of
various :dox:`ArrayView <Containers::ArrayView>` classes. However, after diving
deep into |span|, there was suddenly much more to write about.

.. note-success:: Content care: Feb 19, 2019

    Apart from various clarifications and typo fixes, the `Magnum's array views`_
    section is now corrected to avoid spreading misinformation about :cpp:`operator[]` overhead.

`The story of waiting for a thing to get standardized...`_
==========================================================

Array views were undoubtely one of the main workflow-defining structures since
they were added to Magnum :gh:`almost six years ago <mosra/corrade@ca6f44b4225381f42dc5167d7faa2daf8232066f>`;
back then called ``ArrayReference``, as I didn't discover the idea of slicing
them yet. Suddenly it was no longer needed to pass references to
:dox:`std::vector` / :dox:`std::array` around, or --- the horror --- a pointer
and a size. Back then, still in the brave new C++11 world, I wondered how long
it would take before the standard introduces an equivalent, allowing me to get
rid of my own in favor of a *well-tested, well-optimized and well-known
implementation*.

Fast forward to 2019, we *might* soon be there with |span|, scheduled
for inclusion in C++2a. In the meantime, Magnum's :dox:`Containers::ArrayView`
stabilized, learned from its past mistakes and was used in so many contexts
that I dare to say it's feature-complete. In the process it received a
fixed-size variant called :dox:`Containers::StaticArrayView` and, most
recently, :dox:`Containers::StridedArrayView`, for easier iteration over sparse
data. I'll be showing its sparse magic in a later article.

`... and ultimately realizing it's not really what we want`_
============================================================

Much like :dox:`std::optional`, originally scheduled for C++11 but due to its
design becoming more and more complex (:cpp:`constexpr` support, optional
references, ...), causing it to be delayed until C++17; |span| is, in my
opinion, arriving way too late as well.

Instead of shipping a minimal viable implementation as soon as possible to get
codebases jump on it --- and let its future design adapt to user feedback ---
design-in-a-vacuum means C++2a will ship with a complex implementation and a
set of gudelines that users have to adapt to instead.

In short, the C++2a |span| provides:

-   the usual index-based and iterator access to elements of the view,
-   both dynamic-size and fixed-size array views in a single type (which, as I
    unfortunately soon realized, only complicates *everything* without having
    any real benefits)
-   implicit conversion from C-style arrays, :dox:`std::array` and
    :dox:`std::vector`,
-   as the only STL type uses *signed* integer for sizes (which, again, as I
    realized, only brings complications when interfacing with about every other
    code)
-   and a well-meant, but fundamentally broken *implicit* conversion from *any
    type* that contains a :cpp:`data()` and a :cpp:`size()` member. If that
    sounds dangerous, it's because it really is. More on that below.

Originally, |span| was meant to not only handle both dynamic and fixed-size
array views, but also multi-dimensional and strided views. *Fortunately* such
functionality was separated into `std::mdspan`_, to arrive probably no earlier
than in C++23 (again, *way too late*).

.. block-warning:: Span? Array view?

    The |span| was originally named :cpp:`std::array_view`, which to me
    personally conveys the meaning of a view on a contiguous memory range
    *much better*. However, we ended up with a span, because it seems the
    committee `felt like calling it a span that day <https://cor3ntin.github.io/posts/span/#is-span-a-view>`_.
    Together with lumping both dynamic and statically-sized views together
    while adding some further inconsistencies to the mix (such as signed
    sizes), I fear it only makes such a simple type harder to teach and reason
    about.

    To add more salt to the wound, C++17 has :dox:`std::string_view` (so *not*
    :cpp:`std::string_span`), it has *unsigned* sizes and is a :cpp:`const`
    view. So much for the consistency.

`Magnum's array views`_
=======================

So, what's :dox:`Containers::ArrayView` capable of? Like |span|, it can be
implicitly constructed from a C array reference, or explicitly from a pair of
pointer and a size. It's also possible to slice the array, equivalently to
|subspan| and friends:

.. code:: c++

    float data[] { 1.0f, 4.2f, 133.7f, 2.4f };
    Containers::ArrayView<float> a = data;

    // Multiply the first three items 10 times
    for(float& i: a.prefix(3)) i *= 10.0f;

Similarly it goes for statically-sized array views. It's possible to
convert between dynamically-sized and statically-sized array views using
fixed-size :dox:`slice<n>() <Containers::ArrayView::slice()>` and related APIs
--- again, |span| has that too:

.. code:: c++

    // Implicit conversion allowed only if data has 4 elements as well
    Containers::StaticArrayView<float, 4> b = data;

    // A function accepting a view on exactly three floats
    float min3(Containers::StaticArrayView<float, 3>) { ... }

    float min = min3(b.suffix<3>());

For debug performance reasons, the element access is *not* bounds-checked (in
fact, to reduce the iteration overhead even more, the views are implicitly
convertible to pointers instead of providing custom iterators or an
:cpp:`operator[]`). On the other hand, slicing *is* checked, so iterating over
a slice is preferred over manually calculating an index subrange and indexing
that way. If you step over with your slice, you'll get a detailed Python-like
assertion message:

.. code-figure::

    .. code:: c++

        a.slice(3, 7);

    .. code:: shell-session

        Containers::ArrayView::slice(): slice [3:7] out of range for 4 elements

Of course, fixed-size slices on fixed-size array views are checked already at
compile time.

.. note-success::

    As `@zeuxcg rightfully pointed out on Twitter <https://twitter.com/zeuxcg/status/1097882097975386112>`_,
    not supplying an :cpp:`operator[]` doesn't really help debug performance
    for random access, since the :cpp:`operator T*` has to be called instead.
    Both are function calls and have the same overhead in debug builds. This
    also means array views *might* get checked :cpp:`operator[]` at some point
    as well, howewer it'll probably be opt-in to avoid assertion messages     getting inlined in every place where the function gets called.

`STL compatibility`_
--------------------

Continuing with how :dox:`Containers::Pointer`, :dox:`Containers::Reference`
and :dox:`Containers::Optional` recently became convertible from/to
:dox:`std::unique_ptr`, :dox:`std::reference_wrapper` and :dox:`std::optional`;
array views now expose a similar functionality. The :dox:`Containers::ArrayView`
can be *implicitly* created from a :dox:`std::vector` or an :dox:`std::array`
reference, plus :dox:`Containers::StaticArrayView` can be implicitly converted
from the (fixed-size) :dox:`std::array`. All you need to do is including the
:dox:`Corrade/Containers/ArrayViewStl.h </home/mosra/Code/corrade/src/Corrade/Containers/ArrayViewStl.h>`
header to get the conversion definitions. Similarly as mentioned in
`the previous article <{filename}lightweight-stl-compatible-unique-pointer.rst>`_,
it's a separate header to avoid unconditional heavy :cpp:`#include <vector>`
and :cpp:`#include <array>` being transitively present in all code that touches
array views. With that in place, you can do things like the following --- with
slicing properly bounds-checked, but no further overhead resulting from
iterator or element access:

.. code:: c++

    #include <Corrade/Containers/ArrayViewStl.h>

    …

    std::vector<float> data;

    float sum{}; // Sum of the first 100 elements
    for(float i: Containers::arrayView(data).prefix(100))
        sum += i;

In case you're feeling like using the standard C++2a |span| instead (or you
interface with a library using it), there's no need to worry either. A
compatibility with it is provided in
:dox:`Corrade/Containers/ArrayViewStlSpan.h </home/mosra/Code/corrade/src/Corrade/Containers/ArrayViewStlSpan.h>`.
As far as I'm aware, only libc++ ships an implementation of it at the moment.
For the span there's many more different conversion possibilities,
:dox:`see the docs <Containers-ArrayView-stl>` for more information. This
conversion is again separate from the rest because (at least the libc++) :cpp:`#include <span>` managed to gain almost twice the weight as both
:cpp:`#include <vector>` and :cpp:`#include <array>` together. I don't know
how's that possible for just a fancy pair of pointer and size with a handful of
one-liner member functions to be that big, but here we are.

`Array casting`_
----------------

When working with graphics data, you often end up with a non-descript "array of
bytes", coming from either some file format or being downloaded from the GPU.
Being able to reinterpret them as a concrete type is often very desired
and Magnum provides :dox:`Containers::arrayCast()` for that. Besides change of
type, it also properly recalculates the size to correspond to the new type.

.. code:: c++

    Containers::ArrayView<char> data;
    auto positions = Containers::arrayCast<Vector3>(data); // array of Vector3

Apart from the convenience, its main purpose is to direct the
:cpp:`reinterpret_cast<>` machine gun away from your feet. While it can't fully
stop it from firing, it'll check that both types are standard layout (so
without vtables and other funny business), that one type has its size a
multiple of the other and that the total byte size of the view doesn't change
after the cast. That allows you to do fancier things as well, such as
reinterpreting an array of :dox:`Matrix3` into an array of its column vectors:

.. code:: c++

    Containers::ArrayView<Matrix3> poses;
    auto baseVectors = Containers::arrayCast<Vector3>(poses);

Note that a cast of the ``poses`` to :dox:`Vector4` would not be permitted by
the checks above. Which is a good thing.

.. block-danger:: But, but... strict aliasing?!

    C++ purists may rightfully point out that doing the above is an undefined
    behavior, breaking strict aliasing rules. That's correct. What is also
    correct is that neither `std::mdspan`_ can be implemented cleanly without
    hitting any undefined behavior.

    The case of `std::mdspan`_ was apparently solved by abusing a
    `"legal loophole" <https://stackoverflow.com/questions/49665881/mdspan-and-the-strict-aliasing-rule>`_
    --- the sole presence of a type in standard library means there's no
    undefined behavior in its implementation. Moreover, standard library types
    don't have to be implementable outside of the standard library. I
    personally refuse to accept such status quo, so both the
    :dox:`Containers::arrayCast()` and the :dox:`Containers::StridedArrayView`
    *will stay* and I'll wait for the language
    `to fix itself <http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0593r2.html>`_
    instead.

`Type erasure`_
---------------

Complementary to the casting functionality, some APIs in Magnum accept array
views without requiring any particular type --- various GPU data upload
functions, image views and so on. Such APIs care only about the data pointer
and byte size. A :dox:`Containers::ArrayView<const void> <Containers::ArrayView>`
specialization is used for such case and to make it possible to pass in array
views of any type, it's implicitly convertible from them, with their size
getting recalculated to byte count.

Looking at |span|, it provides something similar through |as_bytes|, however
it's an explicit operation and is using the fancy new :cpp:`std::byte` type
(which, in my opinion, doesn't add anything useful over the similarly opaque :cpp:`void*`) --- and also, due to that, is *not* :cpp:`constexpr` (while the
Magnum array view type erasure *is*).

`Pointer-like semantics`_
-------------------------

Magnum's array views were deliberately chosen to have semantics similar to C
arrays --- they're implicitly convertible to its underlying pointer type
(which, again, allows us to optimize debug performance by not having to
explicitly provide :cpp:`operator[]`) and the usual pointer arithmetic works on
them as well. That allows them to be more easily used when interfacing with C
APIs, for example like below. The |span| doesn't expose any such functionality.

.. code:: c++

    Containers::ArrayView<const void> data;
    std::FILE* file;
    std::fwrite(data, 1, data.size(), file);

The pointer-like semantics means also that :cpp:`operator==` and other
comparison operators work the same way as on pointers. According to
cppreference at least, |span| doesn't provide any of these and since it doesn't
retain anything else from the pointer-like semantics, it's probably
`for the better <https://cor3ntin.github.io/posts/span/#span-operator>`_ ---
since |span| has neither really a pointer nor a container semantics, both
reasons for :cpp:`==` behavior like on a pointer or like on a container are
equally valid for either party and equally confusing for the other.

`Sized null views`_
-------------------

While this seemed like an ugly wart at first, I have to admit the whole API
became more consistent with such feature in place. It's about the possibility
to have a view on a :cpp:`nullptr`, but with a non-zero size attached. This
semantics is used, among other things, by a few OpenGL APIs, where passing a
null pointer together with a size will cause a buffer or texture to be
allocated but with contents uninitialized. To do this, it seemed more natural
to allow sized array views be created from :cpp:`nullptr` than to add dedicated
APIs for preallocation. The following will preallocate a GPU buffer to 384
bytes:

.. code:: c++

    GL::Buffer buffer;
    buffer.setData({nullptr, 32*3*sizeof(float)});

Later, when adding :dox:`Containers::StaticArrayView`, this feature allowed me
to provide it with an implicit constructor. When checking out |span|, I
discovered that implicit constructor of the fixed-size variant is not possible.

.. code:: c++

    Containers::StaticArrayView<16, float> a;   // {nullptr, 16}
    //std::span<float, 16> b;                   // doesn't compile :(

.. block-info:: Null views and boolean conversion

    With normal pointers, conversion to :cpp:`bool` returns :cpp:`false` if the
    pointer is :cpp:`nullptr` and :cpp:`true` if not. With views, and
    especially :cpp:`nullptr` views, the result of boolean conversion is less
    clear. While it's possible to enforce all null views to have a zero size
    (like |span| does), what about zero-sized non-null views? Since the view is empty, should boolean conversion really return :cpp:`true`?

    Currently, Magnum is following the pointer semantics, so :cpp:`false` is
    returned if and only if the pointer is :cpp:`nullptr`. That's mainly due to
    explicit boolean conversion operators being disabled on MSVC 2015, as
    otherwise they cause ambiguous overload with the implicit pointer
    conversion. As soon as it's possible to drop MSVC 2015 support, this may
    get revisited. Further details in :gh:`mosra/corrade#43`.

`Now, let's see those unforgiving numbers`_
===========================================

Below is the usual graph of preprocessed line count for each header, generated
using the following command with GCC 8.2. At the time of writing, libstdc++
doesn't ship with ``<span>`` yet, so it's excluded from the comparison. To
have more data, there comparison includes :cpp:`gsl::span` implementation from
Microsoft's :gh:`Guideline Support Library <Microsoft/GSL>` (version 2.0.0,
requiring at least C++14) and :cpp:`nostd::span` aka
:gh:`Span Lite <martinmoene/span-lite>` 0.4.0 from Martin Moene. As said
before, while preprocessed line count is not the only factor affecting compile
times, it correlates with it pretty well.

.. code:: sh

    echo "#include <vector>" | gcc -std=c++11 -P -E -x c++ - | wc -l

.. plot:: Preprocessed line count, GCC 8.2, C++11
    :type: barh
    :labels:
        <Containers/ArrayView.h>
        <vector>
        <array>
        <vector> + <array>
        <span>
        <gsl/span>
        <span.hpp>
    :labels_extra:
        ..
        ..
        ..
        ..
        N/A
        C++14
        ..
    :units: lines
    :values: 2451 8608 12029 15117 0 30715 17607
    :colors: success primary primary warning default danger warning

|span| ships in Clang's libc++ 7.0 (and thus I assume in Xcode 10.0 as well),
so here's a comparison using libc++. To make the comparison fair, it uses the
C++2a standard in all cases:

.. code:: sh

    echo "#include <span>" | clang++ -std=c++2a -stdlib=libc++ -P -E -x c++ - | wc -l

.. plot:: Preprocessed line count, Clang 7.0, libc++, C++2a
    :type: barh
    :labels:
        <Containers/ArrayView.h>
        <vector>
        <array>
        <vector> + <array>
        <span>
        <gsl/span>
        <span.hpp>
    :units: lines
    :values: 5954 28147 23632 28512 24098 24456 24178
    :colors: success danger danger danger danger danger danger

The Magnum implementation needs ``<type_traits>`` to do a bunch of SFINAE and
compile-time checks, ``<utility>`` is needed for the :dox:`std::forward()`
utility. While ``<utility>`` is comparatively easy to replace, I still don't
think writing my own type traits headers is worth the time investment, mainly
due to all the compiler magic that needs to be different for each platform.

.. block-success:: There's a light at the end of the tunnel --- but only if you refrain from using std::span

    One important thing to note --- to reduce compilation times even further,
    while forward declarations for all container classes in Magnum are
    available simply by including
    :dox:`Corrade/Containers/Containers.h </home/mosra/Code/corrade/src/Corrade/Containers/Containers.h>`,
    neither |span|, :cpp:`gsl::span` nor the Span Lite provide anything
    standardized like that, and due to the default template argument for the
    extent, you can't even provide the forward declaration yourself. So the
    cost of > 25k preprocessed lines is *omnipresent*.

    On the other hand, using :dox:`Containers::ArrayView` can help reduce
    compile times even in STL-based workflows --- for all functions that would
    take an :dox:`std::array` or :dox:`std::vector` by a :cpp:`const` reference
    (or a |span|), take an :dox:`Containers::ArrayView` instead. You'll save on
    the vector/array :cpp:`#include`\ s, and if you go even further and
    forward-declare the view, you can save those 2k lines as well.

`Compile times`_
----------------

To get some real timing, I composed a tiny "microbenchmark" shown below, with
equivalent variants for STL span, GSL span and span lite, using both GCC 8.2 in
C++11 mode and Clang 7.0 with libc++ in C++2a mode. Like in the previous
article, to balance the comparison, I'm switching to the standard assertions by
defining :dox:`CORRADE_STANDARD_ASSERT` and, for better sense of scale, there's
also a baseline time, which is from compiling just :cpp:`int main() {}` with no
:cpp:`#include` at all.

.. code:: c++

    #include <Corrade/Containers/ArrayView.h>

    using namespace Corrade;

    int main() {
        int data[]{1, 3, 42, 1337};

        auto a = Containers::arrayView(data);
        Containers::StaticArrayView<1, int> b = a.slice<1>(2);
        return b[0] - 42;
    }

.. code:: sh

    g++ main.cpp -DCORRADE_STANDARD_ASSERT -std=c++11                    # either
    clang++ main.cpp -DCORRADE_STANDARD_ASSERT -std=c++2a -stdlib=libc++ # or

.. Starting CompileTimeBenchmark with 9 test cases...
..  BENCH [1]  55.39 ± 2.47   ms baseline()@19x1 (wall time)
..  BENCH [2]  82.79 ± 6.78   ms arrayView()@19x1 (wall time)
..  BENCH [3] 336.48 ± 14.49  ms gslSpan()@4x1 (wall time)
..  BENCH [4] 196.33 ± 4.19   ms spanLite()@4x1 (wall time)
..  BENCH [5]  71.61 ± 3.28   ms clangBaseline()@19x1 (wall time)
..  BENCH [6] 127.44 ± 3.81   ms clangArrayView()@19x1 (wall time)
..  BENCH [8] 257.80 ± 6.56   ms clangSpan()@4x1 (wall time)
..  BENCH [7] 253.43 ± 3.73   ms clangGslSpan()@4x1 (wall time)
..  BENCH [9] 248.97 ± 5.23   ms clangSpanLite()@4x1 (wall time)
.. Finished CompileTimeBenchmark with 0 errors out of 105 checks.

.. plot:: Compilation time, GCC 8.2, C++11
    :type: barh
    :labels:
        baseline
        Containers::ArrayView
        std::span
        gsl::span
        nonstd::span
    :labels_extra:
        int main() {}
        ..
        N/A
        C++14
        ..
    :units: ms
    :values: 55.39 82.79 0 336.48 196.33
    :errors: 2.47 6.78 0 14.49 4.19
    :colors: default success default danger warning

.. plot:: Compilation time, Clang 7.0, libc++, C++2a
    :type: barh
    :labels:
        baseline
        Containers::ArrayView
        std::span
        gsl::span
        nonstd::span
    :units: ms
    :values: 71.61 127.44 257.80 253.43 248.97
    :errors: 3.28 3.81 6.56 3.73 5.23
    :colors: default success danger danger danger

.. compare slicing of a vector (Array?) with slicing of it through STL ranges for extra fun

.. note-info::

    I'm not doing any comparison with C++ modules this time. See the
    `previous article <{filename}lightweight-stl-compatible-unique-pointer.rst#but-but-modules>`_
    for a detailed take on the topic.

`Debug performance`_
--------------------

Looking at `the size of assembly output <https://gcc.godbolt.org/z/9Egkeg>`_
for an unoptimized version of the snippet above, the Magnum implementation is
1/3 smaller than equivalent
`code written with Span Lite <https://gcc.godbolt.org/z/W2OOFU>`_ and about
*three times* smaller `than the same using GSL span <https://gcc.godbolt.org/z/GDdRC0>`_. In all cases the compiler is able to optimize everything away at
``-O1``. Unfortunately Compiler Explorer doesn't have an option to use libc++,
so couldn't make a comparison with |span| there.

`The baby steps (and falls) of std::span`_
==========================================

If you survived all the way down here without abruptly leaving with an
irresistible urge to :s:`rewrite everything in Rust` become a barista instead,
you'd think it stops just at awful compile times. Well, no. It's worse than
that.

`Hot take: implicit all-catching constructors are stupid`_
----------------------------------------------------------

I discovered the first issue when writing the STL compatibility conversions.
All Magnum containers and math types have a special constructor and a
conversion operator that makes it possible to convert a type either explicitly
or --- if the type is simple enough, conversion not costly and there are no
risks of causing ambiguous operator overloads --- implicitly from and to a
third-party type. This way Magnum supports seamless usage its math types with
:dox:`GLM </home/mosra/Code/magnum-integration/src/Magnum/GlmIntegration/Integration.h>`,
:dox:`Bullet Physics </home/mosra/Code/magnum-integration/src/Magnum/BulletIntegration/Integration.h>`,
:dox:`Vulkan types </home/mosra/Code/magnum/src/Magnum/Vk/Integration.h>`
or, for example, :dox:`Dear ImGui </home/mosra/Code/magnum-integration/src/Magnum/ImGuiIntegration/Integration.h>`.

This works well and causes no problem as long as the third-party type doesn't
have a constructor that accepts anything you throw at it. I ran into this issue
two weeks ago with `Eigen <http://eigen.tuxfamily.org/>`_, as both its
``Array`` and ``Matrix`` classes have
`such a constructor <https://github.com/eigenteam/eigen-git-mirror/blob/28728b910ed1e280aad4a4c9c46ef4ae2dddccc7/Eigen/src/Core/Array.h#L165-L172>`_.
But in that case it's not harmful, only annoying, as the conversion can no
longer be done directly through an explicit conversion but rather using some
conversion function.

In case of |span|, it's much worse --- there's an all-catching constructor
taking any container-like type. It's a well meant feature, however, it works
even in the case of a fixed-size span --- and there it gets dangerous, as shown
below. And this is not just a cause of an implementation issue in libc++, it's
*designed this way* in
`the standard itself <https://en.cppreference.com/w/cpp/container/span/span>`_
--- of all things (exceptions, asserts, compile-time errors), it chooses the
worst --- such conversion is declared as undefined behavior.  Fortunately,
the `good people of Twitter <https://twitter.com/tristanbrindle/status/1095809866206248962>`_
already recognized this as a defect and are working on a solution. Hopefully
the fix gets in *together* with the span and not tree years later or something.

.. code:: c++

    #include <span>

    struct Vec3 { // your usual Vec3 class
        size_t size() const { return 3; }
        float* data() { return _data; }
        const float* data() const { return _data; }

        private: float _data[3]{};
    };

    int main() {
        Vec3 a;
        std::span<float, 57> b = a; // this compiles?!?!
    }

`Implicit conversion from std::initializer_list is actively harfmul`_
---------------------------------------------------------------------

Some time ago there was a `Twitter discussion <https://twitter.com/hugoamnov/status/1088817207029698563>`_ where it was suggested to add a constructor
taking :dox:`std::initializer_list` to an array view class. I wondered why
Magnum's :dox:`Containers::ArrayView` class doesn't have such an useful feature
... until I remembered why. Consider this innocent-looking snippet, guess what
happens when you access :cpp:`b[0]` later? If you don't know, try again with
``-fsanitize=address``.

.. code:: c++

    std::span<const std::string> b{{"hello", "there"}};
    b[0]; // ?

Thing is, the above-mentioned all-catching constructor can capture an
:dox:`std::initializer_list` as well, however the problem (compared to, let's
say, doing the same with a :dox:`std::vector`), is that it gets constructed
implicitly --- and so it's very hard to realize the initializer list elements
are already destroyed after the semicolon.

In case of Magnum, rather than having array views implicitly constructible from
:dox:`std::initializer_list`, where it makes sense, APIs taking an array view
have also an initializer list overload. It makes the API surface larger, but
that's a reasonable price to pay for array views being safer to use.

`Single-header implementation`_
===============================

The :gh:`Magnum Singles <mosra/magnum-singles>` repository introduced
previously got a new neighbor --- all the array view classes, in a tiny,
self-contained, dependency-less and fast-to-compile header file, meant to be
bundled right into your project:

.. |n| replace:: :label-success:`new`

.. class:: m-table m-fullwidth

========================= === ================ ================================
Library                   LoC Preprocessed LoC Description
========================= === ================ ================================
`CorradeArrayView.h`_ |n| 558 2453             See :dox:`Containers::ArrayView`
                                               and :dox:`StaticArrayView <Containers::StaticArrayView>`
                                               docs
`CorradeOptional.h`_      328 2742             See :dox:`Containers::Optional`
                                               docs
`CorradePointer.h`_       259 2321             See :dox:`Containers::Pointer`
                                               docs
`CorradeReference.h`_     115 1639             See :dox:`Containers::Reference`
                                               docs
`CorradeScopeGuard.h`_    131 34               See :dox:`Containers::ScopeGuard`
                                               docs
========================= === ================ ================================

Funny thing is, even though the :dox:`Containers::ArrayView` API is much larger
than of :dox:`Containers::Optional`, it still boils down to less code after
preprocessing --- reason is simply that the ``<new>`` include was not needed,
since array views don't do any fancy allocations.

.. transition:: * * *

Questions? Complaints? Share your opinion on social networks:

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/1097584346737922048>`_,
    `Reddit r/cpp <https://www.reddit.com/r/cpp/comments/as1a2f/array_view_implementations_in_magnum_and_the_c20/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=19193849>`_

.. _std::mdspan: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html

.. _CorradeArrayView.h: https://github.com/mosra/magnum-singles/blob/master/CorradeArrayView.h
.. _CorradeOptional.h: https://github.com/mosra/magnum-singles/blob/master/CorradeOptional.h
.. _CorradePointer.h: https://github.com/mosra/magnum-singles/blob/master/CorradePointer.h
.. _CorradeReference.h: https://github.com/mosra/magnum-singles/blob/master/CorradeReference.h
.. _CorradeScopeGuard.h: https://github.com/mosra/magnum-singles/blob/master/CorradeScopeGuard.h
