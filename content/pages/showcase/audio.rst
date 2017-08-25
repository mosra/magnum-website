Audio example
#############

:save_as: showcase/audio/index.html
:url: /showcase/audio/
:css: {filename}/showcase/showcase.css

Shows how to play spatialized audio with Magnum. The audio scene includes a 3D
sound source and a (default) listener, which are visualized as a sphere and a
box respectively, the listener initially facing the sound source. The sound
source can be moved around the listener using the keyboard.

.. note-warning::

    Note that, currently, due to implementation limitation, the browser version
    of this example plays just a mono sound. Download and build the native
    version from the link below to experience true 3D sound.

.. topic:: Key controls

    -   **Right**/**Left** rotate the source around Y axis
    -   **Up**/**Down** rotate the source around local X axis
    -   **Page Up**/**Page Down** move the source away/towards the listener

.. raw:: html

    <div id="wrapper3"><div id="wrapper2"><div id="wrapper"><div id="listener">
      <canvas id="module"></canvas>
      <div id="status">Initialization...</div>
      <div id="status-description"></div>
      <script src="{filename}/showcase/EmscriptenApplication.js"></script>
      <script async="async" src="magnum-audio.js"></script>
    </div></div></div></div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser with WebGL 1 enabled. If you see a black rectangle instead of a
    live example, the browser console might show some details about the error.
    See the `Showcase <{filename}/pages/showcase.rst>`_ page for more
    information; you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code

    You can find :gh:`source code of this example <mosra/magnum-examples$master/src/audio>`
    on GitHub.
