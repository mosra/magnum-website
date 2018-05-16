Community Contributions
#######################

:breadcrumb:
    {filename}/features.rst Features
:cover: {filename}/img/cover-features-community.jpg
:summary: Libraries, tools, helpers and concept projects built by our awesome
    community.
:footer:
    .. note-warning::

        Please note that these projects may have different licensing terms,
        different release schedule and generally different workflows. The
        projects might get out of sync with Magnum APIs or become abandoned ---
        we claim no responsibility. Contact the respective authors (or better,
        send them pull requests) if you encounter a problem. However, it's
        possible that particular projects will get integrated into Magnum
        itself at some point.

    .. note-success::

        The above list is not exhaustive --- have you created a Magnum addon
        and would like to have it listed here? Ping us at info@magnum.graphics,
        we'll gladly mention it.

    .. note-dim::
        :class: m-text-center

        `« Plugins & Extensions <{filename}/features/extensions.rst>`_ | `Features <{filename}/features.rst>`_

.. role:: label-success
    :class: m-label m-success
.. role:: label-danger
    :class: m-label m-danger

.. container:: m-container-inflate

    .. raw:: html
        :file: community.svg

On top of everything else, there are libraries, tools and concepts built by the
community --- not part of Magnum itself, but worth mentioning nevertheless.

`Dear ImGui integration`_
=========================

Integrates :gh:`Dear ImGui <ocornut/imgui>`, an immediate-mode UI library, into
Magnum. A fork of :gh:`denesik/MagnumImguiPort` with additional updates and a
WebGL port.

-   **GitHub:** :gh:`lecopivo/magnum-imgui` (:gh:`lecopivo/MagnumImguiPort`)
-   **Demo:** https://lecopivo.github.io/MagnumImguiPort/
-   **License:** :label-success:`MIT`
-   **Platforms:** any

`libRocket integration`_
========================

Integrates :gh:`libRocket <lloydw/libRocket>`, a HTML/CSS UI library, into
Magnum.

-   **GitHub:** :gh:`miguelmartin75/Magnum-libRocket`
-   **License:** :label-success:`MIT`
-   **Platforms:** any desktop

`Magnum Inspector`_
===================

GTK-based variable inspector window running alongside Magnum.

-   **GitHub:** :gh:`wivlaro/magnum-inspector`
-   **License:** :label-danger:`unspecified`
-   **Platforms:** any desktop

`GLM integration`_
==================

Provides basic integration of the `OpenGL Mathematics <https://glm.g-truc.net/>`_
library into Magnum.

-   **GitHub:** :gh:`sigman78/magnum-glm`
-   **License:** :label-danger:`unspecified`
-   **Platforms:** any

`Magnum Web Application template`_
==================================

An Emscripten-only version of ``magnum-bootstrap`` that integrates into
Webpack and allows easy inclusion of other JavaScript or WebAssembly modules.
The bundle also provides a local development server that watches the files and
refreshes the browser on code updates.

-   **GitHub:** :gh:`alkavan/magnum-web-application-template`
-   **License:** :label-success:`MIT`
-   **Platforms:** HTML5/Emscripten

`Magnum VR UI`_
===============

Mix of `Magnum Ui Gallery <{filename}/showcase/magnum-ui-gallery.rst>`_,
:dox:`Magnum Oculus VR example <examples-ovr>` and
:dox:`Magnum Leap Motion example <examples-leapmotion>`, showing how to fusion
them all together.

-   **GitHub:** :gh:`Squareys/magnum-vr-ui`
-   **License:** :label-success:`MIT`
-   **Platforms:** Windows

`Magnum Hot Reload`_
====================

Proof of concept implementation of hot code reloading using
:dox:`Corrade::PluginManager`.

-   **GitHub:** :gh:`Squareys/magnum-hotreload`
-   **License:** :label-success:`MIT`
-   **Platforms:** Windows

`MacPorts packages`_
====================

`MacPorts <https://www.macports.org/>`_ portfiles for Magnum.

-   **GitHub:** :gh:`sigman78/magnum-macports`
-   **License:** :label-danger:`unspecified`
-   **Platforms:** macOS
