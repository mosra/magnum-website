Area Lights Example
###################

:css: {filename}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

.. role:: label-default
    :class: m-label m-default
.. role:: label-warning
    :class: m-label m-warning

A basic implementation of
`Real-Time Polygonal-Light Shading with Linearly Transformed Cosines <https://eheitzresearch.wordpress.com/415-2/>`_
by Eric Heitz, Jonathan Dupuy, Stephen Hill and David Neubelt.  Three lights
are displayed, each with different orientation and parameters.

.. note-warning::

    This example currently doesn't work on browsers that don't support the
    :webglext:`OES_texture_float_linear` extension. Fix will arrive later.

.. topic:: Key controls

    -   :label-default:`W` / :label-default:`A` / :label-default:`S` /
        :label-default:`D` --- move camera
    -   :label-default:`Mouse drag` --- look around
    -   :label-default:`R` / :label-warning:`Shift` :label-default:`R` ---
        increase / decrease roughness of the plane
    -   :label-default:`M` / :label-warning:`Shift` :label-default:`M` ---
        increase / decrease metalness of the plane
    -   :label-default:`F` / :label-warning:`Shift` :label-default:`F` ---
        increase / decrease :math:`f_0` (specular reflection coefficient) of
        the plane

    Roughness, metallness and :math:`f_0` can be also configured through the
    UI.

.. raw:: html

    <div id="wrapper3"><div id="wrapper2"><div id="wrapper"><div id="listener">
      <canvas id="module"></canvas>
      <div id="status">Initialization...</div>
      <div id="status-description"></div>
      <script async="async" src="{filename}/showcase/arealights/magnum-arealights.js"></script>
      <script src="{filename}/showcase/EmscriptenApplication.js"></script>
    </div></div></div></div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser with WebGL 2 enabled. If you see a black rectangle instead of a
    live example, the browser console might show some details about the error.
    See the `Showcase <{filename}/showcase.rst>`_ page for more information;
    you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find further information and source code of this example
    :dox:`in the documentation <examples-arealights>`.
