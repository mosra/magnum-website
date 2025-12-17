ImGui Example
#############

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Integration of :gh:`Dear ImGui <ocornut/imgui>` into Magnum. Shows the Dear
ImGui demo window and two other example windows which demonstrate rendering of
immediate-mode GUI using the HiDPI-enabled :dox:`ImGuiIntegration` library.

.. raw:: html

    <div class="mn-container">
      <div class="mn-sizer"><div class="mn-expander"><div class="mn-listener">
        <canvas class="mn-canvas" id="canvas" tabindex="0"></canvas>
        <div class="mn-status" id="status">Initialization...</div>
        <div class="mn-status-description" id="status-description"></div>
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
    browser with WebGL 1 enabled. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`ImGuiIntegration library itself <mosra/magnum-integration>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find further information and source code of this example
    :dox:`in the documentation <examples-imgui>`.
