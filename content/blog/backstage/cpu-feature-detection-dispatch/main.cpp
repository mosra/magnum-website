// g++ -std=c++14 main.cpp -lCorradeUtility

#include <cstddef>
#include <Corrade/Corrade.h>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Cpu.h>

#define MCSS_ELLIPSIS(...) __VA_ARGS__
#define MCSS_IGNORE(...) __VA_ARGS__

using namespace Corrade;

namespace Foo {
// [declare]
struct ScalarT {};
struct Sse2T: ScalarT {};
MCSS_ELLIPSIS(struct AvxT: Sse2T {};)
struct Avx2T: AvxT {};
struct Avx512T: Avx2T{};

const char* memrchr(ScalarT, const char* data, std::size_t size, char c);
const char* memrchr(Sse2T, const char* data, std::size_t size, char c);
const char* memrchr(Avx2T, const char* data, std::size_t size, char c);
// [/declare]

const char* memrchr(ScalarT, const char*, std::size_t, char) {
    return {};
}
const char* memrchr(Sse2T, const char*, std::size_t, char) {
    return {};
}
const char* memrchr(Avx2T, const char*, std::size_t, char) {
    return {};
}

// [typedef]
#ifdef CORRADE_TARGET_AVX512
typedef Avx512T DefaultCpuT;
#elif defined(CORRADE_TARGET_AVX2)
typedef Avx2T DefaultCpuT;
MCSS_ELLIPSIS()
#elif defined(CORRADE_TARGET_SSE2)
typedef Sse2T DefaultCpuT;
#else
typedef ScalarT DefaultCpuT;
#endif

constexpr DefaultCpuT DefaultCpu;
// [/typedef]

bool targetIsAvx2{}, targetIsSse2{};
// [runtime-wrong]
const char* memrchr(const char* data, std::size_t size, char c) {
    if(targetIsAvx2)
        return memrchr(Avx2T{}, data, size, c);
    if(targetIsSse2)
        return memrchr(Sse2T{}, data, size, c);
    // plus #ifdefs and branches for ARM NEON, WebAssembly SIMD, ...
    return memrchr(ScalarT{}, data, size, c);
}
// [/runtime-wrong]
}

// [final-declare]
#include <Corrade/Cpu.h>

using namespace Corrade;

CORRADE_ALWAYS_INLINE auto memrchrImplementation(Cpu::ScalarT) {
    return +[](const char* data, std::size_t size, char c) { MCSS_ELLIPSIS(static_cast<void>(size); static_cast<void>(c); return data;) };
}

CORRADE_ALWAYS_INLINE auto memrchrImplementation(Cpu::Sse2T) {
    return +[](const char* data, std::size_t size, char c) { MCSS_ELLIPSIS(static_cast<void>(size); static_cast<void>(c); return data;) };
}

CORRADE_ALWAYS_INLINE auto memrchrImplementation(Cpu::Avx2T) {
    return +[](const char* data, std::size_t size, char c) { MCSS_ELLIPSIS(static_cast<void>(size); static_cast<void>(c); return data;) };
}

CORRADE_CPU_DISPATCHER_BASE(memrchrImplementation)
// [/final-declare]
namespace Bar {
// [final-dispatched-pointer]
CORRADE_CPU_DISPATCHED_POINTER(memrchrImplementation,
    const char*(*memrchr)(const char*, std::size_t, char))
// [/final-dispatched-pointer]
}

// [final-dispatched-ifunc]
CORRADE_CPU_DISPATCHED_IFUNC(memrchrImplementation,
    const char* memrchr(const char*, std::size_t, char))
// [/final-dispatched-ifunc]

namespace Baz {
// [final-dispatched-compile-time]
const char* memrchr(const char* data, std::size_t size, char c) {
    return memrchrImplementation(Cpu::DefaultBase)(data, size, c);
}
// [/final-dispatched-compile-time]
}

namespace Bad {

// [enable-macros]
#ifdef CORRADE_ENABLE_AVX2
CORRADE_ALWAYS_INLINE auto memrchrImplementation(Cpu::Avx2T) {
    return +[](const char* data, std::size_t size, char c) CORRADE_ENABLE_AVX2 { MCSS_ELLIPSIS(static_cast<void>(size); static_cast<void>(c); return data;) };
}
#endif
// [/enable-macros]

}


namespace Bar {
// [final-use]
extern const char*(*memrchr)(const char*, std::size_t, char);

MCSS_ELLIPSIS(void main() {

{)

const char* found = memrchr(MCSS_ELLIPSIS({}, {}, {}));
// [/final-use]
static_cast<void>(found);
}

{
using namespace Foo;
using Foo::memrchr;
// [use]
const char* found = memrchr(DefaultCpu, MCSS_ELLIPSIS({}, {}, {}));
// [/use]
static_cast<void>(found);
}

{
using namespace Corrade;
// [final-use-implementation]
const char* found = memrchrImplementation(Cpu::DefaultBase)(MCSS_ELLIPSIS({}, {}, {}));
// [/final-use-implementation]
static_cast<void>(found);
}
}
}

// [tag-conversion-operator]
template<unsigned bits> struct Tag {
    template<
        unsigned otherBits,
        class = std::enable_if_t<(otherBits & bits) == otherBits>
    > operator Tag<otherBits>() const { MCSS_ELLIPSIS(return {};) }
};
// [/tag-conversion-operator]

// [calculated-priority]
template<unsigned i> struct Priority: Priority<i - 1> {};
template<> struct Priority<0> {};
// [/calculated-priority]

namespace Bac {

// [tag-priority-declaration]
void foo(Tag<0b00000000>, Priority<000>) { MCSS_ELLIPSIS() } /* scalar */
void foo(Tag<0b00000011>, Priority<200>) { MCSS_ELLIPSIS() } /* SSE3 */
void foo(Tag<0b01000011>, Priority<201>) { MCSS_ELLIPSIS() } /* SSE3+POPCNT */
void foo(Tag<0b10000111>, Priority<301>) { MCSS_ELLIPSIS() } /* SSSE3+LZCNT */
// [/tag-priority-declaration]
}

// [use-extra]
void foo(CORRADE_CPU_DECLARE(Cpu::Scalar)) { MCSS_ELLIPSIS() }
void foo(CORRADE_CPU_DECLARE(Cpu::Sse3)) { MCSS_ELLIPSIS() }
void foo(CORRADE_CPU_DECLARE(Cpu::Sse3|Cpu::Popcnt)) { MCSS_ELLIPSIS() }
void foo(CORRADE_CPU_DECLARE(Cpu::Ssse3|Cpu::Lzcnt)) { MCSS_ELLIPSIS() }

MCSS_ELLIPSIS(int main() {)

foo(CORRADE_CPU_SELECT(Cpu::Sse3|Cpu::Lzcnt|Cpu::Popcnt));

// [/use-extra]
}

// [dispatcher-extra]
CORRADE_CPU_DISPATCHER(foo, Cpu::Popcnt, Cpu::Lzcnt)
// [/dispatcher-extra]

namespace Bar {
struct MemrchrTest: TestSuite::Tester {
    void test();
};

// [testing]
const struct {
    const char* name;
    Cpu::Features features;
} TestData[]{
    {"scalar", Cpu::Scalar},
    {"SSE2", Cpu::Sse2},
    {"AVX2", Cpu::Avx2},
};

void MemrchrTest::test() {
    auto&& data = TestData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    memrchr = memrchrImplementation(data.features);

    MCSS_ELLIPSIS()
}
// [/testing]
}
