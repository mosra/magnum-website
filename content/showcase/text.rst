Text Example
############

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Showcase of Magnum distance-field text rendering. Instead of rendering the
glyphs for each size the glyphs are prerendered and converted into signed
distance field texture. The texture is then used for rendering the text. Both
glyph prerendering and the actual text layouting supports UTF-8. For mutable
text, buffer mapping is used.

.. topic:: Controls

    -   :label-default:`Mouse wheel` rotates and zooms the text

.. raw:: html

    <div class="mn-container">
      <div class="mn-sizer"><div class="mn-expander"><div class="mn-listener">
        <canvas class="mn-canvas" id="canvas" tabindex="0"></canvas>
        <div class="mn-status" id="status">Initialization...</div>
        <div class="mn-status-description" id="status-description"></div>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
        <script async="async" src="{static}/showcase/text/magnum-text.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 1 enabled. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find further information and source code of this example
    :dox:`in the documentation <examples-text>`.
