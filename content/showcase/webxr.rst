WebXR example
#############

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase
:alias: /showcase/webvr/

A basic demonstration of how to use the
`Emscripten WebXR library <https://github.com/VhiteRabbit/emscripten-webxr>`_
with Magnum. This replaces the example for the now-obsolete `WebVR API <https://kripken.github.io/emscripten-site/docs/api_reference/vr.h.html>`_.

For the full experience, this example requires a WebXR 1.0 capable browser and
a VR headset. Before Chrome 79 or Oculus Browser 7.0 you may need to enable
WebXR support explicitly on your browser --- on Chrome go into ``chrome://flags``
and search for WebXR. Works on Android as well.

.. topic:: Controls

    -   :label-default:`Mouse click` --- enter VR
    -   :label-default:`Esc` --- exit VR

.. raw:: html

    <div class="mn-container mn-aspect-2-1">
      <div class="mn-sizer"><div class="mn-expander"><div class="mn-listener">
        <canvas class="mn-canvas" id="canvas" tabindex="0"></canvas>
        <div class="mn-status" id="status">Initialization...</div>
        <div class="mn-status-description" id="status-description"></div>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
        <script async="async" src="{static}/showcase/webxr/magnum-webxr.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 1 enabled and WebXR support. See the
    `Showcase <{filename}/showcase.rst>`_ page for more information; you can
    also report a bug either for the :gh:`example itself <mosra/magnum-examples>`
    or :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find further information and source code of this example
    :dox:`in the documentation <examples-webxr>`.
