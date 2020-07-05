Community Contributions
#######################

:breadcrumb:
    {filename}/features.rst Features
:cover: {static}/img/cover-features-community.jpg
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

    .. block-success:: Do you have something?

        The above list is not exhaustive --- have you created a Magnum addon
        and would like to have it listed here? Ping us at info@magnum.graphics,
        we'll gladly mention it. Note that some widely-used projects that used
        to be listed here have since become an officially supported part of
        Magnum --- see for example the :dox:`GlmIntegration` and the
        :dox:`ImGuiIntegration` libraries.

    .. note-dim::
        :class: m-text-center

        `« Plugins & Extensions <{filename}/features/extensions.rst>`_ | `Features <{filename}/features.rst>`_

.. container:: m-container-inflate

    .. raw:: html
        :file: community.svg

On top of everything else, there are libraries, tools and concepts built by the
community --- not part of Magnum itself, but worth mentioning nevertheless.

`Graphics Template App`_
========================

A self-contained template for developing graphics applications containing
Magnum, ImGui, :gh:`ImGuizmo <CedricGuillemet/ImGuizmo>` and GLFW. All
dependencies are included CMake subprojects --- generate a project file and
build.

-   **GitHub:** :gh:`ttnghia/GraphicsTemplateApp`
-   **License:** :label-success:`Apache 2.0`
-   **Platforms:** any

`Fedora RPM packages`_
======================

-   **GitHub:** :gh:`alkavan/magnum-rpm-specs-f28`
-   **License:** :label-success:`MIT`
-   **Platforms:** Fedora Linux

`OpenSUSE and Ubuntu packages`_
===============================

Maintained as dependencies for `Monopticon <https://github.com/NSkelsey/monopticon>`_.

.. role:: abbr-strong(abbr)
    :class: m-text m-strong

-   :abbr-strong:`OBS: <OpenSUSE Build Service>` https://build.opensuse.org/project/show/home:synnick
-   **License:** :label-danger:`GPLv3`
-   **Platforms:** OpenSUSE, Ubuntu Linux

`Magnum Tips`_
==============

Collection of useful snippets and handy editor-like utilities for Magnum.

-   **GitHub:** :gh:`alexesDev/magnum-tips`
-   **License:** :label-primary:`UNLICENSE`
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

Mix of `Magnum Ui Gallery <{filename}/showcase/ui-gallery.rst>`_,
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

`Bazel buildsystem`_
====================

Integrates Magnum into the `Bazel <https://bazel.build/>`_ buildsystem.

-   **GitHub:** :gh:`dbacchet/magnum-bazel`
-   **License:** :label-success:`MIT`
-   **Platforms:** Ubuntu 16.04 and 18.04, macOS 10.14+

`MacPorts packages`_
====================

`MacPorts <https://www.macports.org/>`_ portfiles for Magnum.

-   **GitHub:** :gh:`sigman78/magnum-macports`
-   **License:** :label-danger:`unspecified`
-   **Platforms:** macOS
