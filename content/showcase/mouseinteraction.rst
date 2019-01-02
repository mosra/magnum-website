Mouse Interaction
#################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Provides rotation, panning and zooming controls that are relative to depth at
mouse cursor for more convenient interaction with complex scenes.

.. topic:: Controls

    Similar to `Blender <https://www.blender.org/>`_ with *Zoom To Mouse
    Position* enabled.

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

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="module" tabindex="0"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{static}/showcase/mouseinteraction/magnum-mouseinteraction.js"></script>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 2 enabled. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find further information and source code of this example
    :dox:`in the documentation <examples-mouseinteraction>`.
