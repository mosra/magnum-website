Static and dynamic polymorphism in Magnum
#########################################

:date: 2013-10-09 16:52
:modified: 2018-01-03
:archived: True
:category: Backstage
:tags: C++, Corrade, Qt
:summary: Thanks to generic programming and other features exclusive to C++ it
    is possible to handle polymorphism the most effective way for each use
    case. As a result, virtual calls are in Magnum used very sparingly.

.. role:: cpp(code)
    :language: c++

.. block-primary:: Backstage

    This article is part of a series describing the *how* and *why* behind
    Magnum graphics engine. Read also the other parts:

    1.  `C++11 features in Magnum: Better memory management <{filename}/blog/backstage/cpp11-features-in-magnum-better-memory-management.rst>`_
    2.  `C++11 features in Magnum: Simplifying daily workflow <{filename}/blog/backstage/cpp11-features-in-magnum-simplifying-daily-workflow.rst>`_
    3.  Static and dynamic polymorphism in Magnum

.. note-success:: Content care: Jan 03, 2018

    Code snippets were adapted to match current state of the Magnum API,
    documentation and source links are updated and some typos and grammar
    errors were fixed.

Some time ago I was digging in some opensource C++ graphics library and found
their :cpp:`Matrix4` class derived from some generic :cpp:`BaseObject` class
with many virtual functions, :cpp:`debugOutput()` being among them. I decided
to write this post to show that it can be done better than that.

`Run-time (dynamic) polymorphism`_
==================================

This is basically the stepping stone of Object Oriented Programming. You have
some well-defined base class and subclasses are (re)implementing the virtual
functions. In many cases you don't know about the implementation and just work
with reference to the base interface. However, this comes with performance
penalty --- each function call must be dispatched through a virtual table.
While this might not be an issue in conventional applications, it is much more
noticeable in tight game loops running at 60 FPS.

Magnum tries very hard to not overuse dynamic polymorphism. It is needed only
for plugins and scene graph internals. These two cases are implemented with
`Herb Sutter suggestions on virtuality <http://www.gotw.ca/publications/mill18.htm>`_
in mind, which allows many performance and usability improvements. In
particular, Magnum employs this approach:

-   No public-facing :cpp:`virtual` methods --- all virtual methods are called
    through public non-virtual proxy functions, which do additional checks,
    conversions and assertions. It means that the implementation contains only
    the code what matters, free of any boilerplate and redundant stuff or calls
    to parent implementation (which are forgotten more often than not).
-   Having non-virtual public method allows for greater flexibility in derived
    classes --- no issues with covariant return types or conflicting parameters
    and the awesome ability to inline the whole function call. Good
    implementation of public method in derived class can even remove the need
    for virtual call completely, without depending on compiler optimization
    promises.
-   If there is no need to call :cpp:`delete` on base class, the base doesn't
    even need virtual destructor, only a protected one. If the destructor is
    empty, it's then possible to make the type :cpp:`constexpr`, allowing for
    even more optimizations.

You can look into Magnum sources for real usage, files
:gh:`Magnum/Text/AbstractFont.h <mosra/magnum/blob/master/src/Magnum/Text/AbstractFont.h>`
and :gh:`Magnum/Text/AbstractFont.cpp <mosra/magnum/blob/master/src/Magnum/Text/AbstractFont.cpp>`
show consistency checks and conversions which would otherwise need to be done
redundantly in each and every implementation.

`Static polymorphism`_
======================

Unfortunately, in many statically typed languages, dynamic polymorphism is the
only real option. If you define two classes with similar interface, you can
easily swap one for another by just changing the type of variable, but that's
all you can do --- it's not possible to use the other type as parameter into
the same function.

With C++'s templating abilities it's possible to define generic functions
taking any suitable type. In dynamically typed languages (such as Python) this
is known as *duck typing*, but with considerable runtime penalties. In C++ the
performance of templated code is no different from other native code (apart
from possible compilation time / binary size increases, but that's another
story).

All classes with similar use cases in Magnum are statically polymorphic. It
means that you can for example swap transformation representation from
:dox:`DualQuaternion` to :dox:`Matrix4`, easily switch to different platform
toolkit (replace :dox:`Platform::GlutApplication` with :dox:`Platform::Sdl2Application`)
or use faster in-memory image representation on modern graphics cards
(:dox:`BufferImage2D` instead of :dox:`Image2D`). In most cases you can do that
without any additional changes to method calls and everything will just work.

Not everything can be conveniently done without sublcassing. To avoid having
virtual destructor, the base destructor is made protected (as is the case with
:dox:`AbstractFramebuffer` and other classes which are not meant to be
instantiated directly). Also, it's possible to cheat a little with primitive
types for math structures, as only the base class contains the actual data and
the destructors in subclasses are basically a no-op (for example :dox:`Math::Color4`,
derived from :dox:`Math::Vector4`, which is derived from :dox:`Math::Vector`).
In this case not calling destructors of derived classes won't cause any harm
and no memory will be leaked.

Making (statically) polymorphic interface for :dox:`Platform` namespace was the
hardest thing --- windowing toolkits support very diverse feature set, which is
most noticeable in event handlers. It means that it's not possible to pass the
values as separate function parameters, because switching to another toolkit
with different event properties would be a nightmare. A structure is passed
instead, which then has separate getters for all the properties.

From user's point-of-view the usage is the same as if these classes were
implemented using dynamic polymorphism, but internally the code is much faster
thanks to inlining and no need for virtual dispatch or conversion of parameters
to implementation-specific values. Moreover this is the way how STL is
designed.

`No polymorphism`_
==================

Thanks to operator overloading in C++ it is possible to use an operator instead
of implementing a virtual function. The essential example is equality
comparison and debug output (various :cpp:`toString()` or :cpp:`equals()`
functions). The :dox:`Utility::Debug` class uses, similarly to STL and Qt's
:cpp:`qDebug()`, :cpp:`operator<<` to print values on debug output.
