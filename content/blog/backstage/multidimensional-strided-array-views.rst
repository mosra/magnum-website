Multi-dimensional strided array views in Magnum
###############################################

:date: 2019-04-30
:category: Backstage
:tags: C++, animation, Python
:summary: Magnum recently gained a new data structure usable for easy data
    description, transformation and inspection, opening lots of new
    possibilities for more efficient workflows with pixel, vertex and animation
    data.

While :dox:`Containers::ArrayView` and friends
`described previously <{filename}array-view-implementations.rst>`_ are at its core just
a pointer and size stored together, the :dox:`Containers::StridedArrayView` is
a bit more complex beast. Based on a very insightful
`article by Per Vognsen <https://gist.github.com/pervognsen/0e1be3b683d62b16fd81381c909bf67e>`_
it recently went through a major redesign, making it multi-dimensional and
allowing for zero and negative strides. Let's see what that means.)


.. code:: pycon

    >>> from corrade import containers
    >>> a = b'heeeeeee'
    >>> b = containers.ArrayView(a)
    >>> a[2] = 'h'
    Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
    TypeError: 'bytes' object does not support item assignment
    >>> a
    b'heeeeeee'
    >>> b[2] = 'h'
    >>> a
    b'heheeeee'

`I have a bag of data and I am scared of it`_
=============================================

Now, let's say we have some unknown image data and need to see what's inside.
While it's possible to export the data to a PNG (and with the recent addition
of :dox:`DebugTools::screenshot()` it can be just an oneliner), doing so adds a
bunch of dependencies that might otherwise not be needed or available. Going to
a file manager to open the generated image also can be distracting for the
workflow if you need to watch how the image evolves over time, for example.

Graphic debuggers are out of question as well if the image lives in a CPU
memory. One useful tool is Corrade's :dox:`Utility::Debug`, which can print
container contents, so let's unleash it on a part of the image's data buffer:

.. code-figure::

    .. code:: c++

        Image2D image;

        Debug{} << image.data().prefix(300);

    .. include:: multidimensional-strided-array-views/output.ansi
        :code: shell-session
        :class: m-nopad m-console-wrap
        :start-after: [image-data]
        :end-before: [/image-data]

Um. That's not really helpful. The values are kinda low, yes, but that's about
all we are able to gather from the output. We can check that the image is
:dox:`PixelFormat::RGB8Unorm`, so let's cast the data to :dox:`Color3ub` and
try again --- :dox:`Debug <Utility::Debug>` prints them as CSS color values,
which should give us hopefully a more visual info:

.. code-figure::

    .. include:: multidimensional-strided-array-views/main.cpp
        :code: c++
        :start-after: // image-data-hexcolor
        :end-before: // /image-data-hexcolor

    .. include:: multidimensional-strided-array-views/output.ansi
        :code: shell-session
        :class: m-nopad m-console-wrap
        :start-after: [image-data-hexcolor]
        :end-before: [/image-data-hexcolor]

Okay, that's *slightly* better, but even after being 17 years in webdesign, I'm
still not able to imagine the actual color when seeing the 24bit hex value. So
let's skip the pain and print the colors *as colors* using the
:dox:`Debug::color <Utility::Debug::color()>` modifier. In addition,
:dox:`Debug::packed <Utility::Debug::packed()>` prints the container values
one after another without delimiters, which means we can pack even more
information on a screen:

.. code-figure::

    .. include:: multidimensional-strided-array-views/main.cpp
        :code: c++
        :start-after: // image-data-color
        :end-before: // /image-data-color

    .. include:: multidimensional-strided-array-views/output.ansi
        :code: ansi
        :class: m-nopad m-console-wrap
        :start-after: [image-data-color]
        :end-before: [/image-data-color]

.. block-success:: Wait, what?

    Similarly to usual colored terminal output, the
    :dox:`Debug::color <Utility::Debug::color()>` modifier (or the
    :dox:`Debug::Flag::Color <Utility::Debug::Flag::Color>` flag) prints color
    values using the `24-bit ANSI color escape codes <https://en.wikipedia.org/wiki/ANSI_escape_code#24-bit>`_
    and two successive Unicode block characters (``▓▓``) to make the pixels
    roughly square. It works with the :dox:`Color3ub` / :dox:`Color4ub` types
    as well as 8-bit unsigned ints:

    .. code-figure::

        .. include:: multidimensional-strided-array-views/main.cpp
            :code: c++
            :start-after: // wait-what
            :end-before: // /wait-what

        .. include:: multidimensional-strided-array-views/output.ansi
            :code: ansi
            :class: m-nopad
            :start-after: [wait-what]
            :end-before: [/wait-what]

    The downside is that most information gets lost while copying the terminal
    output. To preserve at least something every color is one of the five
    Unicode block shades --- try selecting & copying the above to see them.

Looking at the above output, it doesn't *seem right*. Turns out the image is
37x37 pixels and the rows are aligned to four bytes on import --- adding one
byte padding to each --- so when interpreting the data as a tightly packed
array of 24 bit values, we are off by additional byte on each successive row.

The obvious next step would be to take the data as raw bytes and print the rows
using a :cpp:`for`-cycle, incorporating the alignment. But there's not just
alignment, in general an :dox:`Image` can be a slice of a larger one, having a
custom row length, skip and other :dox:`PixelStorage` parameters. That's a lot
to handle and, I don't know about you, but when I'm figuring out a problem the
last thing I want to do is to make the problem seem even bigger with a buggy
throwaway loop that attempts to print the contents.

`Enter strided array views`_
============================

.. role:: math-primary(math)
    :class: m-primary
.. role:: math-success(math)
    :class: m-success

With a linear array of values, address :math:`a` of an item :math-primary:`i`,
with :math:`b` being the base array address, is retrieved like this:

.. math::

    a = b + {\color{m-primary} i}

With a 2D image, the addressing introduces a row length --- or row stride ---
:math-success:`s_y`:

.. math::

    a = b + {\color{m-primary} i_x} + {\color{m-success} s_y} {\color{m-primary} i_y}

If we take :math:`{\color{m-success} s_x} = 1`, the above can be rewritten like
follows, which is basically what :dox:`Containers::StridedArrayView2D` is:

.. math::

    a = b + {\color{m-success} s_x} {\color{m-primary} i_x} +
        {\color{m-success} s_y} {\color{m-primary} i_y}

Generally, with a :math:`d`-dimensional strided view, base data pointer
:math:`b`, a position vector :math-primary:`\boldsymbol{i}` and a stride vector
:math-success:`\boldsymbol{s}`, the address :math:`a` is calculated as below.
An important thing to note is that the :math-success:`\boldsymbol{s}` values
are not required to be positive --- these can be zero and (if :math:`b` gets
adjusted accordingly), negative as well. Besides that, the strides can be
shuffled to iterate in a different order. We'll see later what is it useful
for.

.. math::

    a = b + {\color{m-success} s_0} {\color{m-primary} i_0} +
        {\color{m-success} s_1} {\color{m-primary} i_1} + \ldots
      = b + \sum_{k = 0}^d {\color{m-success} s_k} {\color{m-primary} i_k}

The :dox:`Image` class (and :dox:`ImageView` / :dox:`Trade::ImageData` as well)
provides a new :dox:`pixels() <Image::pixels()>` accessor, returning a strided
:cpp:`char` view on pixel data. The view has an extra dimension compared to the
image, so for a 2D image the view is 3D, with the last dimension being bytes of
each pixel. The desired workflow is casting it to a concrete type based on
:dox:`Image::format()` before use (and getting rid of the extra dimension that
way), so we'll do just that and print the result:

.. code-figure::

    .. include:: multidimensional-strided-array-views/main.cpp
        :code: c++
        :start-after: // image-pixels
        :end-before: // /image-pixels

    .. include:: multidimensional-strided-array-views/output.ansi
        :code: ansi
        :class: m-nopad m-text m-tiny
        :start-after: [image-pixels]
        :end-before: [/image-pixels]

A multi-dimensional strided array view behaves like a view of views, so when
:dox:`Debug <Utility::Debug>` iterates over it, it gets rows, and then in each
row it gets pixels. Nested array views are delimited by a newline when printing
so we get the image nicely aligned.

The image is upside down, which explains why we were seeing the pixels mostly
black before.

`Copy-free data transformations`_
=================================

Like with normal views, the strided view can be
:dox:`slice() <Containers::StridedArrayView::slice()>`\ d. In addition it's
possible to transpose any two dimensions (swapping their sizes and strides) or
flip them (by negating the stride and adjusting the base). That can be used to
create arbitrary 90° rotations of the image --- in the following example we
take the center square and rotate it three times:

.. include:: multidimensional-strided-array-views/main.cpp
    :code: c++
    :class: m-nopadb
    :start-after: // image-center
    :end-before: // /image-center

.. container:: m-row

    .. container:: m-col-l-4 m-col-s-6 m-nopadb

        .. code-figure::

            .. include:: multidimensional-strided-array-views/main.cpp
                :code: c++
                :start-after: // image-left
                :end-before: // /image-left

            .. include:: multidimensional-strided-array-views/output.ansi
                :code: ansi
                :class: m-nopad m-text m-tiny
                :start-after: [image-left]
                :end-before: [/image-left]

    .. container:: m-col-l-4 m-push-l-4 m-col-s-6 m-nopadb

        .. code-figure::

            .. include:: multidimensional-strided-array-views/main.cpp
                :code: c++
                :start-after: // image-right
                :end-before: // /image-right

            .. include:: multidimensional-strided-array-views/output.ansi
                :code: ansi
                :class: m-nopad m-text m-tiny
                :start-after: [image-right]
                :end-before: [/image-right]

    .. container:: m-col-l-4 m-pull-l-4 m-col-s-6 m-nopadb

        .. code-figure::

            .. include:: multidimensional-strided-array-views/main.cpp
                :code: c++
                :start-after: // image-top
                :end-before: // /image-top

            .. include:: multidimensional-strided-array-views/output.ansi
                :code: ansi
                :class: m-nopad m-text m-tiny
                :start-after: [image-top]
                :end-before: [/image-top]

`Using a view for precisely aimed modifications`_
=================================================

Strided array views are by far not limited to just data viewing. Let's say we
want to add a border to the image --- three pixels on each side. The usual
approach would be to write a bunch of nested :cpp:`for` loops, one for each
side, and once we figure out all memory stomps, off-by-one and sign errors,
we'd be done --- until we realize we might want a four pixel border instead.

Let's think different. Following is a :cpp:`blit()` function that just copies
data from one image view to the other in two nested :cpp:`for` cycles,
expecting that both have the same size. This is the only loop we'll need.

.. include:: multidimensional-strided-array-views/main.cpp
    :code: c++
    :start-after: // blit
    :end-before: // /blit

Now, for the border we'll pick three colors and put them in another strided
view:

.. code-figure::

    .. include:: multidimensional-strided-array-views/main.cpp
        :code: c++
        :start-after: // border
        :end-before: // /border

    .. include:: multidimensional-strided-array-views/output.ansi
        :code: ansi
        :class: m-nopad
        :start-after: [border]
        :end-before: [/border]

`Value broadcasting`_
=====================

Nice, that's three pixels, but we need to extend those *in a loop* to span the
whole side of the image. Turns out the loop in :cpp:`blit()` can do that for us again --- if we use a zero stride. Let's expand the view to 2D and
:dox:`broadcast() <Containers::StridedArrayView::slice()>` one dimension to the
size of the image side:

.. code-figure::

    .. include:: multidimensional-strided-array-views/main.cpp
        :code: c++
        :start-after: // broadcast
        :end-before: // /broadcast

    .. include:: multidimensional-strided-array-views/output.ansi
        :code: ansi
        :class: m-nopad m-text m-tiny
        :start-after: [broadcast]
        :end-before: [/broadcast]

Not bad. Last thing is to apply it correctly rotated four times to each side of
the image:

.. code-figure::

    .. include:: multidimensional-strided-array-views/main.cpp
        :code: c++
        :start-after: // result
        :end-before: // /result

    .. include:: multidimensional-strided-array-views/output.ansi
        :code: ansi
        :class: m-nopad m-text m-tiny
        :start-after: [result]
        :end-before: [/result]

And that's it! The image now looks better and also less scary. I'd call that a
success.

`Where this gets used in Magnum`_
=================================

Apart from :dox:`Image::pixels()` and image-related operations shown above,
strided array views are used inside :dox:`Animation::TrackView` already
`since version 2018.10 <{filename}/blog/announcements/2018.10.rst>`_ --- more
often than not, you have one keyframe with multiple values (rotation *and*
translation, for example) and that's exactly where strided views are useful.

The next step is rewriting most of :dox:`MeshTools` to operate on top of
strided array views. Due to historical reasons, the APIs currently operate
mainly on :dox:`std::vector`\ s, which is *far* from ideal due to the cost of
copying and allocations when your workflow isn't heavily tied to STL. However,
accepting :dox:`Containers::ArrayView` there wouldn't make it any better ---
having vertex attributes *not* interleaved is a very rare case, so one would
usually need to copy anyway. With :dox:`Containers::StridedArrayView` the tools
can operate on *any* data --- directly on a packed GPU buffer, a linear array,
but the :dox:`std::vector` as well, thanks to the
`STL compatibility <{filename}array-view-implementations.rst#stl-compatibility>`_
of all views.

Hand-in-hand with the above goes a rework of :dox:`Trade::MeshData2D` /
:dox:`Trade::MeshData3D`, among other things making it possible to implement
fast :gh:`zero-copy importers <mosra/magnum#240>` --- memory-map a glTF binary
and have the mesh data structure describe where the vertex attributes are
directly in the file no matter how interleaved these are.

Last but not least, the strided array view implementation matches Python's
`Buffer Protocol <https://docs.python.org/3/c-api/buffer.html>`_, meaning it'll
get used in the :gh:`Magnum Python bindings <mosra/magnum#228>` that are
currently underway to allow for efficient data sharing between C++ and Python.

`std::mdspan in C++23(?)`_
==========================

.. role:: link-flat(link)
    :class: m-flat
.. |span| replace:: :link-flat:`std::span <https://en.cppreference.com/w/cpp/container/span>`

|span|, currently `scheduled for C++20 <{filename}array-view-implementations.rst#stl-compatibility>`_,
was originally meant to include multi-dimensional strided as well.
*Fortunately* that's not the case --- even without it, both compile-time-sized
and dynamic views together in a single interface are pretty complex already.
The multi-dimensional functionality is now part of a `std::mdspan`_ proposal,
with an optimistic estimate appearing in C++23. From a brief look, it should
have a *superset* of :dox:`Containers::StridedArrayView` features as it allows
the user to provide a custom data addressing function. However I was not able
to find any currently active implementation so I can'd do a deeper usability
comparison.

.. block-danger:: Strided views and strict aliasing

    Important thing to note is that neither :dox:`Containers::StridedArrayView`
    nor `std::mdspan`_ can be implemented cleanly without hitting any undefined
    behavior in current standards, due to breaking strict aliasing rules. The
    case of `std::mdspan`_ was reportedly solved by abusing a
    `"legal loophole" <https://stackoverflow.com/questions/49665881/mdspan-and-the-strict-aliasing-rule>`_
    --- the sole presence of a type in standard library means there's no
    undefined behavior in its implementation. Only time will tell if this is
    an acceptable implementation policy or the language restrictions need to
    get lifted to allow such use cases.

`Use it in your projects`_
==========================

As with other containers, :dox:`Containers::StridedArrayView` is now available
as a header-only library from the :gh:`Magnum Singles <mosra/magnum-singles>`
repository. It depends on the single-header ``CorradeArrayView.h`` instead of
packing it with itself, because if you need a strided view, you'll need a
linear view too, however grabbing the whole strided view code when all you need
is just :dox:`Containers::ArrayView` wouldn't be nice to compile times, so
these two are separate.

.. |n| replace:: :label-success:`new`

.. class:: m-table m-fullwidth

================================= ========= ===== =========================
Library                           LoC       PpLoC Description
================================= ========= ===== =========================
`CorradeArrayView.h`_             610       2484  See
                                                  :dox:`Containers::ArrayView`
                                                  and :dox:`StaticArrayView <Containers::StaticArrayView>`
                                                  docs
`CorradeStridedArrayView.h`_ |n|  594\ [1]_ 2866  See
                                                  :dox:`Containers::StridedArrayView`
                                                  docs
================================= ========= ===== =========================

.. [1] not a total size due to inter-library dependencies

.. note-dim::

    Questions? Complaints? Share your opinion on social networks:
    `Twitter <https://twitter.com/czmosra/status/1123304711967395842>`_,
    `Reddit r/cpp <https://www.reddit.com/r/cpp/comments/bj7bqk/multidimensional_strided_array_views_in_the/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=19790990>`_

.. _std::mdspan: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
.. _CorradeArrayView.h: https://github.com/mosra/magnum-singles/blob/master/CorradeArrayView.h
.. _CorradeStridedArrayView.h: https://github.com/mosra/magnum-singles/blob/master/CorradeStridedArrayView.h
