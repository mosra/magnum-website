Teaching VR with Magnum
#######################

:date: 2019-03-11
:modified: 2019-04-09
:author: Markus Wiedemann
:category: Guest Posts
:tags: C++, rendering, OpenGL, Vcpkg, PBR, VR
:cover: {static}/img/blog/guest-posts/teaching-vr-with-magnum/voxelizer.jpg
:summary: Last year, Magnum was used to introduce students to virtual reality
    programming at the Ludwig-Maximilians-Universität München --- powering a
    `CAVE <https://en.wikipedia.org/wiki/Cave_automatic_virtual_environment>`_-like
    environment, a room-scale five-sided projection installation.

.. |wink| replace:: 😉
.. role:: cpp(code)
    :language: cpp

.. note-success:: Update: Apr 09, 2019

    Updated project links at the bottom with sources for two more games.

Teaching about virtual reality always benefits from practical experience. For
that, a big part of the lecture Virtual Reality, hosted at the
Ludwig-Maximilians-Universität München, open for students of both Munich
Universities, is to get hands on into their own VR project. This project has to
run on the facilities of the Virtual Reality and Visualisation Centre (V2C) of
the Leibniz Supercomputing Centre (Leibniz-Rechenzentrum, LRZ) of the Bavarian
Academy of Science and Humanities.\ [1]_ Here, students use the following
`five-sided projection installation <https://www.lrz.de/services/v2c_en/installations_en/>`_
for their own project.

.. container:: m-container-inflate

    .. figure:: {static}/img/blog/guest-posts/teaching-vr-with-magnum/fivesided.jpg
        :figclass: m-flat
        :alt: Five-sided projection installation
        :target: {static}/img/blog/guest-posts/teaching-vr-with-magnum/fivesided.jpg
        :width: 600px

        ..

        .. class:: m-text m-dim m-small

        Image credit: Elisabeth Mayer

Having past experience with `OpenSG <https://en.wikipedia.org/wiki/OpenSG>`_,
last year we decided to use Magnum as the graphics engine for the students to
start with --- and we will stick to it for this year as well. We were lucky to
get Vladimír to visit us in Munich and give an introductory lecture on how to
use Magnum. In this lecture, the students got a general overview of how Magnum
is structured and learned how to create their own rendering application using
the scene graph, the model loading features and the input event system.

Although this yielded a great starting point, our five-sided projection
installation requires that you code for a setup that consists of 10 nodes that
render and one node that works as a master node. For that, we provide our own
small library that helps to keep one application on those 10 nodes in sync.

.. image-grid::

    {static}/img/blog/guest-posts/teaching-vr-with-magnum/bowling-god.jpg Bowling God by Sebastian Peralta Friedburg
    {static}/img/blog/guest-posts/teaching-vr-with-magnum/we-arrr-pirates.jpg We Arrr Pirates by Yannick Weiss

The application must be built as two executables --- one for rendering, which
runs on the render nodes, and one that runs on the master node. We have for
each a distinct class with a different set of features. The master node works
as a synchronization server. That means data is distributed from here to each
of the render nodes. This data can be tracking data, transformations from
interactions or physic simulation as well as all other kinds of inputs or
dynamics. For the render nodes, the master node also functions as a
synchronization point for buffer swaps. This means that after all rendering is
finished each node sends a request to the master node and waits until it gets a
response. The master node itself does only respond if all pre-determined nodes
have sent a request. This way we can be certain that each node is rendering the
same scene with the same input data.

The synchronization library itself is based on
`Boost.Asio <https://www.boost.org/doc/libs/1_65_1/doc/html/boost_asio.html>`_
which provides an abstraction layer to UDP and TCP networking. Based on
this we further implemented so called ``synchObject``\ s that can hold a fixed
size generic data type. Those ``synchObject``\ s allow to set data on the
master node and to synchronize them with all the pre-defined render nodes using
udp multicast.

In hindsight, the biggest challenge for us was to provide the students with a
working starting package, comprised of a Magnum example that works on our
installations but also gives the students the possibility to develop on their
own computers, which might use Windows, a Linux derivate or macOS. For this, it
really helped that Magnum is in `Homebrew <https://brew.sh/>`_, allows you to
use `Vcpkg <https://github.com/Microsoft/vcpkg>`_ or to build your own Ubuntu
package.

.. image-grid::

    {static}/img/blog/guest-posts/teaching-vr-with-magnum/voxelizer.jpg Voxelizer by Constantin Geier

Our students could easily start creating their own projects using Magnum. The sheer amount of possibilities and the size of documentation was both an
advantage and a challenge. A small example for this might be getting the
rotation matrix of a transformation --- while one might think using the
function :dox:`rotation() <Math::Matrix4::rotation()>` as in

.. code:: c++

    Matrix4 transformationMatrix;
    Matrix3x3 rotMatrix = transformationMatrix.rotation();

.. class:: m-noindent

is the obvious choice, consulting the documentation gives you the note that
this function asserts that the transformation is not skewed in any way and so
you might rather consider using the function
:dox:`rotationScaling() <Math::Matrix4::rotation()>` as in

.. code:: c++
    :class: m-inverted
    :hl_lines: 2

    Matrix4 transformationMatrix;
    Matrix3x3 rotMatrix = transformationMatrix.rotationScaling();

.. class:: m-noindent

However, this also teaches a valuable lesson --- read the documentation!

What our students really liked were all the debugging functionalities,
especially the :dox:`Debug <Utility::Debug>` class that supports using
:cpp:`Debug{} << myData;` for almost any type. Furthermore, having the
:dox:`Primitives` library, the built-in :dox:`Shaders` as well as the
:dox:`GL` abstraction layer that does most of the tedious work for you allowed
our students to quickly develop their own applications and try out different
approaches.

.. image-grid::

    {static}/img/blog/guest-posts/teaching-vr-with-magnum/time-gun1.jpg TimeGunVR by Konstantin Gomm and Sergey Mitchenko
    {static}/img/blog/guest-posts/teaching-vr-with-magnum/time-gun2.jpg TimeGunVR by Konstantin Gomm and Sergey Mitchenko

Overall, we are very excited about all the different projects our students
developed with Magnum. The photos presented above are from our
`Open Lab Day <https://www.lrz.de/services/termine/vr-openlab/2018_openlab_vortraege/>`_
in December 2018 where our students presented their work to family, friends and
the public.

.. block-info:: Project sources

    With a permission from the students involved, we'll be gradually publishing
    sources to projects mentioned in the above link --- feel free to dig in
    and see how they were made. Here's the first one, with more to follow:

    -   `TimeGunVR <https://gitlab.com/TimeGunVR>`_ by Konstantin Gomm and
        Sergey Mitchenko
    -   `Bowling God <https://gitlab.lrz.de/wiedemann/vorlesung_vr_projekte/tree/master/sebastian_peralta_friedburg_bowling_god>`_
        by Sebastian Peralta Friedburg
    -   `We Arrr Pirates <https://gitlab.lrz.de/wiedemann/vorlesung_vr_projekte/tree/master/yannick_weiss_we_arrr_pirates>`_
        by Yannick Weiss

    Please note the projects are primarily built for the CAVE environment and
    thus can't run without modifications. The code is also provided as-is,
    without any warranty, and the students have no obligation to answer
    questions or fix bugs |wink|

.. [1] The V2C at LRZ offers modern technologies for visualising scientific
    data. This allows for a more rapid advancement and significant enrichment
    of scientific knowledge. The ability of scientists to understand their data
    and discover new interrelations in them is vastly improved by the three
    dimensional, high-resolution data projection on the Powerwall, by the use
    of the five-sided projection installation, and by the interactive
    navigation provided in the V2C leading to breakthroughs in understanding
    and interpreting results. In addition to being used in the natural sciences
    and in technology, complex datasets in humanities and social science
    research are also visualised using the V2C, for instance in the fields of
    arts and multimedia, archaeology, and architecture. A new LED-based
    Powerwall extends the V2C capabilities with the latest technology.

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/1105145615615438848>`_,
    Reddit `r/gamedev <https://www.reddit.com/r/gamedev/comments/azvjso/teaching_vr_with_magnum_engine/>`_,
    `r/virtualreality <https://www.reddit.com/r/virtualreality/comments/azvkmb/teaching_vr_with_magnum_engine/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=19360377>`_,
    `mailing list <https://groups.google.com/d/topic/magnum-engine/I_mb4DiC6CU>`_
