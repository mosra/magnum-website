// Do the following:
//
// put these here:
//  MagnumMath.h
//  https://raw.githubusercontent.com/sgorsten/linalg/10c63b6825381e85688d6a53eb0a41f3cec02010/linalg.h
//
// g++ CompileTimeBenchmark.cpp -o CompileTimeBenchmark -std=c++11 -lCorradeTestSuite-d -lCorradeUtility-d
// ./CompileTimeBenchmark --color on # until it gets white

#include <string>
#include <Corrade/Containers/ArrayView.h>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Directory.h>

using namespace Corrade;

struct CompileTimeBenchmark: TestSuite::Tester {
    explicit CompileTimeBenchmark();

    void cpp11();
    void cpp17();
};

constexpr struct {
    const char* name;
    const char* src;
    const char* cmd;
    int iterations;
} Data[]{

    {"baseline", R"(
        int main() {}
        )", nullptr, 8},
    {"cmath", R"(
        #include <cmath>

        int main() {
            int a[2]{1, 2};
        }
        )", nullptr, 4},
    {"Magnum/Math/Vector.h", R"(
        #define CORRADE_STANDARD_ASSERT
        #define CORRADE_NO_DEBUG
        #define CORRADE_NO_TWEAKABLE
        #include "Magnum/Math/Vector2.h"
        #include "Magnum/Magnum.h"

        using namespace Magnum;

        int main() {
            Vector2i a{1, 2};
        }
        )", nullptr, 4},
    {"MagnumMath.hpp", R"(
        #include "MagnumMath.hpp"

        using namespace Magnum;

        int main() {
            Vector2i a{1, 2};
        }
        )", nullptr, 4},
    {"linalg.h", R"(
        #include "linalg.h"

        int main() {
            linalg::vec<int, 2> a{1, 2};
        }
        )", nullptr, 4},
    {"CxxSwizzle", R"(
        #include <swizzle/glsl/vector.h>
        #include <swizzle/glsl/vector_functions.h>
        #include <swizzle/glsl/scalar_support.h>

        int main() {
            swizzle::glsl::vector<int, 2> a{1, 2};
        }
        )", "-fno-operator-names -I/home/mosra/Code/CxxSwizzle/include", 4},
    {"vml", R"(
        #include <type_traits>
        #include <cstddef>
        #include <vector.h>
        #include <vector_functions.h>

        int main() {
            vml::vector<int, 0, 1> a{1, 2};
        }
        )", "-I/home/mosra/Code/vml/vml", 4},
    {"MathFu", R"(
        #include "mathfu/vector.h"

        int main() {
            mathfu::Vector<int, 2> a{1, 2};
        }
        )", "-I/home/mosra/Code/mathfu/include -I/home/mosra/Code/mathfu/dependencies/vectorial/include", 4},
    {"glm/glm.hpp", R"(
        #define GLM_FORCE_CTOR_INIT
        #include <glm/glm.hpp>

        int main() {
            glm::ivec2 a{1, 2};
        }
        )", nullptr, 2},
    {"glm/glm.hpp + swizzle", R"(
        #define GLM_FORCE_CTOR_INIT
        #define GLM_FORCE_SWIZZLE
        #include <glm/glm.hpp>

        int main() {
            glm::ivec2 a{1, 2};
        }
        )", nullptr, 1},
    {"Eigen/Core", R"(
        #include <eigen3/Eigen/Core>

        int main() {
            Eigen::Vector2i a{1, 2};
        }
        )", nullptr, 2},
    {"boost/qvm/vec.hpp", R"(
        #include <boost/qvm/vec.hpp>

        int main() {
            boost::qvm::vec<int, 2> v = {1, 2};
        }
        )", nullptr, 2},
    {"boost/qvm/all.hpp", R"(
        #include <boost/qvm/all.hpp>

        int main() {
            boost::qvm::vec<int, 2> v = {1, 2};
        }
        )", nullptr, 2},
    {"boost/geometry.hpp", R"(
        #include <boost/geometry.hpp>

        int main() {
            boost::geometry::model::point<int, 2, boost::geometry::cs::cartesian> a{1, 2};
        }
)", nullptr, 1}};

CompileTimeBenchmark::CompileTimeBenchmark() {
    addInstancedBenchmarks({
        &CompileTimeBenchmark::cpp11,
        &CompileTimeBenchmark::cpp17}, 5, Containers::arraySize(Data));
}

void CompileTimeBenchmark::cpp11() {
    auto&& data = Data[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    if(data.name == std::string{"vml"})
        CORRADE_SKIP("This library requires C++17.");

    Utility::Directory::writeString("main.cpp", data.src);

    std::string cmd = "g++ main.cpp -o /dev/null -std=c++11 ";
    if(data.cmd) cmd += data.cmd;

    CORRADE_BENCHMARK(data.iterations) {
        CORRADE_COMPARE(std::system(cmd.data()), 0);
    }

    Utility::Directory::rm("main.cpp");
}

void CompileTimeBenchmark::cpp17() {
    auto&& data = Data[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Utility::Directory::writeString("main.cpp", data.src);

    std::string cmd = "g++ main.cpp -o /dev/null -std=c++17 ";
    if(data.cmd) cmd += data.cmd;

    CORRADE_BENCHMARK(data.iterations) {
        CORRADE_COMPARE(std::system(cmd.data()), 0);
    }

    Utility::Directory::rm("main.cpp");
}

CORRADE_TEST_MAIN(CompileTimeBenchmark)
