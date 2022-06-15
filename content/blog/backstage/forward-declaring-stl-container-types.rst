Forward-declaring STL container types
#####################################

:date: 2019-03-28
:modified: 2022-06-15
:category: Backstage
:tags: C++, singles
:summary: Some time ago, when looking into different ways how to reduce STL
    header bloat, I discovered that libc++'s ``<iosfwd>`` has a forward
    declaration for :dox:`std::vector`. Turns out, there are more of them ---
    and not just on one particular standard library implementation.

.. role:: cpp(code)
    :language: c++
.. role:: link-flat(link)
    :class: m-flat

.. TODO: remove this once the STL tag file has it
.. |span| replace:: :link-flat:`std::span <https://en.cppreference.com/w/cpp/container/span>`
.. |wink| replace:: 😉

.. note-success:: Content care: Jun 15, 2022

    Updated for GCC 11, Clang 13 and MSVC 2022. Slightly larger line counts for
    about everything, except for :dox:`std::array`, :dox:`std::tuple`,
    :dox:`std::set` and other associative containers that are each smaller by
    several thousand lines. Unfortunately I didn't discover any new forward
    declarations on either, on the other hand one :dox:`std::tuple` forward
    declaration got removed in GCC 12, so a different header is needed.

Using forward declarations to avoid the need to :cpp:`#include` the whole
type definition is one of the essential techniques to counter the
ever-increasing compile times. While it's very common to do for some libraries
(such as Qt), other libraries make it harder and some straight out impossible.
If you want to learn more about forward declarations, check out
`this archived post from 2013 <{filename}reducing-cpp-compilation-time-in-magnum-code-optimizations.rst>`_.

The C++ standard explicitly forbids forward-declaring types from the standard
library on the user side, turning it into an undefined behavior. Moreover, the
STL namespace usually wraps another :cpp:`inline` namespace, which is defined
to some platform-specific string like :cpp:`std::__1` on libc++ or
:cpp:`std::__ndk1` on Android, which makes it harder (but not impossible) to
correctly match the type declaration on the other side.

The *nuclear* solution to this impossbility-to-forward-declare is to stop using
STL altogether. Many performance-oriented projects ended up going that way,
but, even though Magnum is gradually providing alternatives to
`more <{filename}lightweight-stl-compatible-unique-pointer.rst>`__
and `more <array-view-implementations.rst>`__ STL types, I don't want to
completely alienate the users but rather give them a choice and a chance to use
Magnum with STL seamlessly.

A potential solution that *isn't* an undefined behavior could be extending the
standard library implementation itself with forward declarations for additional
types. Forward declarations for stream types are in the classic
:dox:`<iosfwd> <iosfwd>` header, but that's about it for what the standard
guarantees. And adding forward declarations for other types isn't as
straightforward as it may seem.

`The problem with templates`_
=============================

Most container types in the STL take optional template parameters for an
allocator and other things. For example, the full set of things needed for
defining the :dox:`std::string` type looks like this:

.. code:: c++

    // <string>

    namespace std {
        template<class> class allocator;
        template<class> class char_traits;

        template<class CharT,
            class Traits = char_traits<CharT>,
            class Allocator = allocator<CharT>
        > class basic_string {
            …
        };

        typedef basic_string<char> string;
        …
    }

As with functions, the standard mandates that the default value is specified
only once --- either on a (forward) declaration or on the definition. So a way
to forward-declare these is putting the default template parameter on a forward
declaration and then have the definition without. This is by the way the main
reason Magnum provides forward declaration headers such as
`Corrade/Containers/Containers.h <https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Containers.h>`_
instead of suggesting people to write the forward declarations on their own
(it's also much easier and less error-prone when the type is a long :cpp:`typedef`
chain).

For the :dox:`std::string` case above, it would mean that the definition has to
be split into two parts --- for example, with ``<iosfwd>`` containing the
forward declaration and all related :cpp:`typedef`\ s, and ``<string>`` just
the definition. For the actual definition we have to include the forward
declaration as well in order to get values of the default parameters.

.. container:: m-row

    .. container:: m-col-m-6 m-nopadt

        .. code:: c++
            :class: m-inverted
            :hl_lines: 1 7 8 9 10

            // <iosfwd>

            namespace std {
                template<class> class allocator;
                template<class> class char_traits;

                template<class CharT,
                    class = char_traits<CharT>,
                    class = allocator<CharT>
                > class basic_string;

                typedef basic_string<char> string;
                …
            }

    .. container:: m-col-m-6 m-nopadt

        .. code:: c++
            :class: m-inverted
            :hl_lines: 1 3 6 7 8 9

            // <string>

            #include <iosfwd>

            namespace std {
                template<class CharT,
                    class Traits,
                    class Allocator
                > class basic_string {
                    …
                };

                …
            }

So, basically, if we're able to find a forward declaration of a STL container
type *including the default arguments* in some (even internal) STL header, the
header is :abbr:`hermetic <can be included on its own>` and significantly
smaller than the corresponding standard header, we won. Conversely, if the type definition contains the template default parameters, then we can be sure that
no forward declaration is possible.

`Detecting what STL we're on`_
==============================

Because we now wandered into the implementation-specific territory, we need a
way to detect what STL flavor is the code being compiled on. Then, for known
STL implementations with known forward declaration headers we include the
particular header, and use the full type definition in the standard header
otherwise. That means our forward declaration wrapper will *always* work,
but giving us compilation time advantages in *some* cases.

The classic way to detect a STL vendor is to include the :dox:`<ciso646> <ciso646>`
header (which is defined to be empty on C++) and then check for either
:cpp:`_LIBCPP_VERSION` (defined on libc++, used by Clang mainly on macOS and
iOS), :cpp:`_CPPLIB_VER` (defined on MSVC STL, formerly Dinkumware) or
:cpp:`__GLIBCXX__` (defined on GCC's libstdc++). One thing to note is that on
GCC before version 6.1 the ``<ciso646>`` header doesn't define the
:cpp:`_LIBCPP_VERSION` macro, so it's needed to get it via some other means.
Beginning with C++20, there will be a new header,
:link-flat:`<version> <https://en.cppreference.com/w/cpp/header/version>`,
standardizing this process.

If you use Corrade and include any of its headers, you'll get the detected
library exposed through one of the :dox:`CORRADE_TARGET_LIBCXX`, :dox:`CORRADE_TARGET_LIBSTDCXX` or :dox:`CORRADE_TARGET_DINKUMWARE` macros.

`What you can expect`_
======================

The following table summarizes what common STL container types can be
forward-declared on which implementation. The left column shows preprocessed
line count with GNU libstdc++ and C++11 (unless said otherwise), gathered using
the following command line:

.. code:: sh

    echo "#include <utility>" | gcc -std=c++11 -P -E -x c++ - | wc -l

The other columns then show how many preprocessed lines is the corresponding
forward declaration on a particular implementation, if applicable.

.. container:: m-hide-s

    .. this is to mirror the footnote references in the table, otherwise the
        [16]_ etc below will conflict with the IDs. The backlinks won't work
        either but what can I do. TODO: is there some better solution for this?

    [1]_ [2]_ [3]_ [4]_ [5]_ [6]_ [7]_ [8]_ [9]_ [10]_ [11]_ [12]_ [13]_ [14]_

.. container:: m-scroll

    .. raw:: html
        :file: forward-declaring-stl-container-types-support.html

.. [1] Some STL implementations have a forward declaration for :dox:`std::pair`,
    but you'll need ``<utility>`` in most cases anyway for the
    :link-flat:`std::move() <https://en.cppreference.com/w/cpp/utility/move>`,
    :dox:`std::forward()` and other utils there's no point in bothering with a
    forward declaration --- which wouldn't be much smaller than the full
    definition anyway. Similarly it goes for :dox:`std::initializer_list`, the
    full definition is also very tiny. Both these types don't have any standard
    default template argument so these could be *theoretically* safe to
    forward-declare however nothing prevents STL implementations from adding
    their own default template arguments.
.. [2] Both :dox:`std::list` and :dox:`std::forward_list` have a full
    definition in the standard ``<list>`` / ``<forward_list>`` headers on all
    three implementations. The libc++ implementation has a forward declaration
    in the same file, but it would first need to be extracted outside to make
    it useful.
.. [3] libstdc++ has :dox:`std::string`, :dox:`std::wstring`,
    :dox:`std::u16string` and :dox:`std::u32string` forward-declared in
    `<bits/stringfwd.h> <https://github.com/gcc-mirror/gcc/blob/2a4787da69071b5d5bc178795accca264073b7e4/libstdc%2B%2B-v3/include/bits/stringfwd.h#L68-L73>`_.
.. [4] libc++ has :dox:`std::string` and :dox:`std::wstring` forward-declared
    in the standard `<iosfwd> <https://github.com/llvm-mirror/libcxx/blob/8c58c2293739d3d090c721827e4217c113ced89f/include/iosfwd#L190-L195>`__,
    unfortunately the :dox:`std::u16string` and :dox:`std::u32string` typedefs
    are missing.
.. [5] MSVC STL has the full definition of :dox:`std::basic_string` including
    default parameters in ``<xstring>``, which makes it impossible to
    forward-declare.
.. [6] :dox:`std::basic_string_view` has the full definition directly in the
    ``<string_view>`` header on both libc++ and libstdc++, MSVC 2017 has both
    a forward declaration and the full definition in ``<xstring>`` --- the
    declaration could be extracted to a separate header to make this work.
.. [7] I couldn't find any forward declaration for :dox:`std::array` in
    libstdc++. It however doesn't mean it hasn't any --- the type has no
    default template parameters so it should be possible.
.. [8] libc++ has a forward declaration for :dox:`std::array` and
    :dox:`std::tuple` in `<__tuple> <https://github.com/llvm-mirror/libcxx/blob/73d2eccc78ac83d5947243c4d26a53f668b4f432/include/__tuple#L163>`_.
.. [9] MSVC STL has a forward declaration for :dox:`std::array` and
    :dox:`std::tuple` defined in the standard ``<utility>``, next to
    :dox:`std::pair`.
.. [10] libstdc++ from version 7 to 11 has a forward declaration for
    :dox:`std::tuple` in the standard `<type_traits> <https://github.com/gcc-mirror/gcc/blob/releases/gcc-7.1.0/libstdc++-v3/include/std/type_traits#L2557-L2558>`_
    header, but it got removed in GCC 12. The other forward declaration, added
    in 4.6 and available also in GCC 12, is in
    `<bits/stl_pair.h> <https://github.com/gcc-mirror/gcc/blob/releases/gcc-12.1.0/libstdc++-v3/include/bits/stl_pair.h#L89>`_.
    To avoid issues, include the whole ``<utility>`` --- it isn't that much
    larger than ``<type_traits>``.
.. [11] MSVC has the full :dox:`std::vector` definition in ``<vector>``,
    libstdc++ has a small-ish full definition in ``<bits/stl_vector.h>`` but
    the header is not hermetic and when all needed dependencies are included as
    well the size is not much different from the standard header.
.. [12] libc++ 3.9 and up has a forward declaration for :dox:`std::vector` in
    the standard `<iosfwd> <https://github.com/llvm-mirror/libcxx/blob/8c58c2293739d3d090c721827e4217c113ced89f/include/iosfwd#L199-L200>`__ (older
    versions don't).
.. [13] libc++ 7.0 has a :cpp:`friend` forward declaration for |span|
    in the `<iterator> <https://github.com/llvm-mirror/libcxx/blob/73d2eccc78ac83d5947243c4d26a53f668b4f432/include/iterator#L1429>`_
    header but that's *not* enough to have the forward declaration available
    globally in the :cpp:`std` namespace. Too bad, because the ``<span>``
    header is *heavy*.
.. [14] All standard (unordered) (multi)map/set implementations have just the
    full definition with no possibility to forward-declare. Since these types
    are very rarely used directly as function parameters or return types, it's
    not such a big problem. Besides that, they tend to be rather heavy both at
    compile time and at runtime due to their extreme genericity, so the less
    they get used the better |wink|

`Conclusion`_
=============

While the heavier map / set types don't have forward declarations, the existing
forward declarations can already cover many use cases for libraries that want
to be both fast-to-compile and STL-friendly:

-   a STL-friendly function overload returning a :dox:`std::string` instead of
    a custom lightweight string type to avoid further copies when passing the
    string to STL-oriented APIs (used for example by
    :dox:`Utility::Directory::read()` vs :dox:`Utility::Directory::readString()`)
-   a function overload which takes a :dox:`std::vector` by mutable reference
    (instead of e.g. a reference to :dox:`Containers::Array`) in order to fill
    it, to avoid unnecessary copies when a :dox:`std::vector` is needed further
-   functions taking and returning :dox:`std::array`\ s / :dox:`std::tuple`\ s
    *could* now reside in otherwise STL-free headers instead of being pushed
    out to avoid the 13k included lines

Too bad that the :dox:`std::string_view` and |span| types, while meant to be
lightweight at runtime, are so compile-time heavy and impossible to
forward-declare.

While it's possible to make use of the forward declarations also for functions
taking/returning vectors, arrays and strings by constant references, a better
approach is to make use of STL compatibility in the :dox:`Containers::ArrayView`
classes --- that'll allow more types than just :dox:`std::vector`,
:dox:`std::array` or :dox:`std::string` to be used and the compile-time impact
is clear and consistent with this type. See the article about
`STL-compatible array views <{filename}array-view-implementations.rst>`_ for
more information and detailed performance overview.

I also didn't look for forward declarations of the :dox:`std::unique_ptr`,
:dox:`std::optional` or :dox:`std::reference_wrapper` types because they're
very simple and thus easy to replace. See the `Lightweight but still STL-compatible unique pointer <{filename}lightweight-stl-compatible-unique-pointer.rst>`_
post for more information.

`Try them in your code`_
========================

Corrade provides the above forward declarations in tiny
:dox:`Corrade/Utility/StlForwardArray.h </home/mosra/Code/corrade/src/Corrade/Utility/StlForwardArray.h>`,
:dox:`Corrade/Utility/StlForwardString.h </home/mosra/Code/corrade/src/Corrade/Utility/StlForwardString.h>`,
:dox:`Corrade/Utility/StlForwardTuple.h </home/mosra/Code/corrade/src/Corrade/Utility/StlForwardTuple.h>` and
:dox:`Corrade/Utility/StlForwardVector.h </home/mosra/Code/corrade/src/Corrade/Utility/StlForwardVector.h>`
headers, simply include them instead of the standard ``<array>``, ``<string>``,
``<tuple>`` or ``<vector>`` headers where desired. The engine is moving to use
them as well, but since it will cause many build errors due to suddenly missing :cpp:`#include`\ s in user code, the move is done gradually to avoid suddenly
breaking *everything*.

The :gh:`Magnum Singles <mosra/magnum-singles>` repository contains these as
well, as a hermeticheaders not depending on Corrade's internals. Take them and
bundle right into your project:

.. class:: m-table m-fullwidth

============================ === ================ =============================
Library                      LoC Preprocessed LoC Description
============================ === ================ =============================
`CorradeStlForwardArray.h`_  67  2455\ [16]_      See :dox:`Corrade/Utility/StlForwardArray.h </home/mosra/Code/corrade/src/Corrade/Utility/StlForwardArray.h>`
                                                  docs
`CorradeStlForwardString.h`_ 74  48               See :dox:`Corrade/Utility/StlForwardString.h </home/mosra/Code/corrade/src/Corrade/Utility/StlForwardString.h>`
                                                  docs
`CorradeStlForwardTuple.h`_  78  1616             See :dox:`Corrade/Utility/StlForwardTuple.h </home/mosra/Code/corrade/src/Corrade/Utility/StlForwardTuple.h>`
                                                  docs
`CorradeStlForwardVector.h`_ 62  741\ [16]_       See :dox:`Corrade/Utility/StlForwardVector.h </home/mosra/Code/corrade/src/Corrade/Utility/StlForwardVector.h>`
                                                  docs
============================ === ================ =============================

.. [16] gathered using Clang 7.0 and libc++, since GCC 8.2's libstdc++ doesn't
    have a forward declaration for :dox:`std::array` / :dox:`std::vector`

`Can we convince vendors to do this more?`_
===========================================

While I think it's possible to add additional forward declarations to *some*
STL implementations, it might not be always possible to do so without breaking
ABI compatibility --- even in Magnum I had to break ABI compatibility a few
times in the past in order to achieve that (and now I know what to avoid to
make new types easily forward-declarable from the start).

The ideal way would be to have the forward declarations guaranteed by the
standard (extending ``<iosfwd>`` further, for example) so we don't need to
include platform-specific internal "bits", but again this may cause an ABI
break for many vendors and thus take years to implement (like it happened with
:dox:`std::string` in C++11 where libstdc++ could no longer have it
copy-on-write --- and the problems it caused
`persist until today <https://github.com/dartsim/dart/issues/967>`_).

There's also a slight chance that, due to complexities of :dox:`std::allocator`
and other types used in default template arguments, adding forward declarations
to ``<iosfwd>`` would make it no longer lightweight. This really depends on how
the implementations are done and what all needs to be known to forward-declare
given type.

.. note-dim::

    Questions? Complaints? Share your opinion on social networks: `Twitter <https://twitter.com/czmosra/status/1111298171764989953>`_,
    `Reddit r/cpp <https://www.reddit.com/r/cpp/comments/b6kov5/forwarddeclaring_stl_container_types/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=19513594>`_

.. _CorradeStlForwardArray.h: https://github.com/mosra/magnum-singles/blob/master/CorradeStlForwardArray.h
.. _CorradeStlForwardString.h: https://github.com/mosra/magnum-singles/blob/master/CorradeStlForwardString.h
.. _CorradeStlForwardTuple.h: https://github.com/mosra/magnum-singles/blob/master/CorradeStlForwardTuple.h
.. _CorradeStlForwardVector.h: https://github.com/mosra/magnum-singles/blob/master/CorradeStlForwardVector.h
