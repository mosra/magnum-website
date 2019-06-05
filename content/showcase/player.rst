Magnum Player
#############

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

A viewer and player for `glTF <https://www.khronos.org/gltf/>`_ scenes. Drop a
``*.gltf`` or ``*.glb`` file onto the canvas to load it.

.. note-warning::

    This is a preview of an experimental work-in-progress feature. It is not
    yet fully tested and documented and may not behave properly on all
    platforms. Bug reports very welcome!

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="canvas" tabindex="0"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{static}/showcase/player/magnum-player.js"></script>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
      </div></div></div>
    </div>
    <script>
    "use strict"; /* it summons the Cthulhu in a proper way, they say */

    function handleDragOver(event) {
        event.stopPropagation();
        event.preventDefault();
        event.dataTransfer.dropEffect = 'copy';
    }

    function handleDrop(event) {
        event.stopPropagation();
        event.preventDefault();

        const files = event.dataTransfer.files;
        if(!files) {
            console.error("No files dropped.");
            return;
        }

        /* Pass all files through to the player. Memory is allocated on the
           JS side and freed on the C++ side to avoid needless copies. */
        for(let i = 0; i != files.length; ++i) {
            (function(file) {
                const fileReader = new FileReader();
                fileReader.onload = function(event) {
                    /* TODO: but still, isn't here way too much copying? */
                    const fileData = new Uint8Array(event.target.result);
                    const pointer = Module._malloc(fileData.length);
                    const data = new Uint8Array(Module.HEAPU8.buffer, pointer, fileData.length);
                    data.set(fileData);
                    Module.ccall('loadFile', null, ['number', 'string', 'number', 'number'], [files.length, file.name, pointer, fileData.length]);
                };
                fileReader.onerror = function() {
                    console.error("Unable to read file " + file.name);
                };
                fileReader.readAsArrayBuffer(file);
            })(files[i]); /* this is how you do a capturing lambda?! ugh */
        }
    }

    /* Done here instead of using Sdl2Application::setContainerCssClass() so
       we can do extra things like hiding scrollbars */
    function setFullsize(fullsize) {
        if(fullsize) {
            document.getElementById('container').className = 'fullsize';
            document.body.style.overflow = 'hidden';
            window.scrollTo(0, 0);
        } else {
            document.getElementById('container').className = '';
            document.body.style.overflow = 'auto';
        }
    }

    Module.canvas.addEventListener('drop', handleDrop);
    Module.canvas.addEventListener('dragover', handleDragOver);
    </script>

.. block-flat:: Controls

    -   :label-default:`Space` plays or pauses the animation
    -   :label-default:`arrow keys` seek the animation forward or backward
    -   :label-default:`mouse drag` rotates the scene around the cursor
    -   :label-warning:`Shift` :label-default:`mouse drag` pans the scene at
        depth of the cursor
    -   :label-default:`mouse wheel` zooms in/out relative to cursor position
    -   :label-default:`Num 1` / :label-warning:`Ctrl` :label-default:`Num 1`
        switches to a front / back view
    -   :label-default:`Num 3` / :label-warning:`Ctrl` :label-default:`Num 3`
        switches to a right / left view
    -   :label-default:`Num 7` / :label-warning:`Ctrl` :label-default:`Num 7`
        switches to a top / bottom view
    -   :label-default:`Num 0` resets the camera back to the original view
    -   :label-default:`+` / :label-default:`Num +` or :label-default:`-` /
        :label-default:`Num -` increases or decreases lighting brightness

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 2 enabled. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`player app itself <mosra/magnum-extras>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Documentation

    See the :dox:`magnum-player` app documentation for more information.
    Again, please note that this is an experimental feature and the
    documentation is not yet finished.
