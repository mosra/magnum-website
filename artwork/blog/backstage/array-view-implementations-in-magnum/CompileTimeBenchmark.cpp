// Do the following:
//
// get https://raw.githubusercontent.com/martinmoene/span-lite/v0.4.0/include/nonstd/span.hpp
//
// g++ CompileTimeBenchmark.cpp -o CompileTimeBenchmark -std=c++11 -lCorradeTestSuite-d -lCorradeUtility-d
// ./CompileTimeBenchmark --color on # until it gets white

#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Directory.h>

using namespace Corrade;

struct CompileTimeBenchmark: TestSuite::Tester {
    explicit CompileTimeBenchmark();

    void baseline();
    void arrayView();
    void gslSpan();
    void spanLite();

    void clangBaseline();
    void clangArrayView();
    void clangGslSpan();
    void clangSpan();
    void clangSpanLite();
};

CompileTimeBenchmark::CompileTimeBenchmark() {
    addBenchmarks({&CompileTimeBenchmark::baseline,
                   &CompileTimeBenchmark::arrayView}, 20,
                  BenchmarkType::WallTime);

    addBenchmarks({&CompileTimeBenchmark::gslSpan,
                   &CompileTimeBenchmark::spanLite}, 5);

    addBenchmarks({&CompileTimeBenchmark::clangBaseline,
                   &CompileTimeBenchmark::clangArrayView}, 20, BenchmarkType::WallTime);

    addBenchmarks({&CompileTimeBenchmark::clangGslSpan,
                   &CompileTimeBenchmark::clangSpan,
                   &CompileTimeBenchmark::clangSpanLite}, 5, BenchmarkType::WallTime);
}

void CompileTimeBenchmark::baseline() {
    Utility::Directory::writeString("main.cpp", R"(
int main() {}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ main.cpp -o baseline -std=c++11"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::arrayView() {
    Utility::Directory::writeString("main.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <Corrade/Containers/ArrayView.h>

using namespace Corrade;

int main() {
    int data[]{1, 3, 42, 1337};

    auto a = Containers::arrayView(data);
    Containers::StaticArrayView<1, int> b = a.slice<1>(2);
    return *b - 42;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ main.cpp -o arrayView -std=c++11"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::gslSpan() {
    Utility::Directory::writeString("main.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <gsl/span>

int main() {
    int data[]{1, 3, 42, 1337};

    gsl::span<int> a = data;
    gsl::span<int, 1> b = a.subspan<2, 1>();
    return b[0] - 42;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ main.cpp -o gslSpan -std=c++14"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::spanLite() {
    Utility::Directory::writeString("main.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include "span.hpp"

int main() {
    int data[]{1, 3, 42, 1337};

    nonstd::span<int> a = data;
    nonstd::span<int, 1> b = a.subspan<2, 1>();
    return b[0] - 42;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ main.cpp -o spanLite -std=c++11"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::clangBaseline() {
    Utility::Directory::writeString("main.cpp", R"(
int main() {}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("clang++ main.cpp -o clangBaseline -std=c++2a -stdlib=libc++"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::clangArrayView() {
    Utility::Directory::writeString("main.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <Corrade/Containers/ArrayView.h>

using namespace Corrade;

int main() {
    int data[]{1, 3, 42, 1337};

    auto a = Containers::arrayView(data);
    Containers::StaticArrayView<1, int> b = a.slice<1>(2);
    return *b - 42;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("clang++ main.cpp -o clangArrayView -std=c++2a -stdlib=libc++"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::clangSpan() {
    Utility::Directory::writeString("main.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <span>

int main() {
    int data[]{1, 3, 42, 1337};

    std::span<int> a = data;
    std::span<int, 1> b = a.subspan<2, 1>();
    return b[0] - 42;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("clang++ main.cpp -o clangSpan -std=c++2a -stdlib=libc++"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::clangGslSpan() {
    Utility::Directory::writeString("main.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <gsl/span>

int main() {
    int data[]{1, 3, 42, 1337};

    gsl::span<int> a = data;
    gsl::span<int, 1> b = a.subspan<2, 1>();
    return b[0] - 42;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("clang++ main.cpp -o clangGslSpan -std=c++2a -stdlib=libc++"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::clangSpanLite() {
    Utility::Directory::writeString("main.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include "span.hpp"

int main() {
    int data[]{1, 3, 42, 1337};

    nonstd::span<int> a = data;
    nonstd::span<int, 1> b = a.subspan<2, 1>();
    return b[0] - 42;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("clang++ main.cpp -o clangSpanLite -std=c++2a -stdlib=libc++"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

CORRADE_TEST_MAIN(CompileTimeBenchmark)
