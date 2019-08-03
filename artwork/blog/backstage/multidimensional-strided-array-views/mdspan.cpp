#include </home/mosra/Code/mdspan/include/experimental/mdspan>
// #include <Magnum/Math/Color.h>

#ifdef Magnum_Math_Color_h
#include <Magnum/Magnum.h>
using namespace Magnum;
using namespace Corrade;
#else
struct Color3ub {
    unsigned char r, g, b;
};
#endif

Color3ub imageData[37*37]{};

constexpr Color3ub borderData[]{
    {0xe2, 0x88, 0xba},
    {0xea, 0xb6, 0xe7},
    {0xf5, 0xd4, 0xdc}
};

void blit(const std::experimental::basic_mdspan<const Color3ub, std::experimental::extents<std::experimental::dynamic_extent, std::experimental::dynamic_extent>, std::experimental::layout_stride<std::experimental::dynamic_extent, std::experimental::dynamic_extent>>& source, const std::experimental::basic_mdspan<Color3ub, std::experimental::extents<std::experimental::dynamic_extent, std::experimental::dynamic_extent>, std::experimental::layout_stride<std::experimental::dynamic_extent, std::experimental::dynamic_extent>>& destination) {
    for(std::ptrdiff_t i = 0; i != source.extent(0); ++i)
        for(std::ptrdiff_t j = 0; j != source.extent(1); ++j)
            destination(i, j) = source(i, j);
}

int main() {
    //Containers::StridedArrayView2D<Color3ub> pixels{imageData, {37, 37}};
    std::experimental::basic_mdspan<Color3ub, std::experimental::extents<std::experimental::dynamic_extent, std::experimental::dynamic_extent>, std::experimental::layout_stride<std::experimental::dynamic_extent, std::experimental::dynamic_extent>> pixels{imageData, std::array<std::ptrdiff_t, 2>{37, 37}};

    //Containers::StridedArrayView1D<const Color3ub> pink{borderData};
    //Containers::StridedArrayView2D<const Color3ub> border =
    //  pink.slice<2>().broadcasted<1>(37);
    std::experimental::basic_mdspan<const Color3ub, std::experimental::extents<std::experimental::dynamic_extent, std::experimental::dynamic_extent>, std::experimental::layout_stride<std::experimental::dynamic_extent, std::experimental::dynamic_extent>> border{borderData, std::experimental::layout_stride<std::experimental::dynamic_extent, std::experimental::dynamic_extent>::template mapping<std::experimental::extents<std::experimental::dynamic_extent, std::experimental::dynamic_extent>>(std::experimental::extents<std::experimental::dynamic_extent, std::experimental::dynamic_extent>(3, 37), std::array<std::ptrdiff_t, 2>{sizeof(Color3ub), 0})};

    /* I gave up on finding out equivalents to transposed() / flipped().
       It would be probably something like mapBroadcast above but with
       numbers shuffled around. */

    //blit(border.transposed<0, 1>(), pixels.prefix({37, pink.size()}));
    blit(border, std::experimental::subspan(pixels, std::pair<int, int>(0, 37), std::pair<int, int>(0, 3)));

    //blit(border.transposed<0, 1>().flipped<1>(), pixels.suffix({0, 37 - pink.size()}));
    blit(border, std::experimental::subspan(pixels, std::pair<int, int>(0, 37), std::pair<int, int>(34, 37)));

    //blit(border, pixels.prefix({pink.size(), 37}));
    blit(border, std::experimental::subspan(pixels, std::pair<int, int>(0, 3), std::pair<int, int>(0, 37)));

    //blit(border.flipped<0>(), pixels.suffix({37 - pink.size(), 0}));
    blit(border, std::experimental::subspan(pixels, std::pair<int, int>(34, 37), std::pair<int, int>(0, 37)));
}
