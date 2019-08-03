// Do the following:
//
// get https://github.com/kokkos/mdspan/tree/04e93536ad9d30703b7cc1d363ff4e7d4a1b8ef8
//
// g++ RuntimeBenchmark.cpp -o RuntimeBenchmark -std=c++11 -lCorradeTestSuite-d -lCorradeUtility-d
//
// or
//
// g++ -O3 -DCORRADE_NO_ASSERT RuntimeBenchmark.cpp -o RuntimeBenchmark -std=c++11 -lCorradeTestSuite-d -lCorradeUtility-d
//
// ./RuntimeBenchmark --color on # until it gets white

#include </home/mosra/Code/mdspan/include/experimental/mdspan>
#include <Corrade/Containers/StridedArrayView.h>
#include <Corrade/TestSuite/Tester.h>

using namespace Corrade;

struct RuntimeBenchmark: TestSuite::Tester {
    explicit RuntimeBenchmark();

    void plainLoop();
    void stridedArrayView();
    void stridedArrayViewOptimized();
    void mdspan();
};

void blit(const std::experimental::basic_mdspan<const int, std::experimental::extents<std::experimental::dynamic_extent, std::experimental::dynamic_extent>, std::experimental::layout_stride<std::experimental::dynamic_extent, std::experimental::dynamic_extent>>& source, const std::experimental::basic_mdspan<int, std::experimental::extents<std::experimental::dynamic_extent, std::experimental::dynamic_extent>, std::experimental::layout_stride<std::experimental::dynamic_extent, std::experimental::dynamic_extent>>& destination) {
    for(std::ptrdiff_t i = 0; i != source.extent(0); ++i)
        for(std::ptrdiff_t j = 0; j != source.extent(1); ++j)
            destination(i, j) = source(i, j);
}

void blit(Containers::StridedArrayView2D<const int> source,
          Containers::StridedArrayView2D<int> destination) {
    for(std::size_t i = 0; i != source.size()[0]; ++i) {
        for(std::size_t j = 0; j != source.size()[1]; ++j)
            destination[i][j] = source[i][j];
    }
}

void blitOptimized(Containers::StridedArrayView2D<const int> source,
                   Containers::StridedArrayView2D<int> destination) {
    for(std::size_t i = 0; i != source.size()[0]; ++i) {
        Containers::StridedArrayView1D<const int> sourceRow = source[i];
        Containers::StridedArrayView1D<int> destinationRow = destination[i];
        for(std::size_t j = 0; j != sourceRow.size(); ++j)
            destinationRow[j] = sourceRow[j];
    }
}

RuntimeBenchmark::RuntimeBenchmark() {
    addBenchmarks({&RuntimeBenchmark::plainLoop,
                   &RuntimeBenchmark::stridedArrayView,
                   &RuntimeBenchmark::stridedArrayViewOptimized,
                   &RuntimeBenchmark::mdspan}, 50);
}

void RuntimeBenchmark::plainLoop() {
    const int source[100][100]{};
    int destination[100][100];

    CORRADE_BENCHMARK(100) {
        for(std::size_t i = 0; i != 100; ++i)
            for(std::size_t j = 0; j != 100; ++j)
                destination[i][j] = source[i][j];
    }

    CORRADE_COMPARE(destination[34][56], 0);
}

void RuntimeBenchmark::stridedArrayView() {
    const int sourceData[100*100]{};
    int destinationData[100*100];

    Containers::StridedArrayView2D<const int> source{sourceData, {100, 100}};
    Containers::StridedArrayView2D<int> destination{destinationData, {100, 100}};

    CORRADE_BENCHMARK(100) {
        blit(source, destination);
    }

    CORRADE_COMPARE(destination[34][56], 0);
}

void RuntimeBenchmark::stridedArrayViewOptimized() {
    const int sourceData[100*100]{};
    int destinationData[100*100];

    Containers::StridedArrayView2D<const int> source{sourceData, {100, 100}};
    Containers::StridedArrayView2D<int> destination{destinationData, {100, 100}};

    CORRADE_BENCHMARK(100) {
        blitOptimized(source, destination);
    }

    CORRADE_COMPARE(destination[34][56], 0);
}

void RuntimeBenchmark::mdspan() {
    const int sourceData[100*100]{};
    int destinationData[100*100];

    std::experimental::basic_mdspan<const int, std::experimental::extents<std::experimental::dynamic_extent, std::experimental::dynamic_extent>, std::experimental::layout_stride<std::experimental::dynamic_extent, std::experimental::dynamic_extent>> source{sourceData, std::array<std::ptrdiff_t, 2>{100, 100}};
    std::experimental::basic_mdspan<int, std::experimental::extents<std::experimental::dynamic_extent, std::experimental::dynamic_extent>, std::experimental::layout_stride<std::experimental::dynamic_extent, std::experimental::dynamic_extent>> destination{destinationData, std::array<std::ptrdiff_t, 2>{100, 100}};

    CORRADE_BENCHMARK(100) {
        blit(source, destination);
    }

    CORRADE_COMPARE(destination(34, 56), 0);
}


CORRADE_TEST_MAIN(RuntimeBenchmark)
