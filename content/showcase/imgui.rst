ImGui Example
#############

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Integration of :gh:`Dear ImGui <ocornut/imgui>` into Magnum. Shows the ImGui
demo window and two other example windows which demonstrate rendering of
immediate-mode GUI using the HiDPI-enabled :dox:`ImGuiIntegration` library.

.. note-success:: WebGL 2

    At the moment, the :dox:`ImGuiIntegration` library is running on WebGL 2
    / OpenGL ES 3 only. Backport to WebGL 1 (GLES 2) for greater compatibility
    is planned soon.

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="module" tabindex="0"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{static}/showcase/imgui/magnum-imgui.js"></script>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
      </div></div></div>
    </div>
    <script>
    "use strict"; /* it summons the Cthulhu in a proper way, they say */

    Module.canvas.addEventListener('contextmenu', function(event) {
        event.preventDefault();
    }, true);
    </script>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 2 enabled. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`ImGuiIntegration library itself <mosra/magnum-integration>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find further information and source code of this example
    :dox:`in the documentation <examples-imgui>`.
