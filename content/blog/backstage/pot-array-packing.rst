Zero-waste single-pass packing of power-of-two textures
#######################################################

:date: 2022-06-15
:category: Backstage
:tags: C++, glTF, math
:summary: Rectangle packing doesn't actually have to be a NP-hard problem if we
    don't need to solve the most general case. In this post I present a simple
    yet optimal algorithm for packing power-of-two textures into a texture
    array.

.. |x| unicode:: U+00D7 .. nicer multiply sign

.. note-info::

    Sorry for the lack of updates on this site. No, Magnum is not dead, I'm
    just working on too many things at once and none of them is finished enough
    to be announced yet. This one is, though.

I don't claim that I "invented a novel algorithm" here. It's so simple that the
computing pioneers would definitely have it invented back in the 1970s already,
but I just couldn't find any prior art. On the other hand, I feel like I need
to risk stating the obvious, because otherwise the general consensus seems to
be that rectangle packing is *hard* no matter what the input is. Well, not
always.

`Background, or why even bother`_
=================================

I have a few hundreds of OBJ, COLLADA, PLY, etc. model files and the goal is to
pack them all into a single glTF file optimized for multi-draw\ [1]_. Thus,
containing a single vertex and index buffer, with meshes being sub-ranges of
this buffer, and a single 2D array texture using the proposed
:gh:`KHR_texture_ktx <KhronosGroup/glTF#1964>` extension\ [2]_, with materials
referencing (transformed) layers of the array.

For meshes, concatenation and baking relative transforms into them was easy
enough with tools like :dox:`MeshTools::concatenate()` or
:dox:`SceneTools::flattenMeshHierarchy3D()`. For texture packing, however, I
had only :dox:`TextureTools::atlas()`, and that's the most stupid and
inefficient implementation imaginable\ [3]_. The obvious next step was thus to
look into replacing it with something better --- but even the simplest
implementations I know of\ [4]_\ [5]_ are still quite complex with a lot of
toggles, heuristics and tradeoffs.

Furthermore, considering I have *hundreds* of input models of which many have
2048\ |x|\ 2048 textures, I'd quickly run of 2D texture size limits of even the
beefiest GPUs. Texture arrays, however, can have as many layers as I need.
Which however means almost all existing rectangle packing algorithms are out of
question, as they operate only on a single 2D texture.

`Looking at the data`_
======================

Instead of directly jumping onto adapting some existing general algorithm to
work with texture arrays, I procrastinated away. Stared at the data. And
realized that basically all models in the set I have follow the best practices,
and use square power-of-two textures --- 64\ |x|\ 64, 256\ |x|\ 256 and such.
Outliers that didn't follow this were less than one out of 100, thus not that
important. Those will be handled separately\ [6]_.

.. raw:: html
    :file: pot-array-packing/input.svg

The simpler flavors of packing algorithms first make all images either portrait
or landcape, sort them by size on one axis, and then, starting with the
largest, pack them into the output, *somehow*.

In my case, given that I can assume power-of-two squares and can just overflow
to another array layer when the previous gets full, the algorithm becomes
pretty clear. Importantly, power-of-two squares also make it possible to do
the packing in a way that makes all layers except the last one completely full,
with zero wasted space. Such constraint also helps avoiding texture-leaking
artifacts when using block compression --- except for the smallest sizes, a
4\ |x|\ 4 block (or 8\ |x|\ 8 for certain ASTC formats) will never span two
independent textures.

.. raw:: html
    :file: pot-array-packing/packed.svg

`The algorithm`_
================

The core of this algorithm is maintaining knowledge of the free space left. As
the space is recursively subdiving, it looks like a problem quadtrees could
solve, but fortunately as we go linearly from largest to smallest, we don't
even need that. All info that we need to maintain is how many images of
current size can still fit. Best explained with a visual example:

.. raw:: html
    :file: pot-array-packing/quadtree.svg

-   Let's say the output size is 2048\ |x|\ 2048, and the largest image we have
    is 1024\ |x|\ 1024. We can fit four of them into the output before the
    layer is full and we need to go to the next.
-   Putting aside any placement rules for now, we place two 1024\ |x|\ 1024
    images into the output (labeled **0** and **1**), which means it's just two
    slots left.
-   Next image in the list is 512\ |x|\ 512, which has a four times smaller
    area than the previous one. Thus, the leftover space quadruples, meaning
    we now have eight slots to fit 512\ |x|\ 512 images.
-   There are five 512\ |x|\ 512 images (labeled **20** to **23** and **30**),
    after which there's three slots left.
-   The next image size is 256\ |x|\ 256, which has a 4\ |x| smaller area, and
    thus the three 512\ |x|\ 512 slots become 12 256\ |x|\ 256 slots.
-   ...

Once an array layer is full, we start a new one, recalculating the number of
free slots based on the ratio of its area and area of the next image to place.
So, for example, we can fit 256 128\ |x|\ 128 images to the next layer of a
2048\ |x|\ 2048 output.

`Placement rules`_
------------------

Calculating remaining free space is hopefully clear, now the actual placement.
Here's where I finally make use of quadtree addressing, but again without
building any complex in-memory structure. As can be seen above, a particular
square in the quadtree can be thus addressed with a base-4 coordinate, where
first digit represents location in the top level, second inside given quadrant
of the top level and recursing further.

The important aspect of this addressing scheme is that the base-4 coordinate of
every image is actually the index of the free slot when adding the image, and
the way it works guarantees no overlap. I.e., when we added the first
256\ |x|\ 256 image above, labeled **310**, there were 12 slots left, and
:math:`(\frac{2048}{256})^2 - 12 = 52 = 310_4`. After adding the image, there
were just 11 slots left (:math:`311_4`), and thus there's no possibility for
the algorithm to return to earlier slots like :math:`3100_4` to :math:`3103_4`
and cause an overlap.

To convert these base-4 numbers to actual texture coordinates, I don't have
anything tangible to refer to\ [7]_, but when represented as binary, each odd
bit is an Y coordinate (lower or upper) and each even bit is an X coordinate
(left or right). Then, every two bits the coordinate range shrinks by half in
both dimensions, and the result is a sum of these.

`Full code`_
------------

For brevity, the following snippet assumes an already-sorted input with largest
images first, every size being a non-zero power-of-two square, and layer size
being at least as large as the largest image in the set. Outputs 3D offsets in
the array texture.

.. include:: pot-array-packing/main.cpp
    :code: c++
    :start-after: // [snippet]
    :end-before: // [/snippet]

`Environmental responsibility and waste management`_
====================================================

The only variable affecting output of this algorithm is the array layer size.
For the least waste in the last layer it's best to set it to size of the
largest image and not more. Generally the more layers you have the less
relative waste there is. With 100 layers, if the last layer would be containing
just one 1\ |x|\ 1 image (which is a rather unlikely worst-case scenario), the
relative waste is still less than 1%. Practically speaking however, if you'd
have thousands of 64\ |x|\ 64 or smaller images --- say, an icon pack --- then
it's better to waste a bit and choose a bigger layer size to avoid hitting
hardware texture size limits.

With a lot of input data there are also other factors to consider. While in my
case there were almost no non-power-of-two textures, there was *quite a few*
uniformly colored 2048\ |x|\ 2048 images that could be scaled down to 1\ |x|\ 1
without any loss in quality.

`Conclusion`_
=============

I don't claim any breakthrough invention. And since the algorithm is neither
generic\ [8]_ nor ML-enabled\ [9]_\ [10]_, I don't think it's cool enough to
gain much traction.

But here it is, implemented as :dox:`TextureTools::atlasArrayPowerOfTwo()` in
Magnum. Cheers!

.. note-dim::

    Questions? Complaints? Share your opinion on social networks: `Twitter <https://twitter.com/czmosra/status/1537117845351538692>`_

.. [1] In an idealistic case being able to submit a single draw call for (a
    culled subset of) the whole scene instead of drawing every mesh separately,
    significantly reducing overhead.
.. [2] There's :dox:`experimental KHR_texture_ktx support in GltfImporter <Trade-GltfImporter-behavior-textures-array>`
    if you are interested, available under an off-by-default option. A glTF
    exporter supporting this extension is currently in a WIP branch and will
    eventually be merged too.
.. [3] The current implementation, dating back to 2012, divides the output into
    a regular grid of cells that are all as large as the largest input. Yes,
    it's that bad.
.. [4] :gh:`TeamHypersomnia/rectpack2D`, or its distilled version at
    https://observablehq.com/@mourner/simple-rectangle-packing
.. [5] :gh:`juj/RectangleBinPack`
.. [6] Sent back to be reworked, ideally; if not possible then padded or
    resampled.
.. [7] Back in 2010, when high-speed mobile internet was still something
    unfathomable, I was reverse-engineering satellite imagery in Google Maps to
    download it for offline viewing on my countryside bike trips. Image URLs in
    one of the layers looked like ``/elevation/tqrsqrqqtstt.jpg`` and, having
    no prior knowledge about anything graphics-related, it took me quite a
    while to figure out what the letters meant. Since then I have the quadtree
    addressing scheme forever ingrained in my head.
.. [8] Yes, I still need to implement a real rectangle packing algorithm for
    the generic use case. Someday.
.. [9] *Ranked Reward: Enabling Self-Play Reinforcement Learning for
    Combinatorial Optimization*, 2018, https://arxiv.org/abs/1807.01672
.. [10] *Attend2Pack: Bin Packing through Deep Reinforcement Learning with
    Attention*, 2021, https://arxiv.org/abs/2107.04333
