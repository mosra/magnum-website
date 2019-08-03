// Do the following:
//
// get https://github.com/kokkos/mdspan/tree/04e93536ad9d30703b7cc1d363ff4e7d4a1b8ef8
//
// g++ CompileTimeBenchmark.cpp -o CompileTimeBenchmark -std=c++11 -lCorradeTestSuite-d -lCorradeUtility-d
// ./CompileTimeBenchmark --color on # until it gets white

#include <string>
#include <Corrade/Containers/Array.h>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/Format.h>

using namespace Corrade;

struct CompileTimeBenchmark: TestSuite::Tester {
    explicit CompileTimeBenchmark();

    void baseline();
    void arrayView();
    void stridedArrayView();
    void mdspanInclude();
    void mdspan();
};

struct {
    const char* name;
    const char* compiler;
} CompilerData[]{
    {"GCC, C++11", "g++ -std=c++11"},
    {"GCC, C++17", "g++ -std=c++17"},
    {"GCC, C++2a", "g++ -std=c++2a"}
};

CompileTimeBenchmark::CompileTimeBenchmark() {
    addBenchmarks({&CompileTimeBenchmark::baseline,
                   &CompileTimeBenchmark::arrayView}, 20);

    addInstancedBenchmarks({&CompileTimeBenchmark::stridedArrayView}, 20,
        Containers::arraySize(CompilerData));

    addBenchmarks({&CompileTimeBenchmark::mdspanInclude}, 20);

    addInstancedBenchmarks({&CompileTimeBenchmark::mdspan}, 20,
        Containers::arraySize(CompilerData));
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
#include <Corrade/Containers/StridedArrayView.h>

using namespace Corrade;

int main() {
    int data[]{1, 3, 42, 1337, -3, 15};

    Containers::StridedArrayView2D<int> a{data, {3, 2}};
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ -std=c++11 main.cpp -o arrayView"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::stridedArrayView() {
    auto&& data = CompilerData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system(Utility::format("{} stridedArrayView.cpp -o stridedArrayView", data.compiler)), 0);
    }
}

void CompileTimeBenchmark::mdspanInclude() {
    Utility::Directory::writeString("main.cpp", R"(
#include </home/mosra/Code/mdspan/include/experimental/mdspan>

int main() {
    int data[]{1, 3, 42, 1337, -3, 15};

    std::experimental::mdspan a{data, {3, 2}};
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ -std=c++11 mdspan.cpp -o mdspan"), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::mdspan() {
    auto&& data = CompilerData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system(Utility::format("{} mdspan.cpp -o mdspan", data.compiler)), 0);
    }
}

CORRADE_TEST_MAIN(CompileTimeBenchmark)
