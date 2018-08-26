Audio Example
#############

:css: {filename}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Shows how to play spatialized audio with Magnum. The audio scene includes a 3D
sound source and a (default) listener, which are visualized as a sphere and a
box respectively, the listener initially facing the sound source. The sound
source can be moved around the listener using the keyboard.

.. topic:: Key controls

    -   :label-default:`Right` / :label-default:`Left` --- rotate the source
        around Y axis
    -   :label-default:`Up` / :label-default:`Down` --- rotate the source
        around local X axis
    -   :label-default:`Page Up` / :label-default:`Page Down` --- move the
        source away/towards the listener

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="module" tabindex="0"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{filename}/showcase/audio/magnum-audio.js"></script>
        <script src="{filename}/showcase/EmscriptenApplication.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser with WebGL 1 enabled. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find further information and source code of this example
    :dox:`in the documentation <examples-audio>`.
