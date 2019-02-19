// Do the following:
//
// g++ CompileTimeBenchmark.cpp -o CompileTimeBenchmark -std=c++11 -lCorradeTestSuite-d -lCorradeUtility-d
// ./CompileTimeBenchmark --color on # until it gets white

#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Directory.h>

using namespace Corrade;

struct CompileTimeBenchmark: TestSuite::Tester {
    explicit CompileTimeBenchmark();

    void baseline();
    void pointer11();
    void pointer2a();
    void reference11();
    void reference2a();
    void uniquePtr11();
    void uniquePtr2a();
    void referenceWrapper11();
    void referenceWrapper2a();

    void clangBaseline();
    void clangPointer();
    void clangMemory();
    void modules();
};

CompileTimeBenchmark::CompileTimeBenchmark() {
    addBenchmarks({&CompileTimeBenchmark::baseline,
                   &CompileTimeBenchmark::pointer11,
                   &CompileTimeBenchmark::pointer2a,
                   &CompileTimeBenchmark::reference11,
                   &CompileTimeBenchmark::reference2a}, 10, BenchmarkType::WallTime);

    addBenchmarks({&CompileTimeBenchmark::uniquePtr11,
                   &CompileTimeBenchmark::uniquePtr2a,
                   &CompileTimeBenchmark::referenceWrapper11,
                   &CompileTimeBenchmark::referenceWrapper2a}, 5, BenchmarkType::WallTime);

    addBenchmarks({&CompileTimeBenchmark::clangBaseline,
                   &CompileTimeBenchmark::clangPointer,
                   &CompileTimeBenchmark::clangMemory,
                   &CompileTimeBenchmark::modules}, 10, BenchmarkType::WallTime);
}

void CompileTimeBenchmark::baseline() {
    Utility::Directory::writeString("baseline.cpp", R"(
int main() {}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ baseline.cpp -o baseline -std=c++11"), 0);
    }

    Utility::Directory::rm("baseline.cpp");
}

void CompileTimeBenchmark::pointer11() {
    Utility::Directory::writeString("pointer11.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <Corrade/Containers/Pointer.h>

using namespace Corrade;

int main() {
    Containers::Pointer<int> a{new int{5}};
    return *a;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ pointer11.cpp -o pointer11 -std=c++11"), 0);
    }

    Utility::Directory::rm("pointer11.cpp");
}

void CompileTimeBenchmark::pointer2a() {
    Utility::Directory::writeString("pointer2a.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <Corrade/Containers/Pointer.h>

using namespace Corrade;

int main() {
    Containers::Pointer<int> a{new int{5}};
    return *a;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ pointer2a.cpp -o pointer2a -std=c++2a"), 0);
    }

    Utility::Directory::rm("pointer2a.cpp");
}

void CompileTimeBenchmark::reference11() {
    Utility::Directory::writeString("reference11.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <Corrade/Containers/Reference.h>

using namespace Corrade;

int main() {
    int a = 5;
    Containers::Reference<int> b = a;
    return b;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ reference11.cpp -o reference11 -std=c++11"), 0);
    }

    Utility::Directory::rm("reference11.cpp");
}

void CompileTimeBenchmark::reference2a() {
    Utility::Directory::writeString("reference2a.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <Corrade/Containers/Reference.h>

using namespace Corrade;

int main() {
    int a = 5;
    Containers::Reference<int> b = a;
    return b;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ reference2a.cpp -o reference2a -std=c++2a"), 0);
    }

    Utility::Directory::rm("reference2a.cpp");
}

void CompileTimeBenchmark::uniquePtr11() {
    Utility::Directory::writeString("uniquePtr11.cpp", R"(
#include <memory>

int main() {
    std::unique_ptr<int> a{new int{5}};
    return *a;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ uniquePtr11.cpp -o uniquePtr11 -std=c++11"), 0);
    }

    Utility::Directory::rm("uniquePtr11.cpp");
}

void CompileTimeBenchmark::uniquePtr2a() {
    Utility::Directory::writeString("uniquePtr2a.cpp", R"(
#include <memory>

int main() {
    std::unique_ptr<int> a{new int{5}};
    return *a;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ uniquePtr11.cpp -o uniquePtr11 -std=c++2a"), 0);
    }

    Utility::Directory::rm("uniquePtr2a.cpp");
}

void CompileTimeBenchmark::referenceWrapper11() {
    Utility::Directory::writeString("referenceWrapper11.cpp", R"(
#include <functional>

int main() {
    int a = 5;
    std::reference_wrapper<int> b = a;
    return b;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ referenceWrapper11.cpp -o referenceWrapper11 -std=c++11"), 0);
    }

    Utility::Directory::rm("referenceWrapper11.cpp");
}

void CompileTimeBenchmark::referenceWrapper2a() {
    Utility::Directory::writeString("referenceWrapper2a.cpp", R"(
#include <functional>

int main() {
    int a = 5;
    std::reference_wrapper<int> b = a;
    return b;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ referenceWrapper2a.cpp -o referenceWrapper2a -std=c++2a"), 0);
    }

    Utility::Directory::rm("referenceWrapper2a.cpp");
}

void CompileTimeBenchmark::clangBaseline() {
    Utility::Directory::writeString("clangBaseline.cpp", R"(
int main() {}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("clang++ clangBaseline.cpp -o clangBaseline -std=c++17"), 0);
    }

    Utility::Directory::rm("clangBaseline.cpp");
}

void CompileTimeBenchmark::clangPointer() {
    Utility::Directory::writeString("clangPointer.cpp", R"(
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <Corrade/Containers/Pointer.h>

using namespace Corrade;

int main() {
    Containers::Pointer<int> a{new int{5}};
    return *a;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("clang++ clangPointer.cpp -o clangPointer -std=c++17"), 0);
    }

    Utility::Directory::rm("clangPointer.cpp");
}

void CompileTimeBenchmark::clangMemory() {
    Utility::Directory::writeString("clangMemory.cpp", R"(
#include <memory>

int main() {
    std::unique_ptr<int> a{new int{5}};
    return *a;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("clang++ clangMemory.cpp -o clangMemory -std=c++17"), 0);
    }

    Utility::Directory::rm("clangMemory.cpp");
}

void CompileTimeBenchmark::modules() {
    Utility::Directory::writeString("modules.cpp", R"(
import std;

int main() {

    std::unique_ptr<int> a{new int{0}};
    return *a;
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("clang++ -fmodules-ts -fmodule-map-file=/usr/include/c++/v1/module.modulemap -fmodules-cache-path=. -fimplicit-modules -stdlib=libc++ modules.cpp -o modules -DNDEBUG -std=c++17"), 0);
    }

    Utility::Directory::rm("modules.cpp");
}

CORRADE_TEST_MAIN(CompileTimeBenchmark)
