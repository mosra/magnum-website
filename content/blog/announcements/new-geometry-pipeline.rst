New geometry pipeline in Magnum
###############################

:date: 2020-05-27
:category: Announcements
:tags: C++, Eigen, glTF, Assimp, meshoptimizer, OpenGEX, OpenGL, Unreal Engine,
    Vulkan
:cover: {static}/img/blog/announcements/new-geometry-pipeline/cover.jpg
:summary: Flexible and efficient mesh representation, custom attributes, new
    data types and a ton of new processing, visualization and analyzing
    tools. GPU-friendly geometry storage as it should be in the 21st century.

.. role:: cpp(code)
    :language: c++
.. role:: ini(code)
    :language: ini
.. role:: sh(code)
    :language: sh

.. TODO: tags
.. TODO: cover
.. TODO: twitter -- Here's why there was no article in the past six+ months

During the past six months, Magnum had undergone a rather massive rework of its
very central parts --- mesh data import and storage. The original (and now
deprecated) :cpp:`Trade::MeshData2D` / :cpp:`Trade::MeshData3D` classes stayed
basically intact from the early 2010s when Magnum was nothing more than a toy
project of one bored university student, and were overdue for a replacement.

`How to not do things`_
=======================

While the :dox:`GL::Mesh` and :dox:`GL::Attribute` on the renderer side
provided all imaginable options for data layout and vertex formats, the
flexibility bottleneck was on the importer side. Increasingly unhappy about the
limitations, I ended up suggesting people to just sidestep the :dox:`Trade`
APIs and make their own representation when they needed to do anything
non-trivial. However, working on the replacement, I discovered --- the horror
--- that Magnum was far from the only library with such limitations embedded in
its design.

.. container:: m-col-m-8 m-left-m m-container-inflate

    .. code-figure::

        .. code:: c++
            :class: m-text m-small

            explicit MeshData3D(MeshPrimitive primitive,
                std::vector<UnsignedInt> indices,
                std::vector<std::vector<Vector3>> positions,
                std::vector<std::vector<Vector3>> normals,
                std::vector<std::vector<Vector2>> textureCoords2D,
                std::vector<std::vector<Color4>> colors,
                const void* importerState = nullptr);

        .. class:: m-text m-small m-dim m-noindent

        Source: :gh:`Magnum/Trade/MeshData3D.h <mosra/magnum/blob/6053cc6fc6761d8e7110d889f01924188106bef8/src/Magnum/Trade/MeshData3D.h#L68-L81>`
        :label-danger:`deprecated`

Here is the original Magnum API. While it allowed multiple sets of all
attributes (usable in mesh morphing, for example), adding a new attribute type
meant adding another vector-of-vectors (and updating calls to this constructor
everywhere), not to mention lack of support any sort of custom attributes or
ability to store different data types. The ``importerState`` is an extension
point that allows accessing arbitrary additional data, but it's
plugin-dependent and thus not usable in a generic way.

.. container:: m-col-m-5 m-right-m m-container-inflate

    .. code-figure::

        .. code:: c++
            :class: m-text m-small

            struct aiMesh
            {
                aiVector3D* mVertices;
                aiVector3D* mNormals;
                aiVector3D* mTangents;
                aiVector3D* mBitangents;
                aiColor4D* mColors[…];
                aiVector3D* mTextureCoords[…];
                aiFace* mFaces;
                …
            };

        .. class:: m-text m-small m-dim m-noindent

        Source: :gh:`assimp/mesh.h <https://github.com/assimp/assimp/blob/b7de061749ce98ef08f46d961876092f1e450ae5/include/assimp/mesh.h#L557-L724>`

Perhaps the most widely used asset import library, `Assimp <https://assimp.org/>`_,
has it very similar. All attributes are tightly packed and in a fixed type, and
while it supports a few more attribute types compared to the original Magnum
API, it has no custom attributes or formats either.

Fixed index and attribute types mean that input data has to be de-interleaved
and expanded to 32-bit ints and floats in order to be stored here, only to have
them interleaved and packed again later to have efficient representation on the
GPU. Both of those representations also *own* the data, meaning you can't use
them to reference external memory (for example a memory-mapped file or a GPU
buffer).

The ultimate winner of this contest, however, is `libIGL <https://libigl.github.io/>`_,
with the following function signature. Granted, it's templated to allow you to
choose a different index and scalar type, but you have to choose the type
upfront and not based on what the file actually contains, which kinda defeats
the purpose. What's the most amazing though is that every position and normal
is a three-component :dox:`std::vector`, every texture coordinate a
two-component vector *and then* each face is represented by another three
vector instances. So if you load a 5M-vertex mesh with 10M faces (which is not
that uncommon if you deal with `real data <https://twitter.com/zeuxcg/status/1210736070129680384>`_),
it'll be spread across 45 millions of allocations. Even with keeping all the
flexibility It could be just a handful\ [1]_, but why keep your feet on the
ground, right? The :dox:`std::string` passed by value is just a nice touch on
top.

.. container:: m-col-m-8 m-center-m

    .. code-figure::

        .. code:: c++
            :class: m-text m-small

            template <typename Scalar, typename Index>
            IGL_INLINE bool readOBJ(
              const std::string obj_file_name,
              std::vector<std::vector<Scalar > > & V,
              std::vector<std::vector<Scalar > > & TC,
              std::vector<std::vector<Scalar > > & N,
              std::vector<std::vector<Index > > & F,
              std::vector<std::vector<Index > > & FTC,
              std::vector<std::vector<Index > > & FN,
              std::vector<std::tuple<std::string, Index, Index >> &FM
              );

        .. class:: m-text m-small m-dim m-noindent

        Source: :gh:`igl/readOBJ.h <https://github.com/libigl/libigl/blob/3069260f0753df94b1615208ace86eb685b8df0c/include/igl/readOBJ.h#L69-L79>`

.. [1] To be fair, libIGL has an overload that puts the result
    `into just six regularly-shaped Eigen matrices <https://github.com/libigl/libigl/blob/3069260f0753df94b1615208ace86eb685b8df0c/include/igl/readOBJ.h#L104-L118>`_.
    However, it's implemented *on top of* the above (so you still need a :abbr:`military-grade <battle-hardened, fulfilling orders without judgment>`
    allocator) and it requires you to know *beforehand* that all faces in the file have the same size.

`Can we do better?`_
====================

The original pipeline (and many importer libraries as well) got designed with
an assumption that a file has to be *parsed* in order to get the geometry data
out of it. It was a sensible decision for classic textual formats such as OBJ,
COLLADA or OpenGEX, and there was little point in parsing those to anything
else than 32-bit floats and integers. For such formats, a relatively massive
amount of processing was needed either way, so a bunch of more copies and data
packing at the end didn't really matter:

.. container:: m-container-inflate

    .. image:: {static}/img/blog/announcements/new-geometry-pipeline/meshdata-before.svg
        :width: 100%

The new pipeline turns this assumption upside down, and instead builds on a
simple design goal --- being able to understand anything that the GPU can
understand as well. Interleaved data or not, half-floats, packed formats,
arbitrary padding and alignment, custom application-specific attributes and so
on. Then, assuming a file already has the data exactly as we want it, it can
simply copy the binary blob over to the GPU and only parse the metadata
describing offsets, strides and formats:

.. container:: m-container-inflate

    .. image:: {static}/img/blog/announcements/new-geometry-pipeline/meshdata-after.svg
        :width: 100%

For the textual formats (and rigidly-designed 3rd party importer libraries) it
means the importer plugin now has to do extra work that involves packing the
data into a single buffer. But that's an optimization done on the right side
--- with increasing model complexity it will make less and less sense to store
the data in a textual format.

`Enter the new MeshData`_
=========================

The new :dox:`Trade::MeshData` class accepts just two memory buffers --- a
typeless index buffer and a typeless vertex buffer. The rest is supplied as a
metadata, with :dox:`Containers::StridedArrayView` powering the data access
(be sure to check out the `original article on strided views <{filename}/blog/backstage/multidimensional-strided-array-views.rst>`_).
This, along with an ability to supply any :dox:`MeshIndexType` and
:dox:`VertexFormat` gives you almost unlimited\ [2]_ freedom of expression. As
an example, let's say you have your positions as half-floats, normals packed in
bytes and a custom per-vertex material ID attribute for deferred rendering,
complete with padding to ensure vertices are aligned to four-byte addresses:

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // packed
    :end-before: // packed
    :code: c++
    :class: m-inverted
    :hl-lines: 1 2 3 4 5 6 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

The resulting ``meshData`` variable is a self-contained instance containing all
vertex and index data of the mesh. You can then for example pass it directly to
:dox:`MeshTools::compile()` --- which will upload the ``indexData`` and
``vertexData`` as-is to the GPU without any processing, and configure it so the
builtin shaders can transparently interpret the half-floats and normalized
bytes as 32-bit floats:

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // compile
    :end-before: // compile
    :code: c++

The data isn't hidden from you either --- using :dox:`indices() <Trade::MeshData::indices()>`
or :dox:`attribute() <Trade::MeshData::attribute()>` you can directly access
the indices and particular attributes in a matching concrete type ...

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // access
    :end-before: // access
    :code: c++

... and because there's many possible types and not all of them are directly
usable (such as the half-floats), there are
:dox:`indicesAsArray() <Trade::MeshData::indicesAsArray()>`,
:dox:`positions3DAsArray() <Trade::MeshData::positions3DAsArray()>`,
:dox:`normalsAsArray() <Trade::MeshData::normalsAsArray()>` etc. convenience
accessors that give you the attribute unpacked to a canonical type so it can
be used easily in contexts that assume 32-bit floats. For example, calculating
an AABB of whatever position type is just an oneliner:

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // asarray
    :end-before: // asarray
    :code: c++

Among the :abbr:`evolutionary <as opposed to revolutionary>` things, mesh
attribute support got extended with tangents and bitangents (in both
representations, either a four-component tangent that glTF uses or a separate
three-component bitangent that Assimp uses), and :gh:`Squareys` is working on
adding support for vertex weights and joint IDs in :gh:`mosra/magnum#441`.

.. [2] You still need to obey the limitations given by the GPU, such as the
    index buffer being contiguous, all attributes having the same index buffer
    or all faces being triangles. `Unless you go with meshlets. <#going-further-custom-attributes-face-and-edge-properties-meshlets>`_

`Tools to help you around`_
===========================

Of course one doesn't always have data already packed in an ideal way, and
doing so by hand is tedious and error-prone. For that, the :dox:`MeshTools`
library got extended with various utilities operating directly on
:dox:`Trade::MeshData`. Here's how you could use :dox:`MeshTools::interleave()`
to create the above packed representation from a bunch of contiguous arrays,
possibly together with :dox:`Math::packInto()`, :dox:`Math::packHalfInto()` and
similar. Where possible, the actual :dox:`VertexFormat` is inferred from the
passed view type:

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // interleave
    :end-before: // interleave
    :code: c++
    :class: m-inverted
    :hl-lines: 6 7 8 9 10 11 12

Thanks to the flexibility of :dox:`Trade::MeshData`, many of historically quite
verbose operations are now available through single-argument APIs. Taking a
mesh, interleaving its attributes, removing duplicates and finally packing the
index buffer to the smallest type that can represent given range can be done by
chaining :dox:`MeshTools::interleave()`,
:dox:`MeshTools::removeDuplicates()` and :dox:`MeshTools::compressIndices()`:

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // meshtools-chained
    :end-before: // meshtools-chained
    :code: c++

There's also :dox:`MeshTools::concatenate()` for merging multiple meshes
together, :dox:`MeshTools::generateIndices()` for converting strips, loops and
fans to indexed lines and triangles, and others. Except for potential
restrictions coming from given algorithm, each of those works on an arbitrary
instance, be it an indexed mesh or not, with any kind of attributes.

Apart from the high-level APIs working on :dox:`Trade::MeshData` instances, the
existing :dox:`MeshTools` algorithms that work directly on data arrays were
ported from :dox:`std::vector` to :dox:`Containers::StridedArrayView`, meaning
they can be used on a much broader range of inputs.

`Binary file formats make the computer happy`_
==============================================

With a mesh representation matching GPU capabilities 1:1, let's look at a few
examples of binary file formats that could make use of it, their flexibility
and how they perform.

`glTF`_
-------

.. figure:: {static}/img/blog/announcements/new-geometry-pipeline/format-gltf.svg
    :figclass: m-fullwidth

    ..

    .. class:: m-text-center

    **glTF** --- :label-warning:`interleaved` :label-success:`attributes` or
    :label-info:`not`, do what you want as long as :label-dim:`indices` stay
    contiguous

The `"JPEG of 3D" <https://en.wikipedia.org/wiki/GlTF>`_ and its very flexible
binary mesh data representation was actually the initial trigger for this work
--- "what if we could simply memory-map the ``*.glb`` and render directly off
it?". In my opinion the current version is a bit too limited in the choice of
vertex formats (no half-floats, no 10.10.10.2 or float 11.11.10
representations for normals and quaternions), but that's largely due to its
goal of being fully compatible with unextended WebGL 1 and nothing an extension
couldn't fix.

To make use of a broader range of new vertex formats, Magnum's
:dox:`TinyGltfImporter <Trade::TinyGltfImporter>` got extended to support the
`KHR_mesh_quantization <https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_mesh_quantization>`_
glTF extension, together with
`KHR_texture_transform <https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_texture_transform>`_,
which it depends on. Compared to the more involved compression schemes
quantization has the advantage of not requiring any decompression step, as the
GPU can still understand the data without a problem. A quantized mesh will have
its positions, normals and texture coordinates stored in the smallest possible
type that can still represent the original data within reasonable error bounds.
So for example texture coordinates in a range of :math:`[0.5, 0.8]` will get
packed to a 8-bit range :math:`[0, 255]` and offset + scale needed to
dequantize them back to original range is then provided through the texture
transformation matrix. The size gains vary from model to model and depend on
the ratio between texture and vertex data. To show some numbers, here's a
difference with two models from the :gh:`glTF-Sample-Models <KhronosGroup/glTF-Sample-Models>`
repository, converted using the ``gltfpack`` utility from :gh:`meshoptimizer <zeux/meshoptimizer>`
(`more on that below <#meshoptimizer-and-plugin-interfaces-for-mesh-conversion>`_):

.. GLB created using magnum-plugins/src/MagnumPlugins/TinyGltfImporter/Test/gltf2glb.py
.. quantized using gltfpack
.. gltf+bin of packed version created using magnum-plugins/src/MagnumPlugins/TinyGltfImporter/Test/gltf2glb.py
.. JSON size is always `glb size - bin size` to discard the overhead of JSON
    formatting and indentation

.. plot:: Quantization using gltfpack
    :class: m-nopadb
    :type: barh
    :labels:
        Cesium Milk Truck
        Cesium Milk Truck
        Reciprocating Saw
        Reciprocating Saw
    :labels_extra:
        original *.glb
        quantized
        original *.glb
        quantized
    :units: kB
    :values:
        5 4.8 64 8
        417.6 417.6 0 0
        106.8 75.6 3416 2984
    :colors:
        default
        success
        info

.. class:: m-tiny m-unstyled m-block-dot-t m-text-center

-   :label-default:`JSON data size`
-   :label-success:`image data size`
-   :label-info:`mesh data size`

While packed attributes are supported by the GPU transparently, the builtin :dox:`Shaders::Phong` and :dox:`Shaders::Flat` had to be extended to support
texture transform as well.

`Stanford PLY`_
---------------

.. figure:: {static}/img/blog/announcements/new-geometry-pipeline/format-ply.svg
    :figclass: m-fullwidth

    ..

    .. class:: m-text-center

    **PLY** --- interleaved per-vertex :label-warning:`position`,
    :label-success:`normal` and :label-info:`color` data, followed by
    :label-primary:`size` and :label-dim:`indices` of each face

`PLY <https://en.wikipedia.org/wiki/PLY_(file_format)>`_ is a very simple, yet
surprisingly flexible and extensible format. Magnum has the
:dox:`StanfordImporter <Trade::StanfordImporter>` plugin for years, but
following the :dox:`Trade::MeshData` redesign it gained quite a few new
features, among which is support for vertex colors, normals, texture
coordinates and object IDs. PLYs also support 8- and 16-bit types for vertex
data, and similarly to glTF's `KHR_mesh_quantization`_ support are now imported
as-is, without expansion to floats.

Because PLYs are so simple *and* because PLYs are very often used for massive
scanned datasets (`Stanford Bunny <http://graphics.stanford.edu/data/3Dscanrep/>`_
being the most prominent of them), I took this as an opportunity to investigate
how far can Magnum reduce the import time, given that it can have the whole
chain under control. Plotted below is import time of a 613 MB scan model\ [3]_
with float positions, 24-bit vertex colors and a per-face 32-bit object ID
property that is purposedly ignored. Measured times start with the original
state before the :dox:`Trade::MeshData` rework, compare
:dox:`AssimpImporter <Trade::AssimpImporter>` and
:dox:`StanfordImporter <Trade::StanfordImporter>` configured for fastest
import\ [4]_ and show the effect of additional optimizations:

.. container:: m-hide-s

    [5]_ [6]_ [6]_ [7]_ [8]_ [9]_ [9]_

.. plot:: Import time, 613 MB Little-Endian PLY, Release
    :type: barh
    :labels:
        AssimpImporter + MeshData3D
        AssimpImporter + MeshData
        StanfordImporter + MeshData3D
        StanfordImporter + MeshData3D
        StanfordImporter + MeshData
        StanfordImporter + MeshData
        StanfordImporter + MeshData
        StanfordImporter + MeshData
        cat file.ply > /dev/null
        Magnum's upcoming *.blob format
    :labels_extra:
        original code
        new MeshData APIs ⁵
        original code
        w/o iostreams ⁶
        new MeshData APIs ⁵
        w/ triangle fast path ⁷
        one less copy on import ⁸
        zerocopy branch ⁹
        warm SSD cache
        meshdata-cereal-killer branch ⁹
    :units: seconds
    :values: 7.972 7.263 2.551 2.231 1.360 0.875 0.535 0.262 0.130 0.002
    :colors: danger danger warning warning info info info success default success

.. [4] For :dox:`AssimpImporter <Trade::AssimpImporter>`, the on-by-default
    :ini:`JoinIdenticalVertices`, :ini:`Triangulate` and :ini:`SortByPType`
    processing options were turned off, as those increase the import time
    significantly for large meshes. To have a fair comparison, in case of
    :dox:`StanfordImporter <Trade::StanfordImporter>` the
    :ini:`perFaceToPerVertex` option that converts per-face attributes to
    per-vertex was turned off to match Assimp that ignores per-face attributes completely.
.. [5] In case of :dox:`StanfordImporter <Trade::StanfordImporter>`, the main
    speedup comes from all :dox:`push_back() <std::vector::push_back()>`\ s
    replaced with a :dox:`Utility::copy()`, which is basically a fancier
    :dox:`std::memcpy()` that works on strided arrays as well.
    :dox:`AssimpImporter <Trade::AssimpImporter>` instead
    :dox:`assign() <std::vector::assign()>`\ ed the whole range at once which
    is faster, however the absolute speedup was roughly the same for both.
    Unfortunately not enough for Assimp to become significantly faster. Commit
    :gh:`mosra/magnum-plugins@79a185b2f63eefffa3f3d7d60b210fc5b37ef713` and
    :gh:`mosra/magnum-plugins@e67c217e7a8748049af9eee72a3d86f6432b035e`.
.. [6] The original :dox:`StanfordImporter <Trade::StanfordImporter>`
    implementation was using :dox:`std::getline()` to parse the textual header
    and :dox:`std::istream::read()` to read the binary contents. Loading the
    whole file into a giant array first and then operating on that proved to be
    faster. Commit :gh:`mosra/magnum-plugins@7d654f10a0eb8610ed486d85963c03172543d4fb`.
.. [7] PLY allows faces to have arbitrary N-gons, which means an importer has
    to go through each face, check its vertex count and triangulate if needed.
    I realized I could detect all-triangle files based solely by comparing face
    count with file size and then again use :dox:`Utility::copy()` to copy the
    sparse triangle indices to a tightly packed resulting array. Commit
    :gh:`mosra/magnum-plugins@885ba490b05c741a46335791cd43d540f93f5a68`.
.. [8] To make plugin implementation easier, if a plugin doesn't provide a
    dedicated :dox:`doOpenFile() <Trade::AbstractImporter::doOpenFile()>`,
    the base implementation reads the file into an array and then passes the
    array to :dox:`doOpenData() <Trade::AbstractImporter::doOpenData()>`.
    Together with assumptions about data ownership it causes an extra copy that
    can be avoided by providing a dedicated :dox:`doOpenFile() <Trade::AbstractImporter::doOpenFile()>`
    implementation. Commit :gh:`mosra/magnum-plugins@8e21c2f87a8492ba016b289b1d08c88f971223e9`.
.. [9] If the importer can make a few more assumptions about data ownership,
    the returned mesh data can be actually a view onto the memory given on
    input, getting rid of another copy. There's still some overhead left from
    deinterleaving the index buffer, so it's not faster than a plain :sh:`cat`.
    A custom file format allows the import to be done in 0.002 seconds, with
    the actual data reading deferred to the point where the GPU needs it ---
    and then feeding the GPU straight from a (memory-mapped) SSD. Neither of
    those is integrated into ``master`` yet, see
    `A peek into the future --- Magnum's own memory-mappable mesh format`_
    below.

`STL ("stereolithography")`_
----------------------------

.. figure:: {static}/img/blog/announcements/new-geometry-pipeline/format-stl.svg
    :figclass: m-fullwidth

    ..

    .. class:: m-text-center

    **STL** --- for each triangle a :label-success:`normal`, three corner
    :label-warning:`positions` and optional :label-info:`color` data

The `STL format <https://en.wikipedia.org/wiki/STL_(file_format)>`_ is
extremely simple --- just a list of triangles, each containing a normal and
positions of its corners. It's commonly used for 3D printing, and thus the
internet is also full of interesting huge files for testing. Until recently,
Magnum used :dox:`AssimpImporter <Trade::AssimpImporter>` to import STLs, and
to do another comparison I implemented a :dox:`StlImporter <Trade::StlImporter>`
from scratch. Taking a 104 MB file (`source <https://www.thingiverse.com/thing:3896745>`_,
`alternative <https://www.cgtrader.com/3d-print-models/art/sculptures/girl-and-dragon-3d-print-model>`_), here's the times ---
:dox:`AssimpImporter <Trade::AssimpImporter>` is configured the same as
above\ [4]_ and similar optimizations\ [8]_ as in
:dox:`StanfordImporter <Trade::StanfordImporter>` were done here as well:

.. container:: m-hide-s

    [8]_ [10]_

.. plot:: Import time, 104 MB STL, Release
    :type: barh
    :labels:
        AssimpImporter
        StlImporter
        StlImporter
        StlImporter
        cat file.stl > /dev/null
    :labels_extra:
        new MeshData APIs
        initial implementation
        one less copy on import ⁸
        per-face normals ignored ¹⁰
        warm SSD cache
    :units: seconds
    :values: 0.329 0.184 0.144 0.087 0.039
    :colors: warning info info success default

.. [10] Because the normals are per-triangle, turning them into per-vertex
    increases the data size roughly by a half (instead of 16 floats per
    triangle it becomes 24). Disabling this (again with a
    :ini:`perFaceToPerVertex` option) significantly improves import time.
    Commit :gh:`mosra/magnum-plugins@e013040d4dd4f6ecd16a713c56d626e60212dbdc`.

`MeshOptimizer and plugin interfaces for mesh conversion`_
==========================================================

While the :dox:`MeshTools` library provides a versatile set of APIs for various
mesh-related tasks, it'll never be able to suit the needs of *everyone*. Now
that there's a flexible-enough mesh representation, it made sense to extend the
builtin engine capabilities with external mesh conversion plugins.

The first mesh processing plugin is :dox:`MeshOptimizerSceneConverter <Trade::MeshOptimizerSceneConverter>`,
integrating :gh:`meshoptimizer <zeux/meshoptimizer>` by `@zeuxcg <https://twitter.com/zeuxcg>`_.
Author of this library is also responsible for the `KHR_mesh_quantization`_
extension and it's all-round a great piece of technology. Unleashing the plugin
in its default config on a mesh will perform the non-destructive operations ---
vertex cache optimization, overdraw optimization and vertex fetch optimization.
All those operations can be done in-place on an indexed triangle mesh using
:dox:`convertInPlace() <Trade::AbstractSceneConverter::convertInPlace()>`:

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // meshoptimizer
    :end-before: // meshoptimizer
    :code: c++

Okay, now what? This may look like one of those impossible
:label-flat-primary:`Press to render fast` magic buttons, and since the
operation took about a second at most and didn't make the output smaller in any
way, it can't really do wonders, right? Well, let's measure, now with a 179 MB
scan\ [3]_ containing 7.5 million triangles with positions and vertex colors,
how long it takes to render before and after meshoptimizer looked at it:

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. plot:: Rendering 7.5 M triangles, GPU time
            :type: barh
            :labels:
                Original
                Optimized
                Original
                Optimized
            :labels_extra:
                Intel 630
                Intel 630
                AMD Vega M
                AMD Vega M
            :units: ms
            :values: 62.52 20.95 11.98 9.91
            :colors: info success info success
            :plot-width: 4.5

    .. container:: m-col-m-6

        .. plot:: Rendering 7.5 M triangles, vertex fetch ratio
            :type: barh
            :labels:
                Original
                Optimized
                Original
                Optimized
            :labels_extra:
                Intel 630
                Intel 630
                AMD Vega M
                AMD Vega M
            :units: vertex shader invocations / all submitted vertices
            :values: 0.82 0.21 0.85 0.24
            :colors: info success info success
            :plot-width: 4.5

To simulate a real-world scenario, the render was deliberately done in a
default camera location, with a large part of the model being out of the view.
Both measurements are done using the (also recently added)
:dox:`DebugTools::GLFrameProfiler`, and while GPU time measures the time GPU
spent rendering one frame, vertex fetch ratio shows how many times a vertex
shader was executed compared to how many vertices were submitted in total. For
a non-indexed triangle mesh the value would be exactly 1.0, with indexed meshes
the lower the value is the better is vertex reuse from the post-transform
vertex cache\ [11]_. The results are vastly different for different GPUs, and
while meshoptimizer helped reduce the amount of vertex shader invocations for
both equally, it helped mainly the Intel GPU. One conclusion could be that the
Intel GPU is bottlenecked in ALU processing, while the AMD card not so much and
thus reducing vertex shader invocations doesn't matter that much. That said,
the shader used here was a simple :dox:`Shaders::Phong`, and the impact could
be likely much bigger for the AMD card with complex PBR shaders.

.. [11] Unfortunately the :glext:`ARB_pipeline_statistics_query` extension
    doesn't provide a way to query the count of indices submitted, so it's not
    possible to know the *overfetch* ratio --- how many times the vertex shader
    had to be executed for a single vertex. This is only possible if the
    submitted indices would be counted on the engine side.

Apart from the above, the :dox:`MeshOptimizerSceneConverter <Trade::MeshOptimizerSceneConverter>`
plugin can also optionally decimate meshes. As that is a destructive operation,
it's not enabled by default, but you can enable and configure it using
plugin-specific options:

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // meshoptimizer-simplify
    :end-before: // meshoptimizer-simplify
    :code: c++

Together with the mesh processing plugins, and similarly to image converters,
there's a new :dox:`magnum-sceneconverter <magnum-sceneconverter>` command-line
tool that makes it possible to use these plugins together with various mesh
tools directly on scene files. Its use is quite limited at this point as the
only supported output format is PLY (via
:dox:`StanfordSceneConverter <Trade::StanfordSceneConverter>`) but the tool
will gradually become more powerful, with more output formats. As an example,
here it first prints an info about the mesh, then takes just the first
attribute, discarding per-face normals, removes duplicate vertices, processes
the data with meshoptimizer on default settings and saves the output to a PLY:

.. code-figure::

    .. code:: sh

        magnum-sceneconverter dragon.stl --info

    .. code:: shell-session
        :class: m-console-wrap m-nopad m-inverted
        :hl-lines: 3 4

        Mesh 0:
          Level 0: MeshPrimitive::Triangles, 6509526 vertices (152567.0 kB)
            Offset 0: Trade::MeshAttribute::Position @ VertexFormat::Vector3, stride 24
            Offset 12: Trade::MeshAttribute::Normal @ VertexFormat::Vector3, stride 24

.. code-figure::

    .. code:: sh

        magnum-sceneconverter dragon.stl dragon.ply \
            --only-attributes "0" \
            --remove-duplicates \
            --converter MeshOptimizerSceneConverter -v

    .. code:: shell-session
        :class: m-console-wrap m-nopad m-inverted
        :hl-lines: 2 3 4 5 6 7 8 9 10 11 12 13 14 15

        Trade::AnySceneImporter::openFile(): using StlImporter
        Duplicate removal: 6509526 -> 1084923 vertices
        Trade::MeshOptimizerSceneConverter::convert(): processing stats:
          vertex cache:
            5096497 -> 1502463 transformed vertices
            1 -> 1 executed warps
            ACMR 2.34879 -> 0.69243
            ATVR 4.69757 -> 1.38486
          vertex fetch:
            228326592 -> 24462720 bytes fetched
            overfetch 17.5378 -> 1.87899
          overdraw:
            107733 -> 102292 shaded pixels
            101514 -> 101514 covered pixels
            overdraw 1.06126 -> 1.00766
        Trade::AnySceneConverter::convertToFile(): using StanfordSceneConverter

.. TODO: --only-attributes doesn't exist yet

The ``-v`` option translates to :dox:`Trade::SceneConverterFlag::Verbose`,
which is another new feature that enables plugins to print extended info about
import or processing. In case of
:dox:`MeshOptimizerSceneConverter <Trade::MeshOptimizerSceneConverter>` it
analyzes the mesh before and after, calculating :abbr:`average cache miss ratio <ACMR>`,
overdraw and other useful metrics for mesh rendering efficiency.

`Going further --- custom attributes, face and edge properties, meshlets`_
==========================================================================

To have the mesh data representation truly future-proofed, it isn't enough to
limit its support to just the "classical" indexed meshes with attributes of
predefined semantics and a (broad, but hardcoded) set of vertex formats.

Regarding vertex formats, similarly as is done `since 2018.04 for pixel formats <{filename}/blog/announcements/2018.04.rst#api-independent-asset-management>`_,
a mesh can contain any attribute in an implementation-specific format. One
example could be normals packed into for example :vktype:`VK_FORMAT_A2R10G10B10_SNORM_PACK32 <VkFormat>`
(which currently doesn't have a generic equivalent in :dox:`VertexFormat`) ---
code that consumes the :dox:`Trade::MeshData` instance can then unwrap the
implementation-specific vertex format and pass it directly to the corresponding
GPU API. Note that because the library has no way to know anything about sizes
of implementation-specific formats, such instances have only limited use in
:dox:`MeshTools` algorithms.

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // custom-format
    :end-before: // custom-format
    :code: c++

Meshes don't stop with just points, lines or triangles anymore. Together with
:dox:`Trade::AbstractImporter::mesh()` allowing a second parameter specifying
mesh level (similarly to image mip levels), this opens new possibilities ---
STL and PLY importers already use it to retain per-face properties, as shown
below on one of the `pbrt-v3 sample scenes <https://www.pbrt.org/scenes-v3.html>`_:

.. code-figure::

    .. code:: sh

        # Disabling the perFaceToPerVertex option to keep face properties as-is
        magnum-sceneconverter dragon_remeshed.ply --info \
            --importer StanfordImporter -i perFaceToPerVertex=false

    .. code:: shell-session
        :class: m-console-wrap m-nopad m-inverted
        :hl-lines: 5 6

        Mesh 0 (referenced by 0 objects):
          Level 0: MeshPrimitive::Triangles, 924422 vertices (10833.1 kB)
            5545806 indices @ MeshIndexType::UnsignedInt (21663.3 kB)
            Offset 0: Trade::MeshAttribute::Position @ VertexFormat::Vector3, stride 12
          Level 1: MeshPrimitive::Faces, 1848602 vertices (21663.3 kB)
            Offset 0: Trade::MeshAttribute::Normal @ VertexFormat::Vector3, stride 12

Among other possibilities is using :dox:`MeshPrimitive::Edges` to store meshes
in half-edge representation (the endlessly-flexible PLY format even has support
for `per-edge data <https://stackoverflow.com/a/58648176>`_, although the
importer doesn't support that yet), :dox:`MeshPrimitive::Instances` to store
instance data (for example to implement the proposed glTF
:gh:`EXT_mesh_gpu_instancing <KhronosGroup/glTF#1691>` extension) or simply
provide additional LOD levels (glTF has a :gh:`MSFT_lod <KhronosGroup/glTF/tree/master/extensions/2.0/Vendor/MSFT_lod>`
extension for this).

.. transition:: ~ ~ ~

.. container:: m-col-m-5 m-right-m  m-container-inflate

    .. code-figure::

        .. include:: new-geometry-pipeline/meshdata.cpp
            :start-after: // meshlets-struct
            :end-before: // meshlets-struct
            :code: c++
            :class: m-text m-small

        .. class:: m-text m-small m-dim m-noindent

        Source: :gh:`meshoptimizer.h <zeux/meshoptimizer/blob/2b103f2409329c023b8b50ce34085b8a9ae4e24b/src/meshoptimizer.h#L358-L376>`

Ultimately, we're not limited to predefined primitive and attribute types
either. The most prominent example of using this newly gained flexibility is
`mesh shaders and meshlets <https://devblogs.nvidia.com/introduction-turing-mesh-shaders/>`_.
Meshlets are a technique that is becoming more and more important for dealing
with heavy geometry, and :gh:`meshoptimizer <zeux/meshoptimizer>` has an
experimental support for those\ [12]_. For given input it generates a sequence
of statically-defined :cpp:`meshopt_Meshlet` structures that are then meant to
be fed straight to the GPU.

Describing such data in a :dox:`Trade::MeshData` instance is a matter of
defining a set of custom attribute names and listing their offsets, types and
array sizes, as shown below. While a bit verbose at first look, an advantage of
being able to specify the layout dynamically is that the same attributes can
work for representations from other tools as well, such as
:gh:`meshlete <JarkkoPFC/meshlete>`.

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // meshlets-data
    :end-before: // meshlets-data
    :code: c++
    :class: m-inverted
    :hl-lines: 8 9 10 11 12 13 14 15 16 17

One important thing to note is the *array attributes* --- those are accessed
with a special syntax, and give you a 2D view instead of a 1D one:

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // meshlets-access
    :end-before: // meshlets-access
    :code: c++

.. [12] Not mentioned in the project README, you need to look directly
    :gh:`in the source <zeux/meshoptimizer/blob/2b103f2409329c023b8b50ce34085b8a9ae4e24b/src/meshoptimizer.h#L358-L376>`.
    At the time of writing, meshlet generation isn't integrated into the
    :dox:`MeshOptimizerSceneConverter <Trade::MeshOptimizerSceneConverter>`
    plugin yet --- but it'll be, once I get a hardware to test the whole mesh
    shader pipeline on. If *you* want to play with them, there's a recent
    `introduction on Geeks3D <https://www.geeks3d.com/20200519/introduction-to-mesh-shaders-opengl-and-vulkan/>`_
    covering both OpenGL and Vulkan.

`Visualize the happiness of your data`_
=======================================

When working with mesh data of varying quality and complexity, it's often
needed to know not only how the mesh renders, but also *why* it renders that
way. The :dox:`Shaders::MeshVisualizer <Shaders::MeshVisualizer3D>` got
extended to have both a 2D and 3D variant and it can now visualize not just
wireframe, but also tangent space\ [13]_ --- useful when you need to know why
your lighting or a normal map is off ---, object ID for semantic annotations or
for example when you have multiple meshes batched together\ [14]_, and finally
two simple but very important properties --- primitive and vertex ID.

.. image-grid::

    {static}/img/blog/announcements/new-geometry-pipeline/tbn.jpg Tangent, bitangent and normal visualization
    {static}/img/blog/announcements/new-geometry-pipeline/object-id.jpg Object ID visualization

    {static}/img/blog/announcements/new-geometry-pipeline/primitive-id.jpg Primitive ID visualization
    {static}/img/blog/announcements/new-geometry-pipeline/vertex-id.jpg Vertex ID visualization

On the truck wheel\ [15]_ above you can see a very "rainbowy" primitive ID
visualization, which hints that the vertices are not rendered in an order that
would make good use of the vertex cache (and which can :dox:`MeshOptimizerSceneConverter <Trade::MeshOptimizerSceneConverter>`
help with). Vertex ID, on the other hand, can point to discontinuities in the
mesh --- even though the blanket\ [3]_ above would look like a smooth
continuous mesh to a naked eye, the visualization uncovers that almost none of
the triangles share a common vertex, which will likely cause issues for example
when decimating the mesh or using it for collision detection.

Supplementary to the mesh visualizer is a gallery of color maps for balanced
and easily recognizable visualizations. The above images were created using the
`Turbo color map <https://ai.googleblog.com/2019/08/turbo-improved-rainbow-colormap-for.html>`_
and :dox:`DebugTools::ColorMap` provides four more that you can choose from.

Lastly, and as already mentioned above, you're encouraged to use
:dox:`DebugTools::FrameProfiler` to measure various aspects of mesh renderer,
both on the CPU and GPU side and with builtin support for custom measurements
and delayed queries to avoid stalls. Hooking up this profiler doesn't mean you
suddenly need to deal with UI and text rendering --- it can simply print its
output to a terminal as well, refreshing itself every once in a while:

.. include:: new-geometry-pipeline/debugtools-frameprofiler.ansi
    :code: ansi

.. [3] The `Matterport3D indoor environment scans <https://niessner.github.io/Matterport/>`_
    were used as a source for various timings, benchmarks and visualizations
.. [13] Model source: :gh:`Lantern <KhronosGroup/glTF-Sample-Models/tree/master/2.0/Lantern>`
    from the glTF Sample Models repository
.. [14] Screenshot from a semantics-annotated scan from the
    `Replica dataset <https://github.com/facebookresearch/Replica-Dataset>`_
.. [15] Model source: :gh:`Cesium Milk Truck <KhronosGroup/glTF-Sample-Models/tree/master/2.0/CesiumMilkTruck>`
    from the glTF Sample Models repository

`Referencing external data, avoiding copies`_
=============================================

One of the ubiquitous annoying problems when dealing with STL containers is
memory management inflexibility --- you can't really\ [16]_ convince a
:dox:`std::vector` to reference external memory or, conversely, release its
storage and reuse it elsewhere. The new :dox:`Trade::MeshData` (and
:dox:`Trade::AnimationData` + :dox:`Trade::ImageData` as well, for that matter)
learned from past mistakes and can act as a non-owning reference to external
index and vertex buffers as well as attribute descriptions.

For example it's possible store index and vertex buffer for a particular model
in constant memory and make :dox:`Trade::MeshData` just reference it, without
any allocations or copies. In Magnum itself this is used by certain primitives
such as :dox:`Primitives::cubeSolid()` --- since a cube is practically always
the same, it doesn't make sense to build a copy of it in dynamic memory every
time.

Another thing the API was explicitly designed for is sharing a single large
buffer among multiple meshes --- imagine a glTF file containing several
different meshes, but all sharing a single buffer that you upload just once:

.. include:: new-geometry-pipeline/meshdata.cpp
    :start-after: // shared-buffer
    :end-before: // shared-buffer
    :code: c++

Lastly, nothing prevents :dox:`Trade::MeshData` from working in an "inverse"
way --- first use it to upload a GPU buffer, and then use the same attribute
layout to conveniently perform modifications when the buffer gets mapped back
to CPU memory later.

.. [16] Standard Library design advocates would mention that you can use a
    custom allocator to achieve that. While that's technically true, it's not
    a practical solution, considering the sheer amount of code you need to
    write for an allocator (when all you really need is a custom deleter).
    Also, have fun convincing 3rd party vendors that you need all their APIs to
    accept :dox:`std::vector`\ s with custom allocators.

`A peek into the future --- Magnum's own memory-mappable mesh format`_
======================================================================

Expanding further on the above-mentioned ability to reference external data,
it's now possible to have :dox:`Trade::MeshData` pointing directly to contents
of a memory-mapped file in a compatible format, achieving a truly zero-copy
asset loading. This is, to some extent, possible with all three --- STL, PLY
and glTF --- file formats mentioned above. A work-in-progress PR enabling this
is :gh:`mosra/magnum#240`, what I still need to figure out is interaction
between memory ownership and custom file loading callbacks; plus in case of
glTF it requires writing a new importer plugin based on
:gh:`cgltf <jkuhlmann/cgltf>` as :dox:`TinyGltfImporter <Trade::TinyGltfImporter>`
(and tiny_gltf in particular) can't really be convinced to work with external
buffers due to its heavy reliance on :dox:`std::vector`\ s.

At some point I realized that even with all flexibility that glTF provides,
it's still not ideal due to its reliance on JSON, which can have a large impact
on download sizes of WebAssembly builds.

What would a minimalist file format tailored for Magnum look like, if we
removed everything that can be removed? To avoid complex parsing and data
logistics, the file format should be as close to the binary representation of
:dox:`Trade::MeshData` as possible, allowing the actual payload to be used
directly without any processing, and the deserialization process being just a
handful of sanity and range checks. With that, it's then possible to have
import times *smaller* than what would a :sh:`cp file.blob > /dev/null` take
(`as shown above <#stanford-ply>`_), because we don't actually need to read
through all data at first --- only when given portion of the file is meant to
be uploaded to the GPU or processed in some other way:

.. code:: c++

    /* Takes basically no time */
    Containers::Array<char, Utility::Directory::MapDeleter> blob =
        Utility::Directory::mapRead("file.blob");

    /* Does a bunch of checks and returns views onto `blob` */
    Containers::Optional<Trade::MeshData> chair = Trade::MeshData::deserialize(blob);

Another aspect of the format is easy composability and extensibility ---
inspired by `RIFF <https://en.wikipedia.org/wiki/Resource_Interchange_File_Format>`_
and design of the `PNG file header <https://en.wikipedia.org/wiki/Portable_Network_Graphics#File_header>`_,
it's composed of sized chunks that can be arbitrarily composed together,
allowing the consumer to pick just a subset and ignore the rest. Packing a
bunch of meshes of diverse formats together into a single file could then look
like this:

.. code:: sh
    :class: m-inverted
    :hl-lines: 1 2 5

    magnum-sceneconverter file.blend --mesh "chair" chair.blob
    magnum-sceneconverter scene.glb --mesh "tree" tree.blob
    …

    cat chair.blob tree.blob car.blob > blobs.blob # because why not

Initial working implementation of all the above together with detailed format
specification is in :gh:`mosra/magnum#427`, and the end goal is to be able to
describe not just meshes but whole scenes. It's currently living in a branch
because the last thing a file format needs is compatibility issues --- it still
needs a few more iterations before its design settles down. This then goes
hand-in-hand with extending :dox:`Trade::AbstractSceneConverter` to support
more than just meshes alone, thus also making it possible to output glTF files
with :dox:`magnum-sceneconverter <magnum-sceneconverter>`, among other things.

.. transition:: * * *

And that's it for now. Thanks for reading and stay tuned for further advances
in optimizing the asset pipeline.

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/1265659775926362115>`_,
    `Reddit r/cpp <https://www.reddit.com/r/cpp/comments/grld3d/a_new_geometry_pipeline_in_the_magnum_graphics/>`_,
    `Reddit r/gamedev <https://www.reddit.com/r/gamedev/comments/grlery/a_new_geometry_pipeline_in_the_magnum_graphics/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=23324495>`_
