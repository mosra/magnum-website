The Unnecessarily Short Ways To Do a Quaternion Slerp
#####################################################

:date: 2018-09-05
:category: Backstage
:cover: {static}/img/blog/backstage/the-unnecessarily-short-ways-to-do-a-quaternion-slerp.jpg
:tags: C++, math, glTF, Unity 3D, Unreal Engine, Eigen, GLM, three.js,
    animation
:summary: Coming to an established workflow for the first time is always an
    interesting experience because I don't have any "it's been like this since
    forever" insights forming my opinion yet. This time it's about quaternion
    interpolation and a sudden optimization opportunity.

.. role:: cpp(code)
    :language: cpp

Traditionally, quaternion :dox:`slerp() <Math::slerp()>` in Magnum was done
like this:

.. math::

    \begin{array}{rcl}
        \theta & = & \arccos \left( \frac{q_A \cdot q_B}{|q_A| |q_B|} \right) = \arccos(q_A \cdot q_B) \\[5pt]
        q_{SLERP} & = & \cfrac{\sin((1 - t) \theta) q_A + \sin(t \theta) q_B}{\sin(\theta)}
    \end{array}

Nothing obviously wrong there, right? Or at least I thought so. While
implementing all sorts of interpolation methods for the new :dox:`Animation`
framework that's part of the upcoming Magnum release, I discovered that the
`Animated Triangle`_ glTF sample is rotating in a funny way: first it goes
three quarters of the way and then instead of finishing the circle it goes
backwards. Apparently I was :gh:`not the only one <KhronosGroup/glTF-Sample-Models#185>`
with this problem.

Having a flashback to four years ago when I was implementing a simple animation
system for 2D touchscreen UIs, the first thing that came to my mind is that the
animation data is wrong --- instead of going from 270° to 360°, the animation
requests to go from 270° to 0° and *somehow* the glTF players and importers
interpret that as going forward instead of backward and nobody questioned the
behavior until now.

Seeing the :gh:`comment <KhronosGroup/glTF-Sample-Models#185#issuecomment-405309613>`
about which viewer was used for verification of the sample, I first thought
these implementations were a bit "special" and it's not usual to do it like
this. Turns out I was wrong (of course) --- the *Shortest Path* quaternion
slerp is the common way:

.. math::

    \begin{array}{rcl}
        d & = & q_A \cdot q_B \\
        {\color{m-info} q'_A} & {\color{m-info} =} & {\color{m-info} \begin{cases}
                {\color{m-default} \phantom{-}q_A}, & d \ge 0 \\
                -q_A, & d < 0
            \end{cases} }\\[15pt]
        \theta & = & \arccos({\color{m-info}|}d{\color{m-info}|}) \\
        q_{SLERP} & = & \cfrac{\sin((1 - t) \theta) {\color{m-info} q'_A} + \sin(t \theta) q_B}{\sin(\theta)}
    \end{array}

But the seed of doubt had already been planted and so I was still unsure if
this is the right way to do it --- the shortest-path choice basically takes one
degree of freedom away from the user. Googling around, I found various people
asking how to bypass the shortest path aspect (`Unity Forum`_,
`Reddit r/Unity3D`_) and getting *extremely dense* replies along the lines of
"you are asking for the impossible", "there are infinite ways to do that" or
"go through Euler angles instead".

.. block-warning:: Spline quaternion interpolation and shortest path

    Besides the above, with animations using the `glTF cubic Hermite spline quaternion interpolation`_
    it's not possible to do any shortest-path rotation like this --- the
    interpolated rotation *has to* follow the defined spline curve. In practice
    that means switching a spline-interpolated animation track to a linear
    interpolation would totally change its behavior --- suddenly rotations
    would go the other way instead of being just less smooth.

`Give the users a choice`_
==========================

In order to prevent Magnum users from such atrocities, I decided to provide
support for both shortest-path and non-shortest-path interpolation. I named the
functions :dox:`Math::slerp()` and :dox:`Math::slerpShortestPath()` (and not :cpp:`slerp()` and :cpp:`slerpNotShortestPath()`) to suggest the one with the
longer name is doing something extra and might not be always the best choice.

Here's the status of such support in various engines I checked. Having support
for basic quaternion :abbr:`lerp <linear interpolation>` next to
:abbr:`slerp <spherical linear interpolation>` is crucial, as it's just a
vector interpolation with a renormalization step after --- a much faster
operation suited for cases where precision is not as important (such as most
animations with dense keyframes):

.. raw:: html

    <style>
      table.m-table#slerp-support th {
        vertical-align: top;
        text-align: center;
      }
      table.m-table#slerp-support td.m-danger,
      table.m-table#slerp-support td.m-success,
      table.m-table#slerp-support td.m-warning,
      table.m-table#slerp-support td.m-dim {
        vertical-align: middle;
        text-align: center;
      }
    </style>
    <table class="m-table" id="slerp-support">
      <thead><tr>
        <th></th>
        <th><abbr title="linear interpolation">lerp</abbr></th>
        <th><abbr title="spherical linear interpolation, shortest path">lerp<br/>SP</abbr></th>
        <th><abbr title="spherical linear interpolation">slerp</abbr></th>
        <th><abbr title="spherical linear interpolation, shortest path">slerp<br/>SP</abbr></th>
      </tr></thead>
      <tbody>
        <tr>
          <td>javax.vecmath <a href="#id1">[1]</a></td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-success">✔</td>
        </tr>
        <tr>
          <td>javagl <a href="#id2">[2]</a></td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-success">✔</td>
        </tr>
        <tr>
          <td>Unity 3D <a href="#id3">[3]</a></td>
          <td class="m-danger">✘</td>
          <td class="m-success">✔ </td>
          <td class="m-danger">✘</td>
          <td class="m-success">✔</td>
        </tr>
        <tr>
          <td>Unreal Engine <a href="#id4">[4]</a></td>
          <td class="m-dim">?</td>
          <td class="m-warning">•</td>
          <td class="m-success">✔</td>
          <td class="m-success">✔</td>
        </tr>
        <tr>
          <td>id Tech 4 (Doom 3) <a href="#id5">[5]</a></td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-success">✔</td>
        </tr>
        <tr>
          <td>three.js <a href="#id6">[6]</a></td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-success">✔</td>
        </tr>
        <tr>
          <td>GLM <a href="#id7">[7]</a></td>
          <td class="m-warning">•</td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-success">✔</td>
        </tr>
        <tr>
          <td>Eigen <a href="#id8">[8]</a></td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-danger">✘</td>
          <td class="m-success">✔</td>
        </tr>
        <tr>
          <td>Magnum::Math <a href="#id9">[9]</a></td>
          <td class="m-success">✔</td>
          <td class="m-success">✔</td>
          <td class="m-success">✔</td>
          <td class="m-success">✔</td>
        </tr>
      </tbody>
    </table>

.. [1] `Quat4f.interpolate() <https://github.com/hharrison/vecmath/blob/8f748b7619f1168be9fc47983ab5fcefbef3f7d8/src/javax/vecmath/Quat4f.java#L621-L625>`_,
    I was not able to find any oth­er vari­ants
.. [2] `SlerpQuaternionInterpolator.interpolate() <https://github.com/javagl/JglTF/blob/8a5128cf57e9bca5fe35ecd00dcbc23d2c540b0a/jgltf-model/src/main/java/de/javagl/jgltf/model/animation/SlerpQuaternionInterpolator.java#L50-L57>`_,
    based on the ``javax.vecmath`` implementation above. I was not able to find
    any oth­er vari­ants.
.. [3] `Quaternion.Lerp() <https://docs.unity3d.com/ScriptReference/Quaternion.Lerp.html>`_,
    `Quaternion.Slerp() <https://docs.unity3d.com/ScriptReference/Quaternion.Slerp.html>`__,
    both shortest-path. Non-shortest-path is reportedly impossible (`Unity Forum`_,
    `Reddit r/Unity3D`_).
.. [4] `FQuat::FastLerp() <https://api.unrealengine.com/INT/API/Runtime/Core/Math/FQuat/FastLerp/index.html>`_
    (shortest path but doesn't renormalize),
    `FQuat::Slerp() <https://api.unrealengine.com/INT/API/Runtime/Core/Math/FQuat/Slerp/index.html>`_
    and `FQuat::SlerpFullPath() <https://api.unrealengine.com/INT/API/Runtime/Core/Math/FQuat/SlerpFullPath/index.html>`_.
    Non-shortest-path lerp *has to be* hidden there somewhere (probably just a
    vector lerp would do that, since :cpp:`FastLerp()` also doesn't
    renormalize).
.. [5] `idQuat::Slerp() <https://github.com/id-Software/DOOM-3-BFG/blob/1caba1979589971b5ed44e315d9ead30b278d8b4/neo/idlib/math/Quat.cpp#L179-L185>`_,
    I was not able to find any other variants
.. [6] `Quaternion.slerp() <https://github.com/mrdoob/three.js/blob/c570b9bd95cf94829715b2cd3a8b128e37768a9c/src/math/Quaternion.js#L531-L538>`__,
    I was not able to find any other variants
.. [7] `glm::lerp() <https://github.com/g-truc/glm/blob/0ceb2b755fb155d593854aefe3e45d416ce153a4/glm/ext/quaternion_common.inl#L28-L38>`_
    and `glm::slerp() <https://github.com/g-truc/glm/blob/0ceb2b755fb155d593854aefe3e45d416ce153a4/glm/ext/quaternion_common.inl#L40-L73>`_,
    note that even though the name is similar, one does a shortest-path
    optimization while the other does not, leading to confusing behavior
.. [8] `Eigen::Quaternion::slerp() <https://github.com/eigenteam/eigen-git-mirror/blob/6d062f0584523e3e282cf9f62ae260e0d961f3dc/Eigen/src/Geometry/Quaternion.h#L747-L775>`_,
    the only implementation where you have to do a weird :cpp:`a.slerp(b, t)`
    instead of :cpp:`slerp(a, b, t)`. I was not able to find any other
    variants, even this one was hard to find.
.. [9] :dox:`Math::lerp()`, :dox:`Math::lerpShortestPath()`,
    :dox:`Math::slerp()`, :dox:`Math::slerpShortestPath()`

`The performance aspect`_
=========================

Besides giving the users more control, there is also the performance side of
things. While I originally didn't assume the extra branch to have a significant
effect in slerp, my thinking was that it'll definitely add something to the
basic lerp, since the dot product would not be needed at all otherwise:

.. math::

    \begin{array}{rcl}
        {\color{m-success} d} & {\color{m-success} =} & {\color{m-success} q_A \cdot q_B }\\[5pt]
        {\color{m-success} q'_A} & {\color{m-success} =} & {\color{m-success} \begin{cases}
                {\color{m-default} \phantom{-}q_A}, & d \ge 0 \\
                -q_A, & d < 0
            \end{cases} }\\[15pt]
        q_{LERP} & = & \cfrac{(1 - t) {\color{m-success} q'_A} + t q_B}{|(1 - t) {\color{m-success} q'_A} + t q_B|}
    \end{array}

To verify the above assumption, I benchmarked the :dox:`Math::lerp()`,
:dox:`Math::lerpShortestPath()`, :dox:`Math::slerp()` and
:dox:`Math::slerpShortestPath()` implementations in latest Magnum master
(:gh:`mosra/magnum@4b7dab10e1f775df7a1d499afbd297320da89a8b`). Hover over the
bars below to see precise numbers; benchmark code for reference is
`here <https://github.com/mosra/magnum/blob/4b7dab10e1f775df7a1d499afbd297320da89a8b/src/Magnum/Math/Test/InterpolationBenchmark.cpp>`__.

.. plot:: CPU time, Linux x64, GCC 8.1 -O3, Core i7 8th gen
    :type: barh
    :labels:
        baseline
        lerpShortestPath()
        lerp()
        slerpShortestPath()
        slerp()
    :labels_extra:
        benchmark overhead
        ..
        ~15% faster
        ..
        ~24% faster
    :values: 2.43 7.27 6.53 48.91 37.81
    :errors: 0.13 0.25 0.19 1.91 1.80
    :units: ns
    :colors: dim success success info info
    :bar_height: 0.6

The big difference with slerp surprised me --- I assumed the time spent by the
:math:`\arccos()` calculation would hide most of the branching overhead ---
this big difference probably points out to speculative execution done by the
CPU, where many things get calculated twice and in the end only half of them is
used.

.. note-info::

    Also, please note this is all "dumb" scalar code, nothing SIMD-optimized
    like in this `blog post by Arseny Kapoulkine <https://zeuxcg.org/2016/05/05/optimizing-slerp/>`_.
    Data-oriented vectorized batch interpolators are later on the roadmap as
    part of the :dox:`Animation` library evolution.

`Implementation in Magnum --- let's fix the data instead`_
==========================================================

In Magnum, users now have the choice to use any interpolation variant they
want. Since shortest-path interpolation is used most commonly,
:dox:`Math::slerpShortestPath()` is the default interpolator picked when you
specify :dox:`Animation::Interpolation::Linear` for quaternion tracks. That's
the least surprising behavior and if you don't like the choice, simply pass
some other interpolator function directly.

But what to do with imported animation data? Since that's where interpolation
will get used most, it would be nice to have some optimization opportunity
there too.

Turns out it's easy --- unlike the trigonometry aspects of
slerp, which are `hard to get rid of <https://zeuxcg.org/2015/07/23/approximating-slerp/>`_,
optimizing away the shortest-path flip is easy --- just patch the data on
import! (Thanks for the hint, :gh:`Squareys`!) Since
:gh:`mosra/magnum-plugins@bba82bf37e13d5515afdeec8a8621d1da3b8330f`, the
:dox:`TinyGltfImporter <Trade::TinyGltfImporter>` plugin by default patches
quaternions in linearly interpolated rotation tracks in order to always have
the shortest path from one keyframe to the other. The code that does that is
just this:

.. code:: c++

    Containers::ArrayView<Quaternion> values;
    Float flip = 1.0f;
    for(std::size_t i = 0; i < values.size() - 1; ++i) {
        if(Math::dot(values[i], values[i + 1]*flip) < 0) flip = -flip;
        values[i + 1] *= flip;
    }

Then, once you have the imported :dox:`Trade::AnimationData` instances, you can
supply a different interpolator of your choice to rotation tracks either
directly with :dox:`Animation::TrackView::at()` or by adding them to the player
using :dox:`Animation::Player::addRawCallback()`:

.. code:: c++

    Animation::TrackView<Float, Quaternion> track;
    Quaternion rotation = track.at(time, Math::slerp);

.. block-success:: Linearly interpolating spline quaternion tracks

    By passing a custom interpolator that doesn't go shortest-path it's also
    possible to safely dial down the interpolation mode of imported glTF
    :dox:`CubicHermiteQuaternion` animations from
    :dox:`Animation::Interpolation::Spline` to linear and still have the
    correct behavior, without any rotations suddenly going backwards:

    .. code:: c++

        Animation::TrackView<Float, CubicHermiteQuaternion> track;
        Quaternion rotation = track.at(time, Math::lerp);

The glTF animation import patching is :dox:`configurable with a runtime option <Trade-TinyGltfImporter-configuration>`,
so if you don't want it for some reason, simply flip the switch back to
:cpp:`false`:

.. code:: c++

    std::unique_ptr<Trade::AbstractImporter> importer =
        manager.loadAndInstantiate("TinyGltfImporter");
    importer->configuration().setValue("optimizeQuaternionShortestPath", false);

And that's it! As you have probably guessed from the above overly terse code
snippets, there's much more to say about the new :dox:`Animation` library, stay
tuned for the next posts. Thank you for reading!

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/1037416215172530177>`_,
    Reddit `r/cpp <https://www.reddit.com/r/cpp/comments/9d9ve7/optimizing_quaternion_interpolation_by_doing_less/>`_,
    `r/gamedev <https://www.reddit.com/r/gamedev/comments/9d9v85/optimizing_quaternion_interpolation_by_doing_less/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=17921048>`_,
    `mailing list <https://groups.google.com/forum/#!topic/magnum-engine/ZqrrjCpu-PQ>`_

.. _Animated Triangle: https://github.com/KhronosGroup/glTF-Sample-Models/tree/master/2.0/AnimatedTriangle
.. _Unity Forum: https://forum.unity.com/threads/by-pass-the-shortest-route-aspect-of-quaternion-slerp.459429/
.. _Reddit r/Unity3D: https://www.reddit.com/r/Unity3D/comments/3za4g2/quaternionlerp_not_rotating_properly_over_180/
.. _glTF cubic Hermite spline quaternion interpolation: https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#appendix-c-spline-interpolation
