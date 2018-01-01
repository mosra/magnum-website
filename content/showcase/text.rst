Text Example
############

:css: {filename}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Showcase of Magnum distance-field text rendering. Instead of rendering the
glyphs for each size the glyphs are prerendered and converted into signed
distance field texture. The texture is then used for rendering the text. Both
glyph prerendering and the actual text layouting supports UTF-8. For mutable
text, buffer mapping is used.

.. topic:: Controls

    -   **Mouse wheel** rotates and zooms the text

.. raw:: html

    <div id="wrapper3"><div id="wrapper2"><div id="wrapper"><div id="listener">
      <canvas id="module"></canvas>
      <div id="status">Initialization...</div>
      <div id="status-description"></div>
      <script async="async" src="{filename}/showcase/text/magnum-text.js"></script>
      <script src="{filename}/showcase/EmscriptenApplication.js"></script>
    </div></div></div></div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser with WebGL 1 enabled. If you see a black rectangle instead of a
    live example, the browser console might show some details about the error.
    See the `Showcase <{filename}/showcase.rst>`_ page for more information;
    you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code

    You can find :gh:`source code of this example <mosra/magnum-examples$master/src/text>`
    on GitHub.
