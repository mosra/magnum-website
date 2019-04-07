// Do the following:
//
// put MagnumMath.hpp here
//
// g++ -march=native InvertBenchmark.cpp -o InvertBenchmark -std=c++11 -lCorradeTestSuite-d -lCorradeUtility-d -I/usr/include/eigen3 -I/home/mosra/Code/mathfu/include -I/home/mosra/Code/mathfu/dependencies/vectorial/include
//
// or -O3 to the above
//
// ./InvertBenchmark --color on # until it gets white
//
//
// em++ InvertBenchmark.cpp -o InvertBenchmark.js -O3 --llvm-lto 1 -std=c++11 /usr/lib/emscripten/system/lib/CorradeTestSuite.bc /usr/lib/emscripten/system/lib/CorradeUtility.bc -I/usr/include/eigen3 -I/home/mosra/Code/glm -I/home/mosra/Code/mathfu/include -I/home/mosra/Code/mathfu/dependencies/vectorial/include
//
// node InvertBenchmark.js

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
#define CORRADE_NO_ASSERT
#define MAGNUM_MATH_EIGEN_INTEGRATION
#define MAGNUM_MATH_GLM_INTEGRATION
#include "MagnumMath.hpp"

// THIS HAS TO BE BELOW magnumMath.hpp because it defines asserts!!
#include <Corrade/Utility/Debug.h>
#include <Corrade/TestSuite/Tester.h>

#include "linalg.h"

/* mathfu */
#include "mathfu/matrix.h"

using namespace Corrade;
using namespace Magnum;
using namespace Magnum::Math::Literals;

struct InvertBenchmark: TestSuite::Tester {
    explicit InvertBenchmark();

    void magnumMath4();
    void magnumMathGaussJordan4();
    void linalg4();
    void glm4();
    void eigen4();
    #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
    void boostQvm4();
    #endif
    void mathfu4();
    #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
    void sse4();
    #endif

    void magnumMath3();
    void magnumMathGaussJordan3();
    void linalg3();
    void glm3();
    void eigen3();
    #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
    void boostQvm3();
    #endif
    void mathfu3();
//     void sse3();

    Matrix4 data4 = Matrix4::rotation(134.7_degf, Vector3{1.0f, 3.0f, -1.4f}.normalized())*Matrix4::translation(Vector3::zAxis());
    Matrix3 data3 = Matrix3::rotation(134.7_degf)*Matrix3::translation(Vector2::yAxis());
};

InvertBenchmark::InvertBenchmark() {
    addBenchmarks({
        &InvertBenchmark::magnumMath4,
        &InvertBenchmark::magnumMathGaussJordan4,
        &InvertBenchmark::linalg4,
        &InvertBenchmark::glm4,
        &InvertBenchmark::eigen4,
        #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
        &InvertBenchmark::boostQvm4,
        #endif
        &InvertBenchmark::mathfu4,
        #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
        &InvertBenchmark::sse4,
        #endif

        &InvertBenchmark::magnumMath3,
        &InvertBenchmark::magnumMathGaussJordan3,
        &InvertBenchmark::linalg3,
        &InvertBenchmark::glm3,
        &InvertBenchmark::eigen3,
        #if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
        &InvertBenchmark::boostQvm3,
        #endif
        &InvertBenchmark::mathfu3,
//         &InvertBenchmark::sse3,

    }, 50);
}

enum: std::size_t { Repeats = 10000 };

void InvertBenchmark::magnumMath4() {
    Matrix4 a = data4;
    CORRADE_BENCHMARK(Repeats) {
        a = a.inverted();
    }

    CORRADE_VERIFY(a.toVector().sum() != 0);
}

void InvertBenchmark::magnumMathGaussJordan4() {
    Matrix4 a = data4;
    CORRADE_BENCHMARK(Repeats) {
        a = Math::Algorithms::gaussJordanInverted(a);
    }

    CORRADE_VERIFY(a.toVector().sum() != 0);
}

void InvertBenchmark::linalg4() {
    linalg::mat<float, 4, 4> a;
    Matrix4::from(&a[0][0]) = data4;
    CORRADE_BENCHMARK(Repeats) {
        a = linalg::inverse(a);
    }

    CORRADE_VERIFY(Matrix4::from(&a[0][0]).toVector().sum() != 0);
}

void InvertBenchmark::glm4() {
    glm::mat4 a(data4);

    CORRADE_VERIFY(Matrix4{glm::inverse(a)} == data4.inverted());

    CORRADE_BENCHMARK(Repeats) {
        a = glm::inverse(a);
    }

    CORRADE_VERIFY(Matrix4{a}.toVector().sum() != 0);
}

void InvertBenchmark::eigen4() {
    Eigen::Matrix4f a = EigenIntegration::eigenCast<Eigen::Matrix4f>(data4);
    CORRADE_BENCHMARK(Repeats) {
        a = a.inverse().eval();
    }

    CORRADE_VERIFY(Matrix4{a}.toVector().sum() != 0);
}

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
void InvertBenchmark::boostQvm4() {
    boost::qvm::mat<float, 4, 4> a;
    Matrix4::from(&A00(a)) = data4;
    CORRADE_BENCHMARK(Repeats) {
        a = boost::qvm::inverse(a);
    }

    CORRADE_VERIFY(Matrix4::from(&A00(a)).toVector().sum() != 0);
}
#endif

void InvertBenchmark::mathfu4() {
    mathfu::Matrix<float, 4, 4> a;
    Matrix4::from(&a(0, 0)) = data4;

    CORRADE_BENCHMARK(Repeats) {
        a = a.Inverse();
    }

    CORRADE_VERIFY(Matrix4::from(&a(0, 0)).toVector().sum() != 0);
}

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
struct alignas(16) SseM4
{
public:
	union
	{
		float m[4][4];
		__m128 mVec[4];
	};
};

#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))

// vec(0, 1, 2, 3) -> (vec[x], vec[y], vec[z], vec[w])
#define VecSwizzleMask(vec, mask)          _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), mask))
#define VecSwizzle(vec, x, y, z, w)        VecSwizzleMask(vec, MakeShuffleMask(x,y,z,w))
#define VecSwizzle1(vec, x)                VecSwizzleMask(vec, MakeShuffleMask(x,x,x,x))
// special swizzle
#define VecSwizzle_0022(vec)               _mm_moveldup_ps(vec)
#define VecSwizzle_1133(vec)               _mm_movehdup_ps(vec)

// return (vec1[x], vec1[y], vec2[z], vec2[w])
#define VecShuffle(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, MakeShuffleMask(x,y,z,w))
// special shuffle
#define VecShuffle_0101(vec1, vec2)        _mm_movelh_ps(vec1, vec2)
#define VecShuffle_2323(vec1, vec2)        _mm_movehl_ps(vec2, vec1)

// for column major matrix
// we use __m128 to represent 2x2 matrix as A = | A0  A2 |
//                                              | A1  A3 |
// 2x2 column major Matrix multiply A*B
inline __m128 Mat2Mul(__m128 vec1, __m128 vec2)
{
	return
		_mm_add_ps(_mm_mul_ps(                     vec1, VecSwizzle(vec2, 0,0,3,3)),
		           _mm_mul_ps(VecSwizzle(vec1, 2,3,0,1), VecSwizzle(vec2, 1,1,2,2)));
}
// 2x2 column major Matrix adjugate multiply (A#)*B
inline __m128 Mat2AdjMul(__m128 vec1, __m128 vec2)
{
	return
		_mm_sub_ps(_mm_mul_ps(VecSwizzle(vec1, 3,0,3,0), vec2),
		           _mm_mul_ps(VecSwizzle(vec1, 2,1,2,1), VecSwizzle(vec2, 1,0,3,2)));

}
// 2x2 column major Matrix multiply adjugate A*(B#)
inline __m128 Mat2MulAdj(__m128 vec1, __m128 vec2)
{
	return
		_mm_sub_ps(_mm_mul_ps(                     vec1, VecSwizzle(vec2, 3,3,0,0)),
		           _mm_mul_ps(VecSwizzle(vec1, 2,3,0,1), VecSwizzle(vec2, 1,1,2,2)));
}

// Inverse function is the same no matter column major or row major
// this version treats it as column major
inline SseM4 GetInverse(const SseM4& inM)
{
	// use block matrix method
	// A is a matrix, then i(A) or iA means inverse of A, A# (or A_ in code) means adjugate of A, |A| (or detA in code) is determinant, tr(A) is trace

	// sub matrices
	__m128 A = VecShuffle_0101(inM.mVec[0], inM.mVec[1]);
	__m128 C = VecShuffle_2323(inM.mVec[0], inM.mVec[1]);
	__m128 B = VecShuffle_0101(inM.mVec[2], inM.mVec[3]);
	__m128 D = VecShuffle_2323(inM.mVec[2], inM.mVec[3]);

#if 0
	__m128 detA = _mm_set1_ps(inM.m[0][0] * inM.m[1][1] - inM.m[0][1] * inM.m[1][0]);
	__m128 detC = _mm_set1_ps(inM.m[0][2] * inM.m[1][3] - inM.m[0][3] * inM.m[1][2]);
	__m128 detB = _mm_set1_ps(inM.m[2][0] * inM.m[3][1] - inM.m[2][1] * inM.m[3][0]);
	__m128 detD = _mm_set1_ps(inM.m[2][2] * inM.m[3][3] - inM.m[2][3] * inM.m[3][2]);
#else
	// determinant as (|A| |C| |B| |D|)
	__m128 detSub = _mm_sub_ps(
		_mm_mul_ps(VecShuffle(inM.mVec[0], inM.mVec[2], 0,2,0,2), VecShuffle(inM.mVec[1], inM.mVec[3], 1,3,1,3)),
		_mm_mul_ps(VecShuffle(inM.mVec[0], inM.mVec[2], 1,3,1,3), VecShuffle(inM.mVec[1], inM.mVec[3], 0,2,0,2))
		);
	__m128 detA = VecSwizzle1(detSub, 0);
	__m128 detC = VecSwizzle1(detSub, 1);
	__m128 detB = VecSwizzle1(detSub, 2);
	__m128 detD = VecSwizzle1(detSub, 3);
#endif

	// let iM = 1/|M| * | X  Y |
	//                  | Z  W |

	// D#C
	__m128 D_C = Mat2AdjMul(D, C);
	// A#B
	__m128 A_B = Mat2AdjMul(A, B);
	// X# = |D|A - B(D#C)
	__m128 X_ = _mm_sub_ps(_mm_mul_ps(detD, A), Mat2Mul(B, D_C));
	// W# = |A|D - C(A#B)
	__m128 W_ = _mm_sub_ps(_mm_mul_ps(detA, D), Mat2Mul(C, A_B));

	// |M| = |A|*|D| + ... (continue later)
	__m128 detM = _mm_mul_ps(detA, detD);

	// Y# = |B|C - D(A#B)#
	__m128 Y_ = _mm_sub_ps(_mm_mul_ps(detB, C), Mat2MulAdj(D, A_B));
	// Z# = |C|B - A(D#C)#
	__m128 Z_ = _mm_sub_ps(_mm_mul_ps(detC, B), Mat2MulAdj(A, D_C));

	// |M| = |A|*|D| + |B|*|C| ... (continue later)
	detM = _mm_add_ps(detM, _mm_mul_ps(detB, detC));

	// tr((A#B)(D#C))
	__m128 tr = _mm_mul_ps(A_B, VecSwizzle(D_C, 0,2,1,3));
	tr = _mm_hadd_ps(tr, tr);
	tr = _mm_hadd_ps(tr, tr);
	// |M| = |A|*|D| + |B|*|C| - tr((A#B)(D#C))
	detM = _mm_sub_ps(detM, tr);

	const __m128 adjSignMask = _mm_setr_ps(1.f, -1.f, -1.f, 1.f);
	// (1/|M|, -1/|M|, -1/|M|, 1/|M|)
	__m128 rDetM = _mm_div_ps(adjSignMask, detM);

	X_ = _mm_mul_ps(X_, rDetM);
	Y_ = _mm_mul_ps(Y_, rDetM);
	Z_ = _mm_mul_ps(Z_, rDetM);
	W_ = _mm_mul_ps(W_, rDetM);

	SseM4 r;

	// apply adjugate and store, here we combine adjugate shuffle and store shuffle
	r.mVec[0] = VecShuffle(X_, Z_, 3,1,3,1);
	r.mVec[1] = VecShuffle(X_, Z_, 2,0,2,0);
	r.mVec[2] = VecShuffle(Y_, W_, 3,1,3,1);
	r.mVec[3] = VecShuffle(Y_, W_, 2,0,2,0);

	return r;
}

void InvertBenchmark::sse4() {
    SseM4 a;
    Matrix4::from(a.m[0]) = data4;
    CORRADE_BENCHMARK(Repeats) {
        a = GetInverse(a);
    }

    CORRADE_VERIFY(Matrix4::from(a.m[0]).toVector().sum() != 0);
}
#endif

void InvertBenchmark::magnumMath3() {
    Matrix3 a = data3;
    CORRADE_BENCHMARK(Repeats) {
        a = a.inverted();
    }

    CORRADE_VERIFY(a.toVector().sum() != 0);
}

void InvertBenchmark::magnumMathGaussJordan3() {
    Matrix3 a = data3;
    CORRADE_BENCHMARK(Repeats) {
        a = Math::Algorithms::gaussJordanInverted(a);
    }

    CORRADE_VERIFY(a.toVector().sum() != 0);
}

void InvertBenchmark::linalg3() {
    linalg::mat<float, 3, 3> a;
    Matrix3::from(&a[0][0]) = data3;
    CORRADE_BENCHMARK(Repeats) {
        a = linalg::inverse(a);
    }

    CORRADE_VERIFY(Matrix3::from(&a[0][0]).toVector().sum() != 0);
}

void InvertBenchmark::glm3() {
    glm::mat3 a(data3);

    CORRADE_VERIFY(Matrix3{glm::inverse(a)} == data3.inverted());

    CORRADE_BENCHMARK(Repeats) {
        a = glm::inverse(a);
    }

    CORRADE_VERIFY(Matrix3{a}.toVector().sum() != 0);
}

void InvertBenchmark::eigen3() {
    Eigen::Matrix3f a = EigenIntegration::eigenCast<Eigen::Matrix3f>(data3);
    CORRADE_BENCHMARK(Repeats) {
        a = a.inverse().eval();
    }

    CORRADE_VERIFY(Matrix3{a}.toVector().sum() != 0);
}

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
void InvertBenchmark::boostQvm3() {
    boost::qvm::mat<float, 3, 3> a;
    Matrix3::from(&A00(a)) = data3;
    CORRADE_BENCHMARK(Repeats) {
        a = boost::qvm::inverse(a);
    }

    CORRADE_VERIFY(Matrix3::from(&A00(a)).toVector().sum() != 0);
}
#endif

void InvertBenchmark::mathfu3() {
    mathfu::Matrix<float, 3, 3> a;
    Matrix3::from(&a(0, 0)) = data3;

    CORRADE_BENCHMARK(Repeats) {
        a = a.Inverse();
    }

    CORRADE_VERIFY(Matrix3::from(&a(0, 0)).toVector().sum() != 0);
}

CORRADE_TEST_MAIN(InvertBenchmark)
