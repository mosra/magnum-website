Magnum::Ui Gallery
##################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase
:alias: /showcase/magnum-ui-gallery/

A gallery of various widgets (buttons, labels, text input, modals) provided by
the :dox:`Magnum::Ui` library. It's possible to select among different themes:

-   `m.css dark theme <?>`_ (based on `m.css <https://mcss.mosra.cz>`_)
-   `Default theme <?style=default>`_

.. note-warning::

    This is a preview of an experimental work-in-progress feature. It is not
    yet fully tested and documented and may not behave properly on all
    platforms. Bug reports very welcome!

.. raw:: html

    <div class="mn-container">
      <div class="mn-sizer"><div class="mn-expander"><div class="mn-listener">
        <canvas class="mn-canvas" id="canvas" tabindex="0"></canvas>
        <div class="mn-status" id="status">Initialization...</div>
        <div class="mn-status-description" id="status-description"></div>
        <script async="async" src="{static}/showcase/ui-gallery/magnum-ui-gallery.js"></script>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 2 enabled. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`UI library itself <mosra/magnum-extras>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Documentation

    See the :dox:`magnum-ui-gallery` app documentation for more information.
    Again, please note that this is an experimental feature and the
    documentation is not yet finished.
