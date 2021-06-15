A Decade of Magnum
##################

:date: 2020-12-21
:category: Announcements
:cover: {static}/img/blog/announcements/a-decade-of-magnum.jpg
:tags: Vulkan, OpenGL, C++, Corrade, CMake, m.css, Eigen, GLM, Doxygen, singles
:summary: On 19th December 2010, Magnum saw its first commit. A bunch more
    commits happened since then and I learned some things along the way.

.. role:: cpp(code)
    :language: c++

A proven way to start a project is with a bunch of utilities created out of
necessity to make one's life a bit less miserable, and then iterating from
there. In my case, in late 2010, the `fifth edition of OpenGL SuperBible came out <hhttps://books.slashdot.org/story/10/10/25/134209/opengl-superbible-5th-ed>`_,
fully revised for modern OpenGL and throwing away everything related to the
classic fixed-function pipeline. The book was *great*, however the accompanying
code was not, and it was taking a lot of fun out of the learning experience. So
:gh:`I started an experiment <mosra/magnum@09d74422d955223db758f4939302eb8a662c88d9>`
to find a better way ... and then kept going.

.. transition:: ~ ~ ~

*A lot* can happen in a decade, so I'll skip the obvious dry knowledge like
"automated tests are good" and focus just on a few juicy bits.

`Let's try this new thing called C++0x`_
========================================

Back in the 2000s, C++ was this venerable old language that wasn't exactly fun
to use. C++11, originally codenamed C++0x, arrived to change that. I saw a huge
potential in the new features, especially scoped enums, move semantics and the
shorthand :cpp:`{}`-initialization --- "classic" C++, with non-copyable
classes allocated with :cpp:`new`, enums with ugly prefixes to avoid name
clashes and :cpp:`foo(const Vector3&)` / :cpp:`foo(float, float, float)`
overloads for everything suddenly felt like an ugly and inefficient maintenance
nightmare --- and so I decided to design the library with C++11 in mind.

.. container:: m-right-m m-col-m-4 m-container-inflate

        .. class:: m-text m-big m-em

        "You need to become old and wise and delay any new C++ feature adoption
        by at least 5 years."

        --- `@bkaradzic <https://twitter.com/bkaradzic/status/623625252447260672>`_

    ..

It was a risky decision and *hard* at first because I was commonly hitting
exciting yet-undiscovered compiler bugs. Industry veterans repeatedly told me
using C++11 was not a good idea, nevertheless I'm happy that I didn't back off
on that. (Even though backporting to GCC 4.4 to run on a
`BeagleBoard <http://beagleboard.org>`_ and to MSVC 2013 to finally run on
Windows was truly hellish.) The usability and cleanness of the design simply
wouldn't be there without C++11, though to this day I feel uneasy about using
lambdas because I remember how *bad* the support was in the early days, and I
can still enumerate all unfinished corners in the GCC 4.7 STL implementation.

Today, Magnum is *still* C++11 and I'm not really planning to require a newer
standard version anytime soon. While Magnum users are commonly on C++17, there
isn't anything nearly as groundbreaking as was in C++11 to be useful *on the
library level*. I'm now happily on the "caveman" side of the spectrum and users
that chose Magnum because it was the only thing still compiling on a CentOS
with GCC 4.8 confirm that I made the right decision.

`Make upgrades painless and APIs easy to deprecate`_
====================================================

When evolving a library, there's always a tradeoff in how much you allow
yourself to innovate `without making users too mad <https://thenewstack.io/greg-kroah-hartman-lessons-for-developers-from-20-years-of-linux-kernel-work/>`_.
If upgrades are painful, users will hesitate to upgrade, which means there will
often be nobody to discover bugs in new code. In turn, even less users want
to upgrade, resulting in a downwards spiral of the library increasingly losing
touch with user expectations.

I decided to not bother with ABI compatibility because it makes it basically
impossible to experiment, and that's no good in the fast-changing landscape of
GPU development. On the other hand, causing compiler failures every time the
users upgrade would mean they eventually lose patience and leave.

Over time, I realized what were the possibilities for API-compatible feature
deprecation:

-   A function can have an :cpp:`inline` alias that matches the previous
    signature as long as the two overloads are unambiguous
-   A class can be turned into a :cpp:`typedef` and vice versa (which is useful
    when making a class templated or turning a template into a concrete class)
-   A namespace can be renamed and its members exposed under the old name via
    :cpp:`using`, :cpp:`typedef` or :cpp:`namespace Old = New`
-   Headers can be renamed, split or joined and deprecated "proxies" kept that
    :cpp:`#include` from the new locations
-   A CMake target can hide *a lot* of backwards compatibility --- linking to
    libraries that were split out of the original target, adding back obsolete
    include paths, modifying compiler flags ...
-   Plugin interfaces can be changed freely as long as all the :cpp:`virtual`
    functions are `kept private <http://www.gotw.ca/publications/mill18.htm>`_,
    out of users' reach. That way plugin implementations can be switched to use
    the new interfaces and backwards compatibility provided only in the
    non-virtual public API instead of every plugin.

And what were the limits:

-   Plain structure data members are basically impossible to deprecate, so if
    you decide to have given member calculated on-the-fly or change its name /
    type, you have no way to provide backwards compatibility. This is why all
    Magnum getters are :cpp:`inline` functions, with data members never exposed
    directly.
-   It's not generally possible to reorder function arguments, unless the types
    are distinct and not implicitly convertible to each other. I'm stuck on
    this in a few places (:dox:`Utility::Directory::write()` is one).
-   Function return types are hard to change, unless the types are implicitly
    convertible to each other or otherwise compatible. This is also why I
    discourage users from :cpp:`auto` --- if they use a concrete type,
    backwards-compatibility measures ensure what's returned is implicitly
    converted to what's expected. Thanks to that the change from
    :dox:`std::unique_ptr` to :dox:`Containers::Pointer` everywhere went so
    smooth for most of the users.

While I want user code to keep building, I also want to *nudge* them to upgrade
to newer APIs and drop backwards-compatibility aliases and wrappers after a
year or two after deprecation. Apart from deprecated APIs being clearly marked
as such in the documentation, this is done using the :dox:`CORRADE_DEPRECATED()`
family of macros, which on sufficiently recent compilers can add deprecation
warnings on everything including functions, classes, enums, namespaces and
files. One step further, the users also have an option to
:dox:`disable backwards compatibility altogether <MAGNUM_BUILD_DEPRECATED>` and
fix even the spots the compiler didn't / couldn't warn about.

This paid off the most `when I made the OpenGL wrapper optional in 2018 <{filename}2018.04.rst>`_
--- while it was basically a complete rewrite of the most central parts, the
compatibility aliases made existing code still compile, only with a ton of
warnings that told people what to change and how. The upgrade went surprisingly
well for everybody and I finally removed the compatibility interfaces earlier
this year.

I'm happy to report that I have several long-time users running their
*production* code off Magnum ``master`` --- because they trust it that much.

`Friendly vendor lock-in`_
==========================

While projects made by industry veterans with good reputation are usually
trusted implicitly, a project from an unknown has to be *exceptionally perfect*
to make a dent. Though ... why even lower the bar as the reputation builds up over time?

Even though many successful projects can do with only a GitHub README just
fine, I spent several months building a `whole CSS layout framework and site theme <https://mcss.mosra.cz>`_
from scratch, with a `fast documentation search <{filename}/blog/meta/implementing-a-fast-doxygen-search.rst>`_
arriving shortly after. The time wasn't wasted and it was great to hear
feedback saying that people miss this in other projects, or see people adapting
Magnum's documentation system for their projects.

Apart from that there's various minor Quality-of-Life features one gets quickly
used to like the :cpp:`90.0_degf` or :cpp:`0x3bd267_rgbf` literals, ability to
conveniently print almost any container or enum with
:dox:`Debug <Utility::Debug>` or assert messages that show *what exactly* went
wrong, not just that *something* wrong happened. To help adoption, small
reusable bits of Magnum were extracted to :gh:`single-header libraries <mosra/magnum-singles>`
so users can easily bring their favorite APIs to other projects as well.

`Finding the right amount of NIH`_
==================================

.. container:: m-left-m m-col-m-4 m-container-inflate

        .. class:: m-text m-big m-em

        *If everyone followed the "Don't reinvent the wheel" saying, we'd still
        be stuck with wooden wheels today.*

    ..

While investing time into writing my own math library was worth it as I could
design something from the ground up without being tied to how GLSL works or how
math was done in the C++03 days, attempting to write my own physics library was
*a mistake*. It was a useful learning experience tho --- next time I made sure
that if I go all architecture astronaut on something, I'll implement the
actually useful bits (in case of physics, :dox:`intersection algorithms <Math::Intersection>`)
on a lower level first so when it ultimately doesn't work out, I don't need to
throw away *everything*.

The benefits of a layered approach was something that dawned on me very slowly
--- originally, to get anything on screen, it was mandatory to use a scene
graph, a camera abstraction and a barebones GLUT-based application. Today, you
can use for example just the math library and output to a SVG, without touching
the GPU or opening any window, and `the library pieces are largely independent <{filename}/features.rst>`_.
As the uses broadened from games to editors to research demos to heavyweight
data processing, it became clear that one design can't fit everything and while
Magnum APIs *can* be better for a certain use case than a common 3rd party lib,
there are also use cases for which the same 3rd party lib is more suited than
Magnum.

-   The builtin :dox:`Math` library is featureful and well documented, but if
    you need to do heavy calculations on large sparse matrices, Eigen is a
    better fit. :dox:`And we can do the type conversion for you. <EigenIntegration>` Or maybe the bigger half of your project already uses
    GLM? :dox:`No worries either. <GlmIntegration>`
-   Magnum has a hierarchical :dox:`SceneGraph`, but you can simply not use it,
    if you don't need. :gh:`Or, if you're into ECS instead, use entt <skypjack/entt>`.
-   There's :dox:`Platform::Sdl2Application` that covers most platforms, or
    :dox:`Platform::GlfwApplication` if you need just desktop and want to be
    leaner. Or :dox:`Platform::EmscriptenApplication` if you want something
    optimized for the web. Or go completely custom and embed Magnum in GTK,
    wxWidgets or Qt --- :gh:`for each there's a bootstrap project to get you started <mosra/magnum-bootstrap>`.
-   Want physics? Feel free to use Magnum with Bullet, DART, Box2D or for
    example PhysX. :dox:`Integrations <DartIntegration>`,
    :dox:`helpers <BulletIntegration>` and :dox:`examples <examples-box2d>`
    provided.
-   Want to use some OpenGL or Vulkan API that's not wrapped by Magnum, or
    combine with 3rd party rendering? :dox:`No problem. <vulkan-wrapping>`
-   Don't like Magnum's OpenGL or Vulkan wrappers at all, actually? Turn them
    off and keep using Magnum asset management APIs in combination with
    :dox:`sokol_gfx <examples-triangle-sokol>`, :gh:`bgfx <bkaradzic/bgfx>`
    and other 3rd party renderers.

I have to admit it took quite some effort to swallow the pride and accept the
fact that it's simply not humanly possible to make Magnum the best option for
*every* use case --- but in the end I realized that if I give the users
convenient integrations with 3rd party libraries, they will happily stay
because the remaining parts of Magnum are still worth it for them.

`It's not good to use the STL but it's not good to not use it either`_
======================================================================

Originally I wanted to upgrade to C++14 as soon as it comes out to make use of
:dox:`std::optional`, however that container got
:abbr:`overengineered beyond any reason <come on, optional references are just a pointer, really>`
and delayed to C++17; then I considered updating to C++17 to get
:dox:`std::string_view` and :cpp:`std::array_view`, however string views ended
up immutable and useless and the array view got renamed to :dox:`std::span` and
delayed to C++20, and :cpp:`std::mdspan` might finally arrive in C++23 if
things go well ...

One of the other long-term issues was :dox:`std::unique_ptr`.
:cpp:`#include <memory>` had a *measurable impact* on compile times from the
very beginning, which is why I hesitated to use it in class internals, meaning
a lot of `PIMPL <https://en.wikipedia.org/wiki/Opaque_pointer>`_'d state was
instead managed (and routinely leaked) using classic :cpp:`new` / :cpp:`delete`.

Even though it was suggested numerous times, I frowned upon the thought of
"writing my own STL" or switching to STLport / EASTL, because doing so would
mean alienating common users --- they would not only need to learn a new
engine, but also write extra code to transform their :dox:`std::vector`\ s and
:dox:`std::string`\ s to something the engine used. Only
`relatively recently <{filename}/blog/backstage/lightweight-stl-compatible-unique-pointer.rst>`_
I realized that I can design containers that are both STL-independent and
STL-compatible, so the engine can benefit from faster compile times and extra
flexibility like memory ownership transfer, but users can still keep using
:dox:`std::vector`, :dox:`std::unique_ptr` and the like, often without even
realizing those are not the types the engine natively works with.

`A friendly community is what keeps the project ticking`_
=========================================================

And finally, I can't underestimate how much this project owes the community
on :gh:`GitHub <mosra/magnum>`, `Gitter <https://gitter.im/mosra/magnum>`_ and
elsewhere for its existence. I had the luck that over the years I only had to
deal with a single person with offensive behavior, everyone else is going out
of their way to help each other, provide valuable feedback and encouragement
and contribute back massive amounts of great code.

I'm happy to see that every effort to help users do their first steps or
resolve their issues has returned back numerous times and the community brings
a continued wholesome experience.

**Thank you, everybody.** Cheers for the next ten years.

----------------------------

.. class:: m-text m-small m-dim

    Cover image created with `Gource <https://gource.io>`_, reflecting the
    state from December 19th, 2020.

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/1341174606288330753>`_, `Reddit r/cpp <https://www.reddit.com/r/cpp/comments/ki6zds/a_decade_of_magnum/>`_
