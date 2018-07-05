Area Lights with LTCs
#####################

:date: 2018-01-15
:category: Guest posts
:cover: {filename}/img/blog/guest-posts/area-lights-with-ltcs.jpg
:tags: C++, Rendering, PBR, OpenGL, Emscripten, WebGL
:author: Jonathan Hale
:summary: A new Magnum example implements an analytic method for area light
    shading presented in the paper "Real-Time Polygonal-Light Shading with
    Linearly Transformed Cosines", by Eric Heitz, Jonathan Dupuy, Stephen Hill
    and David Neubelt.

.. role:: cpp(code)
    :language: c++
    :class: highlight

The code is available through the :dox:`examples-arealights` example page in
the documentation and the example has also a live web version linked below.
This blog post explains the basics of shading with linearly transformed cosines
(LTCs) and how Magnum was used for the implementation.

.. container:: m-row

    .. container:: m-col-m-6 m-push-m-3

        .. include:: ../../showcase-figures.rst.in
            :start-after: [arealights]
            :end-before: [/arealights]

`Shading with LTCs`_
====================

.. note-success::

    Check out the `paper on Eric Heitz's Research Page <https://eheitzresearch.wordpress.com/415-2/>`_
    for a more detailed and complete explanation. It is very well written and
    has a ton of supplemental material such as a WebGL implementation to play
    around with.

To understand linearly transformed cosines I will start off by explaining some
basics. If you already know what BRDFs are, you may want to skip the next
paragraph or two.

`Binary Reflectance Distribution Functions`_
--------------------------------------------

When shading a point on a surface, physically, you need to take all incoming
rays from every direction into account. Some light rays affect the final color of
the shaded point more than others --- depending on their direction,
the view direction and the properties of the material.

A perfect mirror, for example, may take only the exact reflection of the view
vector into account, as can be seen in figure (a), whereas a more diffuse
material will be affected by all or most incoming rays similarly or equally, as
visualized in figure (b).

These figures show spherical distributions: imagine you want to shade a point
on a surface (the point where the view vector is reflected). Imagine a ray of
light hitting this point, it will pierce through the colored sphere at some
location. The color at that location on the sphere indicates how much this ray
will affect the color of the surface point: the more red, the higher the effect.

.. container:: m-row

    .. container:: m-col-m-6

        .. figure:: {filename}/img/blog/guest-posts/area-lights-with-ltcs/brdf-mirror.png
            :alt: Visualization of the BRDF of a nearly perfect mirror
            :figclass: m-flat

            (a) Visualization of what the BRDF of a nearly perfect mirror may
            look like.

    .. container:: m-col-m-6

        .. figure:: {filename}/img/blog/guest-posts/area-lights-with-ltcs/brdf-diffuse.png
            :alt: Visualization of the BRDF of a diffuse material
            :figclass: m-flat

            (b) Visualization of what the BRDF of a more diffuse material
            may look like.

The function that describes how much effect an incoming light ray has for given
viewing and incoming light angles, is called
:abbr:`BRDF <binary reflectance distribution function>`.
This function is very specific to every material. As this is very impractical
for real-time rendering and art pipelines, it is common to instead use
a so called parametric BRDF; a function which is able to approximate many
different BRDFs of different materials using intuitive parameters, e.g.
roughness or metalness.

There are many parametric BRDFs out there: the GGX microfacet BRDF, the
Schlick BRDF and the Cook-Torrance BRDF. I recomment playing around with
them in
`Disney's BRDF Explorer <https://www.disneyanimation.com/technology/brdf.html>`_.

`Shading area lights`_
----------------------

With point lights, shading is really simple as you only have a single incoming
ray --- assuming you do not want to take indirect rays into account. You can
get the appropriate factor (of how much of that ray will be reflected in view
direction) from the BRDF using the view angle and light angle, multiply that
with the light intensity and that is already it.

With area lights, it is a lot more complicated, as you have an infinite amount
of incoming rays. The final intensity of the shaded point is the integral over
the BRDF in the domain of the polygon of the light (which, projected onto the
spherical distribution, is a spherical polygon).

This is a problem, because we do not have an analytical solution to integrating
over arbitrary spherical distributions. Instead, such a solution is known only
for very specific distributions, the uniform sphere or the cosine distribution
for example.

So, how can we still do it without radically approximating the area light?

`Linearly transformed cosines`_
-------------------------------

The genius of the paper is that the authors realized they can transform
spherical distributions using linear transforms (scaling, rotation and
skewing) and that this leaves the value of the integral unchanged.

.. container:: m-row

    .. container:: m-col-m-6

        .. figure:: {filename}/img/blog/guest-posts/area-lights-with-ltcs/ltc-cosine.png
            :alt: Visualization of a cosine distribution
            :figclass: m-flat

            (Untransformed) cosine distribution

    .. container:: m-col-m-6

        .. figure:: {filename}/img/blog/guest-posts/area-lights-with-ltcs/ltc-isotropic.gif
            :alt: Animation for scaling a cosine distribution
            :figclass: m-flat

            Uniformly scaled cosine distribution

            Results in more/less roughness.

    .. container:: m-clearfix-m

        ..

    .. container:: m-col-m-6

        .. figure:: {filename}/img/blog/guest-posts/area-lights-with-ltcs/ltc-anisotropic.gif
            :alt: Animation for scaling a cosine distribution on one axis
            :figclass: m-flat

            Cosine distribution scaled on one axis

            Results in anisotropy.

    .. container:: m-col-m-6

        .. figure:: {filename}/img/blog/guest-posts/area-lights-with-ltcs/ltc-skewness.gif
            :alt: Animation for skewing a cosine distribution
            :figclass: m-flat

            Skewed cosine distribution

.. class:: m-text m-text-right m-dim m-em

    Image source: `Eric Heitz's Research Page <https://eheitzresearch.wordpress.com/415-2/>`_

You can therefore transform a spherical distribution to look like another
spherical distribution. This means that you can transform something like the
cosine distribution to look like a specific BRDF given a certain view angle. You
can then --- because the integral is unaffected by the linear transform ---
integrate over the cosine distribution, to which an analytical solution is known,
instead of integrating over the BRDF.

As this BRDF is view dependent, you need a transformation for every incident
view angle, and every parameter of a parametric BRDF. In the paper, they achieve
this by fitting a 3x3 matrix (for the transformation) for a set of sampled values
for the BRDF parameter ``alpha`` (roughness) of the GGX Microfacet BRDF as well as
the viewing angle.

The 3x3 matrices have only four really significant components. Consequently they can
be stored in an RGBA texture.

.. math::

    M = \left(\begin{matrix}
            a & 0 & b \\
            0 & c & 0 \\
            d & 0 & 1
        \end{matrix}\right)

For shading we need the inverse matrices to transform the polygon of the light.
Originally it is of course in the space of the BRDF over which we do not
know how to integrate over. If we apply the inverse matrix to polygon, it is
then in the space of the cosine distribution over which we can integrate
instead.

.. figure:: {filename}/img/blog/guest-posts/area-lights-with-ltcs/ltc-integration.gif
    :alt: Animation for transforming the polygonal light into cosine distribution space
    :figclass: m-flat

    Transforming the BRDF and light polygon into a cosine distribution

.. class:: m-text m-text-right m-dim m-em

    Image source: `Eric Heitz's Research Page <https://eheitzresearch.wordpress.com/415-2/>`_

`Implementation`_
=================

To aid my understanding of the method, I implemented a basic version of LTC
shading using Magnum. The C++ example provided with the paper uses the Schlick
BRDF and already contained textures with the fitted inverse LTC matrices.

The code of the Magnum example is well documented and if you are interested, I
recommend you go check it out. Instead of giving a thorough line by line
explanation, I will point out some of the features in Magnum that were
most helpful to me. They are more generally applicable to other projects as
well.

`Loading LTC matrix textures`_
------------------------------

The original C++ implementation provided with the paper already contained .dds
files for the fitted inverse LTC matrices. Many thanks to Eric Heitz, who was
kind enough to let me use these for the Magnum example.

I packed these dds files as a resource into the binary (makes porting to web
easier later). It was a matter of simply adding the ``resources.conf``, telling
Corrade to compile it in your ``CMakeLists.txt``...

.. code:: cmake

    corrade_add_resource(AreaLights_RESOURCES resources.conf)

    add_executable(magnum-arealights AreaLightsExample.cpp ${AreaLights_RESOURCES})

... and then loading the texture from the resource memory using
:dox:`DdsImporter <Trade::DdsImporter>`:

.. code:: c++

    /* Load the DdsImporter plugin */
    PluginManager::Manager<Trade::AbstractImporter> manager{MAGNUM_PLUGINS_IMPORTER_DIR};
    std::unique_ptr<Trade::AbstractImporter> importer =
        manager.loadAndInstantiate("DdsImporter");
    if(!importer) std::exit(1);

    /* Get the resource containing the images */
    const Utility::Resource rs{"arealights-data"};
    if(!importer->openData(rs.getRaw("ltc_mat.dds")))
        std::exit(2);

    /* Set texture data and parameters */
    Containers::Optional<Trade::ImageData2D> image = importer->image2D(0);
    CORRADE_INTERNAL_ASSERT(image);
    _ltcMat.setWrapping(Sampler::Wrapping::ClampToEdge)
        .setMagnificationFilter(Sampler::Filter::Linear)
        .setMinificationFilter(Sampler::Filter::Linear)
        .setStorage(1, TextureFormat::RGBA32F, image->size())
        .setSubImage(0, {}, *image);

    /* Bind the texture for use in the shader */
    _shader.bindLtcMatTexture(_ltcMat);

`Shader Hot-Reload`_
--------------------

During shader development, you will not want to restart your application every
time you make a change to the GLSL shader code. It is rather nice to be able to
just hit :label-default:`F5` and see the changes immediately instead.

It turns out that if you implemented an :dox:`AbstractShaderProgram`,
hot-reloading is just a matter of reinstantiating it:

.. code:: c++

    /* Reload the shader */
    _shader = AreaLightShader{};

Yes, it is that simple.

Often you will compile your shader files as resources in Magnum (as done in the
example). To use shaders from a resource in your :dox:`AbstractShaderProgram`
you would again make use of :dox:`Utility::Resource`:

.. code:: c++

    Shader vert{version, Shader::Type::Vertex};
    Shader frag{version, Shader::Type::Fragment};

    /* Load shaders from compiled-in resource */
    Utility::Resource rs("arealights-data");
    vert.addSource(rs.get("AreaLights.vert"));
    frag.addSource(rs.get("AreaLights.frag"));

In this case you will need to override the resource group using
:dox:`Utility::Resource::overrideGroup()` to load the resource from the
original file rather than from memory before hot-reloading:

.. code:: c++

    /* Reload the shader */
    Utility::Resource::overrideGroup("arealights-data", "<path>/resources.conf");
    _shader = Shaders::AreaLight{};

`Thanks`_
=========

Final appreciations go to Eric Heitz, Jonathan Dupuy, Stephen Hill and David
Neubelt for publishing a incredibly well written paper with a ton of
supplemental material and effort around it --- and of course Magnum for making
it viable to quickly get this basic implementation running.

Thank you for reading! I'll be back.

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/952961309032767489>`_,
    `Reddit <https://www.reddit.com/r/cpp/comments/7qldic/area_lights_with_ltcs_in_the_magnum_graphics/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=16152101>`_
