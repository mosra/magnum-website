WebVR example
#############

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

A basic demonstration of how to use the
`Emscripten VR API <https://kripken.github.io/emscripten-site/docs/api_reference/vr.h.html>`_
with Magnum.

For the full experience, this example requires a WebVR 1.1 capable browser and
a VR headset. If you don't have a VR headset, you can try to load the example
on your phone or use one of the
`WebVR API emulation <https://chrome.google.com/webstore/detail/webvr-api-emulation/gbdnpaebafagioggnhkacnaaahpiefil>`_
browser extensions. You may need to enable WebVR support explicitly on your
browser --- on Chrome go to ``chrome://flags`` and search for WebVR. Works on
Android as well.

.. topic:: Controls

    -   :label-default:`Mouse click` --- enter VR
    -   :label-default:`Esc` --- exit VR

.. raw:: html

    <div id="container" class="aspect-2-1">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="canvas" tabindex="0"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{static}/showcase/webvr/magnum-webvr.js"></script>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 1 enabled and WebVR support. See the
    `Showcase <{filename}/showcase.rst>`_ page for more information; you can
    also report a bug either for the :gh:`example itself <mosra/magnum-examples>`
    or :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find further information and source code of this example
    :dox:`in the documentation <examples-webvr>`.
