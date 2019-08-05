// Do the following:
//
// put MagnumMath.hpp here
//
// g++ -march=native MultiplyBenchmark.cpp -o MultiplyBenchmark -std=c++17 -lCorradeTestSuite-d -lCorradeUtility-d -I/usr/include/eigen3 -I/home/mosra/Code/CxxSwizzle/include -I/home/mosra/Code/vml/vml -I/home/mosra/Code/mathfu/include -I/home/mosra/Code/mathfu/dependencies/vectorial/include -I/home/mosra/Code/rtm/includes -fno-operator-names
//
// or -O3 to the above
//
// ./MultiplyBenchmark --color on # until it gets white
//
//
// /opt/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ MultiplyBenchmark.cpp -o MultiplyBenchmark.arm64 --target=aarch64-none-linux-android --gcc-toolchain=/opt/android-ndk/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64 --sysroot=/opt/android-ndk/platforms/android-22/arch-arm64 -no-canonical-prefixes -std=c++17 -funwind-tables -no-canonical-prefixes -D__ANDROID_API__=22 -fexceptions -frtti -DNDEBUG -fPIE -pie -Wl,--gc-sections /opt/android-ndk/platforms/android-22/arch-arm64/usr/lib/libCorradeTestSuite.a /opt/android-ndk/platforms/android-22/arch-arm64/usr/lib/libCorradeUtility.a -llog "/opt/android-ndk/sources/cxx-stl/llvm-libc++/libs/arm64-v8a/libc++_static.a" "/opt/android-ndk/sources/cxx-stl/llvm-libc++/libs/arm64-v8a/libc++abi.a" -latomic -nostdlib++ -isystem /opt/android-ndk/sources/cxx-stl/llvm-libc++/include -isystem /opt/android-ndk/sources/android/support/include -isystem /opt/android-ndk/sources/cxx-stl/llvm-libc++abi/include -isystem /opt/android-ndk/sysroot/usr/include -isystem /opt/android-ndk/sysroot/usr/include/aarch64-linux-android -I/usr/include/eigen3 -I/home/mosra/Code/glm -I/home/mosra/Code/CxxSwizzle/include -I/home/mosra/Code/vml/vml -I/home/mosra/Code/mathfu/include -I/home/mosra/Code/mathfu/dependencies/vectorial/include -I/home/mosra/Code/rtm/includes -fno-operator-names
//
// or -O3 to above
//
// adb push MultiplyBenchmark.arm64 /data/local/tmp/ && adb shell /data/local/tmp/MultiplyBenchmark.arm64 --repeat-every 1 --color on --repeat-all 5
//
//
// em++ MultiplyBenchmark.cpp -o MultiplyBenchmark.js -O3 --llvm-lto 1 -std=c++17 /usr/lib/emscripten/system/lib/CorradeTestSuite.bc /usr/lib/emscripten/system/lib/CorradeUtility.bc -I/usr/include/eigen3 -I/home/mosra/Code/glm -I/home/mosra/Code/CxxSwizzle/include -I/home/mosra/Code/vml/vml -I/home/mosra/Code/mathfu/include -I/home/mosra/Code/mathfu/dependencies/vectorial/include -I/home/mosra/Code/rtm/includes -fno-operator-names

#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
#include <Eigen/Core>
#include <Eigen/LU>
#else
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/LU>
#include <immintrin.h>
#include <boost/qvm/all.hpp>
#endif
// #define GLM_FORCE_PURE breaks all the things, DON'T
#define GLM_FORCE_CTOR_INIT
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#define MAGNUM_MATH_EIGEN_INTEGRATION
#define MAGNUM_MATH_GLM_INTEGRATION
#include "MagnumMath.hpp"

// THIS HAS TO BE BELOW magnumMath.hpp because it defines asserts!!
#include <Corrade/Utility/Debug.h>
#include <Corrade/TestSuite/Tester.h>

#include "linalg.h"

/* stupid what THE FUCK */
#include <swizzle/detail/vector_traits.h>
#include <swizzle/glsl/matrix.h>
#include <swizzle/glsl/vector.h>
#include <swizzle/glsl/vector_functions.h>
#include <swizzle/glsl/scalar_support.h>

/* this is ... uhhh ... vml I think? */
#include <vector.h>
#include <matrix.h>
#include <vector_functions.h>

/* mathfu */
#include "mathfu/matrix.h"

/* Real-Time Math */
#include <rtm/matrix3x3f.h>
#include <rtm/matrix4x4f.h>

using namespace Corrade;
using namespace Magnum;
using namespace Magnum::Math::Literals;

struct MultiplyBenchmark: TestSuite::Tester {
    explicit MultiplyBenchmark();

    void magnumMath4();
    void linalg4();
    void cxxSwizzle4();
    void vml4();
    void glm4();
    void eigen4();
    #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
    void boostQvm4();
    #endif
    void mathfu4();
    void rtm4();
    #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
    void sse4();
    #endif

    void magnumMath3();
    void linalg3();
    void cxxSwizzle3();
    void vml3();
    void glm3();
    void eigen3();
    #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
    void boostQvm3();
    #endif
    void mathfu3();
    void rtm3();
    #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
    void sse3();
    #endif

    Matrix4 data4 = Matrix4::rotation(134.7_degf, Vector3{1.0f, 3.0f, -1.4f}.normalized())*Matrix4::translation(Vector3::zAxis());
    Matrix3 data3 = Matrix3::rotation(134.7_degf)*Matrix3::translation(Vector2::yAxis());
};

MultiplyBenchmark::MultiplyBenchmark() {
    addBenchmarks({
        &MultiplyBenchmark::magnumMath4,
        &MultiplyBenchmark::linalg4,
        &MultiplyBenchmark::cxxSwizzle4,
        &MultiplyBenchmark::vml4,
        &MultiplyBenchmark::glm4,
        &MultiplyBenchmark::eigen4,
        #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
        &MultiplyBenchmark::boostQvm4,
        #endif
        &MultiplyBenchmark::mathfu4,
        &MultiplyBenchmark::rtm4,
        #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
        &MultiplyBenchmark::sse4,
        #endif

        &MultiplyBenchmark::magnumMath3,
        &MultiplyBenchmark::linalg3,
        &MultiplyBenchmark::cxxSwizzle3,
        &MultiplyBenchmark::vml3,
        &MultiplyBenchmark::glm3,
        &MultiplyBenchmark::eigen3,
        #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
        &MultiplyBenchmark::boostQvm3,
        #endif
        &MultiplyBenchmark::mathfu3,
        &MultiplyBenchmark::rtm3,
        #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
        &MultiplyBenchmark::sse3,
        #endif

    }, 50);
}

enum: std::size_t { Repeats = 10000 };

void MultiplyBenchmark::magnumMath4() {
    Matrix4 a = data4;
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(a.toVector().sum() != 0);
}

void MultiplyBenchmark::linalg4() {
    linalg::mat<float, 4, 4> a;
    Matrix4::from(&a[0][0]) = data4;

    auto aa = linalg::mul(a, a);
    CORRADE_VERIFY(Matrix4::from(&aa[0][0]) == data4*data4);
    CORRADE_BENCHMARK(Repeats) {
        a = linalg::mul(a, a);
    }

    CORRADE_VERIFY(Matrix4::from(&a[0][0]).toVector().sum() != 0);
}

void MultiplyBenchmark::cxxSwizzle4() {
    swizzle::glsl::matrix<swizzle::glsl::vector, float, 4, 4> a;
    Matrix4::from(&a[0][0]) = data4;

    auto aa = a*a;
    CORRADE_VERIFY(Matrix4::from(&aa[0][0]) == data4*data4);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix4::from(&a[0][0]).toVector().sum() != 0);
}

void MultiplyBenchmark::vml4() {
    vml::matrix<float, vml::vector, vml::indices_pack<0, 1, 2, 3>, vml::indices_pack<0, 1, 2, 3>> a;
    Matrix4::from(&a[0][0]) = data4;

    auto aa = a*a;
    CORRADE_VERIFY(Matrix4::from(&aa[0][0]) == data4*data4);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix4::from(&a[0][0]).toVector().sum() != 0);
}

void MultiplyBenchmark::glm4() {
    glm::mat4 a(data4);

    CORRADE_VERIFY(Matrix4(a*a) == data4*data4);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix4{a}.toVector().sum() != 0);
}

void MultiplyBenchmark::eigen4() {
    Eigen::Matrix4f a = EigenIntegration::eigenCast<Eigen::Matrix4f>(data4);

    CORRADE_VERIFY(Matrix4(Eigen::Matrix4f(a*a)) == data4*data4);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix4{a}.toVector().sum() != 0);
}

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
void MultiplyBenchmark::boostQvm4() {
    boost::qvm::mat<float, 4, 4> a;
    Matrix4::from(&A00(a)) = data4;

    auto aa = a*a;
    CORRADE_VERIFY(Matrix4::from(&A00(aa)) == data4*data4);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix4::from(&A00(a)).toVector().sum() != 0);
}
#endif

void MultiplyBenchmark::mathfu4() {
    mathfu::Matrix<float, 4, 4> a;
    Matrix4::from(&a(0, 0)) = data4;

    auto aa = a*a;
    CORRADE_VERIFY(Matrix4::from(&aa(0, 0)) == data4*data4);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix4::from(&a(0, 0)).toVector().sum() != 0);
}

void MultiplyBenchmark::rtm4() {
    rtm::matrix4x4f a;
    Matrix4::from(&a.x_axis[0]) = data4;

    auto aa = rtm::matrix_mul(a, a);
//     CORRADE_VERIFY(Matrix4::from(&a.x_axis[0]) == data4*data4);
    CORRADE_BENCHMARK(Repeats) {
        a = rtm::matrix_mul(a, a);
    }

    CORRADE_VERIFY(Matrix4::from(&a.x_axis[0]).toVector().sum() != 0);
}

void MultiplyBenchmark::sse4() {

// https://codereview.stackexchange.com/a/211696, adapted

typedef float float32;

class Matrix4x4
{
public:
float32 m11, m21, m31, m41,
        m12, m22, m32, m42,
        m13, m23, m33, m43,
        m14, m24, m34, m44;

void multiply(Matrix4x4 ma) {

    __m128 col1 = _mm_loadu_ps(&m11);
    __m128 col2 = _mm_loadu_ps(&m12);
    __m128 col3 = _mm_loadu_ps(&m13);
    __m128 col4 = _mm_loadu_ps(&m14);
    __m128 t1 = _mm_add_ps(_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(col1, _mm_set1_ps(ma.m11)),
        _mm_mul_ps(col2, _mm_set1_ps(ma.m21))),
        _mm_mul_ps(col3, _mm_set1_ps(ma.m31))),
        _mm_mul_ps(col4, _mm_set1_ps(ma.m41)));
    __m128 t2 = _mm_add_ps(_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(col1, _mm_set1_ps(ma.m12)),
        _mm_mul_ps(col2, _mm_set1_ps(ma.m22))),
        _mm_mul_ps(col3, _mm_set1_ps(ma.m32))),
        _mm_mul_ps(col4, _mm_set1_ps(ma.m42)));
    __m128 t3 = _mm_add_ps(_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(col1, _mm_set1_ps(ma.m13)),
        _mm_mul_ps(col2, _mm_set1_ps(ma.m23))),
        _mm_mul_ps(col3, _mm_set1_ps(ma.m33))),
        _mm_mul_ps(col4, _mm_set1_ps(ma.m43)));
    __m128 t4 = _mm_add_ps(_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(col1, _mm_set1_ps(ma.m14)),
        _mm_mul_ps(col2, _mm_set1_ps(ma.m24))),
        _mm_mul_ps(col3, _mm_set1_ps(ma.m34))),
        _mm_mul_ps(col4, _mm_set1_ps(ma.m44)));

    _mm_storeu_ps(&m11, t1);
    _mm_storeu_ps(&m12, t2);
    _mm_storeu_ps(&m13, t3);
    _mm_storeu_ps(&m14, t4);
}
};

    Matrix4x4 a;
    Matrix4::from(&a.m11) = data4;

    a.multiply(a);
    CORRADE_VERIFY(Matrix4::from(&a.m11) == data4*data4);
    CORRADE_BENCHMARK(Repeats) {
        a.multiply(a);
    }

    CORRADE_VERIFY(Matrix4::from(&a.m11).toVector().sum() != 0);

}

void MultiplyBenchmark::magnumMath3() {
    Matrix3 a = data3;
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(a.toVector().sum() != 0);
}

void MultiplyBenchmark::linalg3() {
    linalg::mat<float, 3, 3> a;
    Matrix3::from(&a[0][0]) = data3;

    auto aa = linalg::mul(a, a);
    CORRADE_VERIFY(Matrix3::from(&aa[0][0]) == data3*data3);
    CORRADE_BENCHMARK(Repeats) {
        a = linalg::mul(a, a);
    }

    CORRADE_VERIFY(Matrix3::from(&a[0][0]).toVector().sum() != 0);
}

void MultiplyBenchmark::cxxSwizzle3() {
    swizzle::glsl::matrix<swizzle::glsl::vector, float, 3, 3> a;
    Matrix3::from(&a[0][0]) = data3;

    auto aa = a*a;
    CORRADE_VERIFY(Matrix3::from(&aa[0][0]) == data3*data3);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix3::from(&a[0][0]).toVector().sum() != 0);
}

void MultiplyBenchmark::vml3() {
    vml::matrix<float, vml::vector, vml::indices_pack<0, 1, 2>, vml::indices_pack<0, 1, 2>> a;
    Matrix3::from(&a[0][0]) = data3;

    auto aa = a*a;
    CORRADE_VERIFY(Matrix3::from(&aa[0][0]) == data3*data3);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix3::from(&a[0][0]).toVector().sum() != 0);
}

void MultiplyBenchmark::glm3() {
    glm::mat3 a(data3);

    CORRADE_VERIFY(Matrix3(a*a) == data3*data3);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix3{a}.toVector().sum() != 0);
}

void MultiplyBenchmark::eigen3() {
    Eigen::Matrix3f a = EigenIntegration::eigenCast<Eigen::Matrix3f>(data3);

    CORRADE_VERIFY(Matrix3(Eigen::Matrix3f(a*a)) == data3*data3);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix3{a}.toVector().sum() != 0);
}

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
void MultiplyBenchmark::boostQvm3() {
    boost::qvm::mat<float, 3, 3> a;
    Matrix3::from(&A00(a)) = data3;

    auto aa = a*a;
    CORRADE_VERIFY(Matrix3::from(&A00(aa)) == data3*data3);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix3::from(&A00(a)).toVector().sum() != 0);
}
#endif

void MultiplyBenchmark::mathfu3() {
    mathfu::Matrix<float, 3, 3> a;
    Matrix3::from(&a(0, 0)) = data3;

    auto aa = a*a;
//     CORRADE_VERIFY(Matrix3::from(&aa(0, 0)) == data3*data3);
    CORRADE_BENCHMARK(Repeats) {
        a = a*a;
    }

    CORRADE_VERIFY(Matrix3::from(&a(0, 0)).toVector().sum() != 0);
}

void MultiplyBenchmark::rtm3() {
    rtm::matrix3x3f a;
    Vector3::from(&a.x_axis[0]) = data3[0];
    Vector3::from(&a.y_axis[0]) = data3[1];
    Vector3::from(&a.z_axis[0]) = data3[2];

    auto aa = rtm::matrix_mul(a, a);
//     CORRADE_VERIFY(Vector3::from(&a.x_axis[0]) == (data3*data3)[0]);
//     CORRADE_VERIFY(Vector3::from(&a.y_axis[0]) == (data3*data3)[1]);
//     CORRADE_VERIFY(Vector3::from(&a.z_axis[0]) == (data3*data3)[2]);
    CORRADE_BENCHMARK(Repeats) {
        a = rtm::matrix_mul(a, a);
    }

    CORRADE_VERIFY(Matrix3::from(&a.x_axis[0]).toVector().sum() != 0);
}

void MultiplyBenchmark::sse3() {

// https://codereview.stackexchange.com/a/211696

typedef float float32;

class Matrix3x3
{
public:
float32 m11, m21, m31,
        m12, m22, m32,
        m13, m23, m33, padding;

void multiply(Matrix3x3 ma) {

    __m128 col1 = _mm_loadu_ps(&m11);
    __m128 col2 = _mm_loadu_ps(&m12);
    __m128 col3 = _mm_loadu_ps(&m13);
    __m128 t1 = _mm_add_ps(_mm_add_ps(
        _mm_mul_ps(col1, _mm_set1_ps(ma.m11)),
        _mm_mul_ps(col2, _mm_set1_ps(ma.m21))),
        _mm_mul_ps(col3, _mm_set1_ps(ma.m31)));
    __m128 t2 = _mm_add_ps(_mm_add_ps(
        _mm_mul_ps(col1, _mm_set1_ps(ma.m12)),
        _mm_mul_ps(col2, _mm_set1_ps(ma.m22))),
        _mm_mul_ps(col3, _mm_set1_ps(ma.m32)));
    __m128 t3 = _mm_add_ps(_mm_add_ps(
        _mm_mul_ps(col1, _mm_set1_ps(ma.m13)),
        _mm_mul_ps(col2, _mm_set1_ps(ma.m23))),
        _mm_mul_ps(col3, _mm_set1_ps(ma.m33)));

    _mm_storeu_ps(&m11, t1);
    _mm_storeu_ps(&m12, t2);
    _mm_storeu_ps(&m13, t3);
}
};

    Matrix3x3 a;
    Matrix3::from(&a.m11) = data3;

    a.multiply(a);
    CORRADE_VERIFY(Matrix3::from(&a.m11) == data3*data3);
    CORRADE_BENCHMARK(Repeats) {
        a.multiply(a);
    }

    CORRADE_VERIFY(Matrix3::from(&a.m11).toVector().sum() != 0);

}

CORRADE_TEST_MAIN(MultiplyBenchmark)
