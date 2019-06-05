New Application implementation for Emscripten
#############################################

:category: Announcements
:date: 2019-06-05
:tags: Emscripten, JavaScript, OpenGL, WebAssembly
:summary: If you build your Magnum apps for the web, you can now make use of
    a new feature-packed, smaller and more power-efficient application
    implementation. It is using the Emscripten HTML5 APIs directly instead of
    going through compatibility layers.

.. role:: js(code)
    :language: js
.. role:: cpp(code)
    :language: cpp
.. role:: cmake(code)
    :language: cmake
.. role:: link-doc-external(link)
    :class: m-flat

Until now, the :dox:`Platform::Sdl2Application` was the go-to solution for most
platforms including the web and mobile. However, not everybody needs all the
features SDL provides and, especially on Emscripten, apart from simplifying
porting it doesn't really add anything extra on top. On the contrary, the
additional layer of translation between HTML5 and SDL APIs increases the
executable size and makes some features unnecessarily hard to access.

To solve that, the new :dox:`Platform::EmscriptenApplication`, contributed in
:gh:`mosra/magnum#300` by :gh:`Squareys`, is using Emscripten HTML5 APIs
directly, opening new possibilities while making the code smaller and more
efficient.

`"SDL2" vs SDL2`_
=================

Since there's some confusion about SDL among Emscripten users, let's clarify
that first. Using SDL in Emscripten is actually possible in two ways --- the
implicit support, implemented in :gh:`library_sdl.js <emscripten-core/emscripten/blob/incoming/src/library_sdl.js>`,
gives you a slightly strange hybrid of SDL1 and SDL2 in a relatively small
package. Not all SDL2 APIs are present there, on the other hand it has enough
from SDL2 to make it a viable alternative to the SDL2 everyone is used to. This
is what :dox:`Platform::Sdl2Application` is using.

The other way is a "full SDL2", available if you pass ``-s USE_SDL=2`` to the
linker. Two years ago we tried to remove all Emscripten-specific workarounds
from :dox:`Platform::Sdl2Application` by switching to this full SDL2, but
quickly realized it was a bad decision --- in total it removed 30 lines of
code, but caused the resulting code to be almost 600 kB larger. The size
increase was so serious that it didn't warrant the very minor improvements in
code maintainability. For the record, the original pull request is archived at
:gh:`mosra/magnum#218`.

`The SDL-free EmscriptenApplication`_
=====================================

All application implementations in Magnum strive for almost full API
compatibility, with the goal of making it possible to use an implementation
optimal for chosen platform and use case. This was already the case with
:dox:`Platform::GlfwApplication` and :dox:`Platform::Sdl2Application`, where
switching from one to the other is in 90% cases just a matter of using a
different :cpp:`#include` and passing a different component to CMake's
:cmake:`find_package()`.

The new :dox:`Platform::EmscriptenApplication` continues in this fashion and we
ported all existing examples and tools that formerly used
:dox:`Platform::Sdl2Application` to it to ensure it works in broad use cases.
Apart from that, the new implementation fixes some of the long-standing issues
like miscalculated event coordinates on mobile web browsers or the
:label-default:`Delete` key leaking through text input events.

.. note-info::

    Only two widely used APIs are missing from it right now --- the
    :dox:`Platform::Sdl2Application::tickEvent()` and
    :dox:`Platform::Sdl2Application::setSwapInterval()`. The former will get
    added together with an equivalent in GLFW application, while the second
    will be exposed differently, allowing to use the extended browser APIs.
    Right now it's enough to :cpp:`#ifdef` around it, as browsers, unlike
    most desktop platforms, enable VSync by default.

`Power-efficient idle behavior`_
================================

Since the very beginning, all Magnum application implementations default to
redrawing only when needed in order to save power --- because Magnum is not
just for games that have to animate something every frame, it doesn't make
sense to use up all system resources by default. While this is simple to
implement efficiently on desktop apps where the application has the full
control over the main loop (and thus can block indefinitely waiting for an
input event), it's harder in the callback-based browser environment.

The original :dox:`Platform::Sdl2Application` makes use of
:link-doc-external:`emscripten_set_main_loop() <https://emscripten.org/docs/api_reference/emscripten.h.html#c.emscripten_set_main_loop>`,
which periodically calls :link-doc-external:`window.requestAnimationFrame() <https://developer.mozilla.org/en-US/docs/Web/API/window/requestAnimationFrame>`
in order to maintain a steady frame rate. For apps that need to redraw only
when needed this means the callback will be called 60 times per second only to
be a no-op. While that's still significantly more efficient than drawing
everything each time, it still means the browser has to wake up 60 times per
second to do nothing.

:dox:`Platform::EmscriptenApplication` instead makes use of
:link-doc-external:`requestAnimationFrame() <https://developer.mozilla.org/en-US/docs/Web/API/window/requestAnimationFrame>`
directly --- the next animation frame is implicitly scheduled, but cancelled
again after the draw event if the app doesn't wish to redraw immediately again.
That takes the best of both worlds --- redraws are still VSync'd, but the
browser is not looping needlessly if the app just wants to wait with a redraw
for the next input event. To give you some numbers, below is a ten-second
output of Chrome's performance monitor comparing SDL and Emscripten app
implementation waiting for an input event. You can reproduce this with the
`Magnum Player <{filename}/showcase/player.rst>`_ --- no matter how complex
animated scene you throw at it, if you pause the animation it will use as much
CPU as a plain static text web page.

.. container:: m-row

    .. container:: m-col-m-6

        .. figure:: {static}/img/blog/announcements/new-emscripten-application-implementation/sdl2.png
            :scale: 58%
            :figclass: m-flat

            Idle :dox:`Sdl2Application <Platform::Sdl2Application>`

    .. container:: m-col-m-6

        .. figure:: {static}/img/blog/announcements/new-emscripten-application-implementation/emscripten.png
            :scale: 58%
            :figclass: m-flat

            Idle :dox:`EmscriptenApplication <Platform::EmscriptenApplication>`

`DPI awareness revisited`_
==========================

Arguably to simplify porting, the Emscripten SDL emulation recalculates all
input event coordinates to match framebuffer pixels. The actual DPI scaling
(or device pixel ratio) is then being exposed through :dox:`dpiScaling() <Platform::Sdl2Application::dpiScaling()>`,
making it behave the same as Linux, Windows and Android on high-DPI screens. In
contrast, HTML5 APIs behave like macOS / iOS and
:dox:`Platform::EmscriptenApplication` follows that behavior ---
:dox:`framebufferSize() <Platform::EmscriptenApplication::framebufferSize()>`
thus matches device pixels while :dox:`windowSize() <Platform::EmscriptenApplication::windowSize()>`
(to which all events are related) is smaller on HiDPI systems. For more
information, check out the :dox:`Platform-EmscriptenApplication-dpi` docs.

.. note-success::

    It's important to note that even though different platforms expose DPI
    awareness in a different way, Magnum APIs are designed in a way that makes
    it possible to have the same code behave correctly everywhere. The
    separation into :dox:`dpiScaling() <Platform::Sdl2Application::dpiScaling()>`,
    :dox:`framebufferSize() <Platform::Sdl2Application::framebufferSize()>` and
    :dox:`windowSize() <Platform::Sdl2Application::windowSize()>` properties
    is mainly for a more fine-grained control where needed.

`Executable size savings`_
==========================

Because we didn't end up using the heavyweight "full SDL2" in the first place,
the difference in executable size is nothing extreme --- in total, in a Release
WebAssembly build, the JS size got smaller by about 20 kB, while the WASM file
stays roughly the same.

.. plot:: Download size (*.js, *.wasm)
    :type: barh
    :labels:
        Sdl2Application
        Sdl2Application
        EmscriptenApplication
    :labels_extra:
        -s USE_SDL=2
        -s USE_SDL=1
        ..
    :units: kB
    :values:
        111.9 74.4 52.1
        731.2 226.3 226.0
    :colors:
        success
        info

`Minimal runtime, or brain surgery with a chainsaw`_
====================================================

On the other hand, since the new application doesn't use any of the :cpp:`emscripten_set_main_loop()` APIs from ``library_browser.js``, it makes it
a good candidate for playing with the relatively recent
:gh:`MINIMAL_RUNTIME <emscripten-core/emscripten#7923>` feature of Emscripten.
Now, while Magnum is moving in the right direction, it's not yet in a state
where this would "just work". Supporting ``MINIMAL_RUNTIME`` requires either
moving fast and breaking lots of things or have the APIs slowly evolve into a
state that makes it possible. Because reliable backwards compatibility and
painless upgrade path is a valuable asset in our portfolio, we chose the
latter --- it will eventually happen, but not right now. Another reason is that
while Magnum itself can be highly optimized to be compatible with minimal
runtime, the usual application code is not able to satisfy those requirements
without removing and rewriting most third-party dependencies.

That being said, why not spend one afternoon with a chainsaw and try
demolishing the code to see what *could* come out? It's however important to
note that ``MINIMAL_RUNTIME`` is still a very fresh feature and thus it's very
likely that a lot of code will simply *not work* with it. All the discovered
problems are listed below because at this point there are *no results at all*
when googling them, so hopefully this helps other people stuck in similar
places:

-   :dox:`std::getenv()` or the :cpp:`environ` variable (used by
    :dox:`Utility::Arguments`) results in :js:`writeAsciiToMemory()` being
    called, which is right now `explicitly disabled <https://github.com/juj/emscripten/blob/458cc9bfcd42eccfd6294dbd00c373d2dab748f5/src/runtime_strings.js#L2>`_
    for minimal runtime (and thus you either get a failure at runtime or the
    Closure Compiler complaining about these names being undefined). Since
    Emscripten's environment is just a bunch of hardcoded values and Magnum is
    using Node.js APIs to get the real values for command-line apps anyway,
    solution is to simply not use those functions.
-   Right now, Magnum is using C++ iostreams on three isolated places
    (:dox:`Utility::Debug` being the most prominent) and those uses are
    gradually being phased off. On Emscripten, using anything that even
    remotely touches them will make the backend emit calls to
    :cpp:`llvm_stacksave()` and
    :cpp:`llvm_stackrestore()`. The `JavaScript implementations <https://github.com/emscripten-core/emscripten/blob/8b518967f71bdeee4d57a98f3bb9b78c0158e931/src/library.js#L1679-L1692>`_
    then call :js:`stackSave()` and :js:`stackRestore()` which however do not
    get pulled in in ``MINIMAL_RUNTIME``, again resulting in either a runtime
    error every time you call into JS (so also all
    :cpp:`emscripten_set_mousedown_callback()` functions) or when you use the
    Closure Compiler. After wasting a few hours trying to convince Emscripten
    to emit these two by adding :js:`_llvm_stacksave__deps: ['$stackSave']` the
    ultimate solution was to kill everything stream-related. Considering
    everyone who's interested in ``MINIMAL_RUNTIME`` probably did that already,
    it explains why this is another ungoogleable error.
-   If you use C++ streams, the generated JS driver file contains a full
    JavaScript implementation of :cpp:`strftime()` and the only way to get rid
    of it is removing all stream usage as well. Grep your JS file for ``Monday``
    --- if it's there, you have a problem.
-   JavaScript Emscripten APIs like :js:`dynCall()` or :js:`allocate()` are not
    available and putting them into either :js:`EXTRA_EXPORTED_RUNTIME_METHODS`
    or :js:`RUNTIME_FUNCS_TO_IMPORT` either didn't do anything or moved the
    error into a different place. For the former it was possible to work around
    it by directly calling one of its specializations (in that particular case
    :js:`dynCall_ii()`), the second resulted in a frustrated tableflip and the
    relevant piece of code getting cut off.

Below is a breakdown of various optimizations on a minimal application that
does just a framebuffer clear, each step chopping another bit off the total
download size. All sizes are uncompressed, built in Release mode with ``-Oz``,
``--llvm-lto 1`` and ``--closure 1``. Later on in the process,
:gh:`Bloaty McBloatFace <google/bloaty>` experimental
`WebAssembly support <https://groups.google.com/forum/#!topic/emscripten-discuss/jyLD-D09JUw>`_ was used to discover what functions contribute the most to final
code size.

.. class:: m-table

======================================================= =========== =========
Operation                                               JS size     WASM size
======================================================= =========== =========
Initial state                                           52.1 kB     226.3 kB
Enabling minimal runtime [1]_                           36.3 kB     224.5 kB
Additional slimming flags [2]_                          35.7 kB     224.5 kB
Disabling filesystem [3]_                               19.4 kB     224.5 kB
Chopping off all C++ stream usage                       14.7 kB     83.6 kB
Enabling :dox:`CORRADE_NO_ASSERT`                       14.7 kB     75.4 kB
Removing a single use of :dox:`std::sort()` [4]_        14.7 kB     69.3 kB
Removing one :dox:`std::unordered_map` [4]_             14.7 kB     62.6 kB
Using emmalloc instead of dlmalloc [5]_                 14.7 kB     56.3 kB
Removing all :dox:`printf() <std::printf()>` usage [6]_ 14.7 kB     44 kB *(estimate)*
======================================================= =========== =========

.. plot:: Download size (*.js, *.wasm)
    :type: barh
    :labels:
        Initial state
        Enabling minimal runtime
        Additional slimming flags
        Disabling filesystem
        Chopping off all C++ stream usage
        Enabling CORRADE_NO_ASSERT
        Removing a single use of std::sort()
        Removing one std::unordered_map
        Using emmalloc instead of dlmalloc
        Removing all printf() usage
    :units: kB
    :bar_height: 0.3
    :values:
        52.1 36.3 35.7 19.4 14.7 14.7 14.7 14.7 14.7 14.7
        226.3 224.5 224.5 224.5 83.6 75.4 69.3 62.6 56.3 44
    :colors:
        success
        info

.. [1] ``-s MINIMAL_RUNTIME=2 -s ENVIRONMENT=web -lGL`` plus temporarily
    enabling also ``-s IGNORE_CLOSURE_COMPILER_ERRORS=1`` in order to make
    Closure Compiler survive undefined variable errors due to iostreams and
    other, mentioned above
.. [2] ``-s SUPPORT_ERRNO=0 -s GL_EMULATE_GLES_VERSION_STRING_FORMAT=0 -s GL_EXTENSIONS_IN_PREFIXED_FORMAT=0 -s GL_SUPPORT_AUTOMATIC_ENABLE_EXTENSIONS=0 -s GL_TRACK_ERRORS=0 -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1``
    --- basically disabling what's enabled by default. In particular, the
    ``GL_EXTENSIONS_IN_PREFIXED_FORMAT=0`` is not supported by Magnum right
    now, causing it to not report any extensions, but that can be easily fixed.
    The result of disabling all these is ... underwhelming.
.. [3] ``-s FILESYSTEM=0``, makes Emscripten not emit any filesystem-related
    code. Magnum provides filesystem access through various APIs
    (:dox:`Utility::Directory`, :dox:`GL::Shader::addFile()`,
    :dox:`Trade::AbstractImporter::openFile()`, ...) and at the moment there's
    no possibility to compile all these out, so this is a nuclear option that
    works.
.. [4] :dox:`GL::Context` uses a :dox:`std::sort()` and a
    :dox:`std::unordered_map` to check for extension presence and print their
    list in the engine startup log. It was frightening to see a removal of a
    single :dox:`std::sort()` causing a 10% drop in executable size --- since
    WebGL has roughly two dozens extensions (compared to > 200 on desktop and
    ES), maybe a space-efficient alternative implementation could be done
    for this target instead.
.. [5] `Doug Lea <https://en.wikipedia.org/wiki/C_dynamic_memory_allocation#dlmalloc>`_\ 's
    :dox:`malloc() <std::malloc()>` is a general-purpose allocator, used by
    glibc among others. It's very performant and a good choice for code that
    does many small allocations (:dox:`std::unordered_map`, I'm looking at
    you). The downside is its larger size, and code doing fewer larger
    allocations might want to use ``-s MALLOC=emmalloc`` instead. We don't
    pretend Magnum is at that state yet, but other projects
    :gh:`sucessfully switched to it <BinomialLLC/basis_universal#7>`, shaving
    more bytes off the download size.
.. [6] After removing all of the above, :dox:`std::printf()` internals started
    appearing at the top of Bloaty's size report, totalling at about 10% of the
    executable size. Magnum doesn't use it anywhere directly and all transitive
    usage of it was killed together with iostreams; further digging revealed
    that it gets called from libc++'s `abort_message() <https://github.com/emscripten-core/emscripten/blob/6e4b98636618989bcd99308391e51aa1b81f4c61/system/lib/libcxxabi/src/abort_message.cpp#L25-L48>`_,
    for example when aborting due to a pure virtual function call. Independent
    measurement showed that :dox:`std::printf()` is around 12 kB of additional
    code compared to :dox:`std::puts()`, mainly due to the inherent complexity
    of floating-point string conversion. It's planned to use the much
    simpler and smaller `Ryū algorithm <https://dl.acm.org/citation.cfm?id=3192369>`_
    for Magnum's :dox:`std::printf()` replacement, additionally ensuring that
    float-to-string conversions can be :abbr:`DCE <dead code elimination>`-d
    when not used. We might be looking into patching Emscripten's libc++ to not
    use the expensive implementation in its abort messages.

While all of the above size reductions were done in a hack-and-slash manner,
the final executable still initializes and executes properly, clearing the
framebuffer and reacting to input events. For reference, check out diffs of
the ``chainsaw-surgery`` branches in :gh:`corrade <mosra/corrade$chainsaw-surgery>`
and :gh:`magnum <mosra/magnum$chainsaw-surgery>`.

The above is *definitely not* all that can be done --- especially considering
that removing *two* uses of semi-heavy STL APIs led to almost 20% save in code
size, there are most probably more of such low hanging fruits. The above tasks
were added to :gh:`mosra/magnum#293` (if not there already) and will get
gradually integrated into ``master``.

`Conclusion`_
=============

Bright times ahead! The new :dox:`Platform::EmscriptenApplication` is the first
step to truly minimal WebAssembly builds and the above hints that it's possible
to have download sizes not too far from `code carefully written in plain C <https://floooh.github.io/2018/05/01/cpp-to-c-size-reduction.html>`_.
To give a fair comparison, the basic framebuffer clear sample from
:gh:`floooh`\ 's `Sokol Samples <https://floooh.github.io/sokol-html5/>`_ is 42
kB in total, while the above equivalent is roughly 59 kB. Using C++(11), but
not overusing it --- and that's just the beginning.

.. note-dim::

    Questions? Complaints? Share your opinion on social networks:
    `Twitter <https://twitter.com/czmosra/status/1136419181442846720>`_,
    Reddit `r/cpp <https://www.reddit.com/r/cpp/comments/bx9qwl/optimizing_emscripten_c_apps_for_powerefficient/>`_,
    `r/WebAssembly <https://www.reddit.com/r/WebAssembly/comments/bx9uze/optimizing_emscripten_c_apps_for_powerefficient/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=20110251>`_
