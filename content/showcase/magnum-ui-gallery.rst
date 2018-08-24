Magnum::Ui Gallery
##################

:css: {filename}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

A gallery of various widgets (buttons, labels, text input, modals) provided by
the :dox:`Magnum::Ui` library. It's possible to select among different themes:

-   `m.css dark theme <?>`_ (based on `m.css <http://mcss.mosra.cz>`_)
-   `Default theme <?style=default>`_

.. note-warning::

    This is a preview of an experimental work-in-progress feature. It is not
    yet fully tested and documented and may not behave properly on all
    platforms. Bug reports very welcome!

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="module"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{filename}/showcase/magnum-ui-gallery/magnum-ui-gallery.js"></script>
        <script src="{filename}/showcase/EmscriptenApplication.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser with WebGL 2 enabled. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`UI library itself <mosra/magnum-extras>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Documentation

    See the :dox:`Magnum::Ui` library documentation for more information.
    Again, please note that this is an experimental feature and the
    documentation is not yet finished.
