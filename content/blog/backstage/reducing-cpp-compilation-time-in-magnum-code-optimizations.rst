Reducing C++ compilation time in Magnum: code optimizations
###########################################################

:date: 2013-12-11 13:43
:modified: 2018-01-03
:archived: True
:category: Backstage
:tags: C++, CMake, Corrade, OpenGL, Qt
:summary: Large C++ projects often suffer with very long times for both full
    and incremental compilation, severely hurting productivity. The following
    series will overview some techniques employed in Magnum to make iteration
    times smaller, this article focuses on code-based optimizations.

.. role:: cpp(code)
    :language: c++
.. role:: hl
    :class: m-text m-strong m-danger

.. note-success:: Content care: May 06, 2018

    Code snippets were updated to match current state of the Magnum API and
    some typos and grammar errors were fixed. The measured numbers are kept on
    their 2013 values.

To put things into perspective, the Magnum graphics engine has around 100k
lines of templated C++ code, documentation and comments. Currently the
unoptimized build with Clang, CMake and Ninja runs 2 minutes and 59 seconds
with tests enabled, without tests it's just 76 seconds. Times mentioned in the
article were measured in various stages of development, thus they may not
always reflect the current state.

`The preprocessor issue`_
=========================

C preprocessor is the predecessor of all module systems and it's showing its
age. Simply put, instead of providing the compiler with only essential module
information, it just concatenates all the required code into one big file and
passes it to the compiler. This isn't much of an issue with C-based projects
where the headers are small, but with C++'s templates we need to put much more
code into the header. When you include *the right* STL headers and *proper*
OpenGL headers, the preprocessed source can have well over 100k lines, which
then takes significant amount of time to parse. Usually it doesn't matter
whether the code is spread over one or one hundred files, as any sane system
should already have all the headers in disk cache.

To solve this and many other preprocessor issues, Clang developers are working
on a module system, but it is not usable for C++ yet.

`Discovering problematic includes`_
-----------------------------------

If you are using CMake with Makefile generator for your project, you can use it
to generate just the preprocessed file so you can examine preprocessed line
count for each source file --- just append ``.i`` to name of source file. Then
you can try removing some :cpp:`#include`\ s to bisect the big ones.

.. code:: shell-session

    [build/src/Magnum/GL]$ make Framebuffer.cpp.i
    Preprocessing CXX source to CMakeFiles/MagnumGLObjects.dir/Framebuffer.cpp.i
    [build/src/Magnum/GL]$ wc -l CMakeFiles/MagnumGLObjects.dir/Framebuffer.cpp.i
    35695 CMakeFiles/MagnumGLObjects.dir/Framebuffer.cpp.i

`Reducing includes in headers`_
-------------------------------

The first and obvious trick is to remove :cpp:`#include`\ s which are not
needed anymore. This is boring and time-consuming task if done by hand, but
helps a lot without even touching the actual code. There are also some
semi-automated tools for this, the simplest and dumbest brute-force method is
removing :cpp:`#include`\ s as long as the code can still compile.

If given type is not used in the header, you can use forward declarations and
move the :cpp:`#include`\ s from header to implementation file. Having the big
:cpp:`#include` in one ``*.cpp`` file as opposed to having it in ``*.h`` which
is included in 150 other files helps a lot. If given type is used only in some
non-templated function, you can move its definition into source file. The only
problem remains when the type is used as class member or in templated function.

Forward-declaring non-templated classes and structs is trivial (and very
common, for example with Qt). It gets more complicated when :cpp:`namespace`\ s
and :cpp:`typedef`\ s are involved, with template classes you need to repeat
the whole template list and it can get quicky out of hand:

.. code:: c++

    class Timeline; // easy

    namespace Math { template<std::size_t, class> class Matrix; }
    typedef float Float;
    typedef Math::Matrix<3, Float> Matrix3x3; // ehh...

For user convenience Magnum has :dox:`forward declaration headers <compilation-forward-declarations>`,
which are available for each namespace, so the users can just include this tiny
header and don't need to write forward declarations on their own:

.. code:: c++

    // forward-declares both Timeline and Matrix3x3
    #include <Magnum/Magnum.h>

The problem is when you want to forward-declare class with default template
arguments. Similarly to default arguments in functions, in C++ you can't repeat
the default argument when defining the type. As we already have forward
declaration header, we can put the default arguments in that header and omit
them in the actual definition. The type definition must be complete, so the
forward declaration header must be included in the type definition header.

.. code:: c++

    // SceneGraph.h
    template<UnsignedInt, class T, class TranslationType = T> class TranslationTransformation;

.. code:: c++

    // TranslationTransformation.h
    #include "SceneGraph.h"

    template<UnsignedInt dimensions, class T, class TranslationType> class TranslationTransformation {
        // ...
    };

With C++11 it is also possible to forward-declare typed enums. In Magnum some
enums are used on many places (:dox:`MeshPrimitive`, :dox:`GL::BufferUsage`
...) and some are very large (:dox:`PixelFormat`, :dox:`GL::TextureFormat`,
...) and the enum values often depend on OpenGL headers which are also big. The
compiler doesn't care about particular named values and needs to know only the
type, thus you can pass the value around without having full definition of the
enum around:

.. code:: c++

    // forward-declares ColorFormat enum
    #include <Magnum/Magnum.h>

    // Don't need the header here
    PixelFormat format = image.format();

.. code:: c++

    // Need it here
    #include <Magnum/PixelFormat.h>

    format = PixelFormat::RGBA8Unorm;

Note that in C++ it is not possible to forward declare class members. To reduce
header dependencies I had to extract some widely-used enums from their classes
(thus :cpp:`GL::Buffer::Usage` is now :dox:`GL::BufferUsage` etc.), but the
change resulted in improved compilation times of code where the enum
forward-declaration is enough.

`STL includes`_
---------------

The Standard C++ library is a problem on its own. It is notorious for its heavy
headers, which got even bigger with C++11. The STL types are heavily templated
with default template arguments and implementation-defined template parameters,
thus, as said above, it's impossible to work around the issue and create our
own forward declaration headers.

The table below lists preprocessed line count of the largest STL headers. It
was generated from GCC's libstdc++ 4.8.2 and Clang's libc++ 3.3 with the
following command, headers which didn't exceed 25k lines were omitted. In
comparison, whole ``<cmath>`` has just below 3k lines and ``<vector>`` is
merely 11k lines in C++11 libstdc++.

.. code:: sh

    echo "#include <iostream>" | g++ -std=c++11 -E -x c++ - | wc -l

.. class:: m-table m-center-t

=================== =============== =============== ============
Header              C++03 libstdc++ C++11 libstdc++ C++11 libc++
=================== =============== =============== ============
``<forward_list>``                  25927           18095
``<queue>``         8749            13830           26309
``<algorithm>``     9801            :hl:`46279`     16645
``<complex>``       21160           28312           :hl:`44507`
``<valarray>``      14671           :hl:`49630`     24949
``<random>``                        :hl:`36180`     :hl:`51187`
``<ios>``           15442           21561           29202
``<*stream>``       ~18000          ~24000          :hl:`~41000`
``<iomanip>``       11504           24296           :hl:`40545`
``<streambuf>``     11839           17946           29652
``<locale>``        17913           24027           :hl:`35188`
``<codecvt>``                       n/a             28922
``<regex>``                         :hl:`70409`     :hl:`41601`
``<thread>``                        27436           17155
``<future>``                        32254           19618
=================== =============== =============== ============

Note how the line count varies wildly between GCC's libstdc++ and Clang's
libc++. The numbers aren't exactly absolute, as many headers share common code,
but the main offenders are the various ``<*stream>`` headers and various
algorithm headers. Fortunately for ``<*stream>`` there is a forward-declaration
header ``<iosfwd>`` which was created in some older revision of C++ for exactly
this purpose, because the stream implementation was far bigger than the other
headers. The situation changed with C++11, but sadly there were no more
forward-declaration headers added. The various container classes are around
10-20k lines and thus can be used as class members without much impact on
compilation time, but the other :cpp:`#include`\ s shouldn't appear in headers
at all.

Removing all usage of ``<algorithm>`` from Magnum header files resulted in
significant compile time reductions (4:30 before, 4:10 affer), removing stream
usage or replacing all ``<*stream>`` occurences with ``<iosfwd>`` resulted in
another 20 seconds saved.

Another solution is not to use STL at all and implement everything from
scratch. It's then possible to achieve
`very impressive compilation times <https://twitter.com/EricLengyel/status/380590862578614272>`_,
but the resources required to implement the equivalent of C++11 STL
functionality are just too large.

`Other heavy includes`_
-----------------------

The Boost library is also known for its header size, but it is not used in
Magnum (and C++11 incorporates many useful things from this library so the need
for it is even smaller). The other heavy thing are OpenGL headers. Originally
Magnum used GLEW for OpenGL extension handling, but GLEW headers have about 18k
lines and contain many functions the engine will never use. Recently I switched
to ``glLoadGen``, which generates the header with only requested functions. The
generated header has about 3k lines (which is roughly the size of official
``gl.h``) and compilation time was reduced from 5:00 to 4:45.

`Reducing includes needed for class members`_
---------------------------------------------

If you have some value type as class member, you need to :cpp:`#include`, so
the compiler can know its size and can generate proper constructor, assignment
operator and destructor. You can circumvent this by making it a reference or
pointer and then explicitly define the constructor and other functions in
source file. The D-Pointer approach, which is very heavily used in Qt, is
another solution for this and many other issues, however the additional heap
allocation and indirection has performance implications and thus is not used in
Magnum.

`Reducing templated code in headers`_
-------------------------------------

If the templated code is used for limited set of types (e.g. only
:cpp:`float`\ s and :cpp:`double`\ s), you can move the definition into source
file and explicitly instantiate the template for each type. This approach is
used in Magnum's scene graph. Additonally Magnum provides special
:dox:`template implementation header <compilation-speedup-hpp>` for each class,
which contains the definitions of templated functions. If the users want to use
the template for e.g. :cpp:`int`\ s (which isn't provided by default), they can
include this header in some source file and do the explicit instantiation
themselves:

.. code:: c++

    // instantiation.cpp
    #include "SceneGraph/AbstractObject.hpp"

    template class SceneGraph::AbstractBasicObject2D<Int>;

`Balancing size and count of compilation units`_
------------------------------------------------

For headers it's often good to split the header into smaller ones with less
dependencies, but for source files it's better to combine more of them into
one, as the compiler then needs to preprocess the included headers only once
instead of more times. Be aware that this is double-edged sword and it will
hurt iteration times --- recompiling the whole huge file after a small change
would take much longer than rebuilding only a small one. Also the compile time
reduction is not as significant as when optimizing a widely-used header file.
Magnum uses this approach for template instantiation files, the merging
resulted in 5 seconds shorter build time.

`Reducing amount of generated code`_
====================================

C++11 :cpp:`extern template` keyword tells the compiler that the code is
already compiled in some library and thus the compiler can skip the compilation
and optimizing of given code fragment and leave it for the linker.

Reducing amount of exported symbols helps the linker (and also dynamic linker
at runtime), as it doesn't have to process huge symbol table containing stuff
that isn't used outside the library. See GCC's
`documentation about visibility <https://gcc.gnu.org/wiki/Visibility>`_.

The next part will be about optimizing the build system.
