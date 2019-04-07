Introducing single-header math
##############################

:date: 2019-03-25
:category: Backstage
:tags: C++
:summary: If you include ``<cmath>`` in C++17, it drags in 10 000 lines
    containing stuff you probably :abbr:`won't ever need <the Riemann zeta function>`.
    Wouldn't it be wonderful if you got some actually useful stuff in a
    similarly sized package?

.. TODO: tags
.. TODO: feature table check rigid etc. inverse
.. TODO: check everything with clang -ftime-trace

.. TODO: annoyances:

-   linalg * is componentwise, weird
-   cxxswizzle requires me to include all its headers manually, WTF, also -fno-operator-names
-   vml has crazy type definitions (and non-hermetic headers, ugh)
-   boost:qvm has inline fully unrolled code for all operations on 1/2/3/4-component vec/mat types, machine-generated, can't beat that
-   mathfu calling home

.. TODO: https://github.com/HandmadeMath/Handmade-Math !!!!!!!!
.. TODO: compare with https://github.com/nfrechette/rtm !!
.. todo: comparison with https://github.com/sgorsten/linalg !!
.. with https://github.com/valentingalea/vml, https://github.com/gwiazdorrr/CxxSwizzle
.. todo: comparison with glm, glm+swizzle, eigen, boost::geometry (heh)
.. also https://github.com/google/mathfu
.. and https://www.edwardrosten.com/cvd/toon.html
.. https://github.com/glampert/vectormath
.. comparison table showing main areas (simd, speed, but also size, compile times, license, single-header)
.. eigen ~~doesn't have bit vectors~~, duals, dual quats, ... splines are in unsupported, no AABB ranges or colors ... go through all math classes and check

.. a section named "I'm still not convinced", showing the integration libs

.. a section named "What changes for me as a Magnum user", explaining that nothing changes


.. mention accident-avoiding design decisions: quaternion always accepts vec+scalar, not 4 SCALARS
.. also obvious stuff like not allowing a matrix/vector to be created from a different amount of elements *at compile time* (eigen doesn't have that)

.. bx math? eww, still baby steps: https://github.com/bkaradzic/bx/commit/32ac9ad461f4e37e2a6c0ea0e933ceec2dc937c0

Debug performance
=================

.. mention that converting op[] calls to raw array access makes it 3x faster

.. before:
..
.. Starting MultiplyBenchmark with 7 test cases...
..  BENCH [1] 530.11 ± 95.84  ns magnumMath()@499x1000 (wall time)
..  BENCH [2] 617.14 ± 31.58  ns linalg()@499x1000 (wall time)
..  BENCH [3]   7.56 ± 0.14   µs cxxSwizzle()@499x1000 (wall time)
..  BENCH [4] 492.55 ± 33.45  ns vml()@499x1000 (wall time)
..  BENCH [5] 873.61 ± 28.24  ns glm()@499x1000 (wall time)
..  BENCH [6] 810.49 ± 33.34  ns eigen()@499x1000 (wall time)
..  BENCH [7]  44.79 ± 4.62   ns boostQvm()@499x1000 (wall time)
.. Finished MultiplyBenchmark with 0 errors out of 6500 checks.

.. after:

.. Starting MultiplyBenchmark with 7 test cases...
..  BENCH [1] 187.07 ± 17.14  ns magnumMath()@499x1000 (wall time)
..  BENCH [2] 615.12 ± 29.29  ns linalg()@499x1000 (wall time)
..  BENCH [3]   7.58 ± 0.18   µs cxxSwizzle()@499x1000 (wall time)
..  BENCH [4] 497.74 ± 28.63  ns vml()@499x1000 (wall time)
..  BENCH [5] 882.40 ± 40.05  ns glm()@499x1000 (wall time)
..  BENCH [6] 810.07 ± 30.67  ns eigen()@499x1000 (wall time)
..  BENCH [7]  44.83 ± 4.71   ns boostQvm()@499x1000 (wall time)
.. Finished MultiplyBenchmark with 0 errors out of 6500 checks.

.. invert before:

.. Starting InvertBenchmark with 9 test cases...
..  BENCH [1]   5.62 ± 0.14   µs magnumMath()@499x1000 (wall time)
..  BENCH [2] 811.06 ± 61.10  ns magnumMathGaussJordan()@499x1000 (wall time)
..  BENCH [3] 236.21 ± 12.74  ns linalg()@499x1000 (wall time)
..   SKIP [4] cxxSwizzle()@1
..         No such thing here.
..   SKIP [5] vml()@1
..         No such thing here.
..  BENCH [6]   1.70 ± 0.04   µs glm()@499x1000 (wall time)
..  BENCH [7]  11.15 ± 0.29   µs eigen()@499x1000 (wall time)
..  BENCH [8]  90.68 ± 14.86  ns boostQvm()@499x1000 (wall time)
..  BENCH [9]  92.73 ± 15.74  ns sse()@499x1000 (wall time)
.. Finished InvertBenchmark with 0 errors out of 4000 checks.

.. invert after:

.. Starting InvertBenchmark with 9 test cases...
..  BENCH [1]   2.83 ± 0.07   µs magnumMath()@499x1000 (wall time)
..  BENCH [2] 800.65 ± 35.02  ns magnumMathGaussJordan()@499x1000 (wall time)
..  BENCH [3] 229.74 ± 19.59  ns linalg()@499x1000 (wall time)
..   SKIP [4] cxxSwizzle()@1
..         No such thing here.
..   SKIP [5] vml()@1
..         No such thing here.
..  BENCH [6]   1.68 ± 0.06   µs glm()@499x1000 (wall time)
..  BENCH [7]  11.12 ± 0.19   µs eigen()@499x1000 (wall time)
..  BENCH [8]  83.98 ± 12.57  ns boostQvm()@499x1000 (wall time)
..  BENCH [9]  87.18 ± 12.87  ns sse()@499x1000 (wall time)
.. Finished InvertBenchmark with 0 errors out of 4000 checks.


Compile time
============

.. code:: sh

    echo "#include <cmath>" | gcc -std=c++11 -P -E -x c++ - | wc -l

.. plot:: Preprocessed line count, GCC 8.2, C++11
    :type: barh
    :labels:
        cmath
        MagnumMath.hpp
        linalg.h
        glm/glm.hpp
        glm/glm.hpp
        Eigen/Core
        boost/geometry.hpp
    :labels_extra:
        ..
        ..
        ..
        GLM_FORCE_SWIZZLE
        ..
        ..
    :units: lines
    :values: 2929 9404 15270 50695 50945 56023 196593
    :colors: default success warning warning warning danger

.. plot:: Preprocessed line count, GCC 8.2, C++17
    :type: barh
    :labels:
        cmath
        MagnumMath.hpp
        linalg.h
        glm/glm.hpp
        glm/glm.hpp
        Eigen/Core
        boost/geometry.hpp
    :labels_extra:
        ..
        ..
        ..
        GLM_FORCE_SWIZZLE
        ..
        ..
    :units: lines
    :values: 11313 9984 20231 56601 56851 70616 209538
    :colors: default success warning warning warning danger

.. plot:: Compilation time — instantiating a type, GCC 8.2, C++11
    :type: barh
    :labels:
        baseline
        <cmath>
        Math::Vector2i
        glm::ivec2
        glm::ivec2
        Eigen::Vector2i
        boost::geom…::point
    :labels_extra:
        int main() {}
        int a[2];
        ..
        ..
        GLM_FORCE_SWIZZLE
        ..
        ..
    :units: ms
    :values: 50.45 78.56 169.83 464.84 2800 517.59 2080
    :errors: 3.06 3.72 8.07 18.32 90 4.10 110
    :colors: default default success warning danger warning danger

.. plot:: Compilation time — instantiating a type, GCC 8.2, C++17
    :type: barh
    :labels:
        baseline
        <cmath>
        Math::Vector2i
        glm::ivec2
        glm::ivec2
        Eigen::Vector2i
        boost::geom…::point
    :labels_extra:
        int main() {}
        int a[2];
        ..
        ..
        GLM_FORCE_SWIZZLE
        ..
        ..
    :units: ms
    :values: 50.78 147.31 173.56 511.88 3270 689.44 2260
    :errors: 6.57 27.15 5.79 7.21 660 9.59 60
    :colors: default warning success warning danger warning danger
