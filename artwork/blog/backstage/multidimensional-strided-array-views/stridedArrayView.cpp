#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <Corrade/Containers/StridedArrayView.h>
// #include <Magnum/Math/Color.h>

using namespace Corrade;

#ifdef Magnum_Math_Color_h
#include <Magnum/Magnum.h>
using namespace Magnum;
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

void blit(Containers::StridedArrayView2D<const Color3ub> source,
          Containers::StridedArrayView2D<Color3ub> destination) {
    CORRADE_INTERNAL_ASSERT(source.size() == destination.size());
    for(std::size_t i = 0; i != source.size()[0]; ++i)
        for(std::size_t j = 0; j != source.size()[1]; ++j)
            destination[i][j] = source[i][j];
}

int main() {
    Containers::StridedArrayView2D<Color3ub> pixels{imageData, {37, 37}};

    Containers::StridedArrayView1D<const Color3ub> pink{borderData};

    #if !defined(CORRADE_NO_DEBUG) && !defined(CORRADE_STANDARD_ASSERT)
    Utility::Debug{} << "It's pink!!" << Utility::Debug::color << pink;
    #endif

    Containers::StridedArrayView2D<const Color3ub> border =
        pink.slice<2>().broadcasted<1>(37);

    blit(border.transposed<0, 1>(), pixels.prefix({37, pink.size()}));

    /* Right */
    blit(border.transposed<0, 1>().flipped<1>(), pixels.suffix({0, 37 - pink.size()}));

    /* Bottom */
    blit(border, pixels.prefix({pink.size(), 37}));

    /* Top */
    blit(border.flipped<0>(), pixels.suffix({37 - pink.size(), 0}));

    #if !defined(CORRADE_NO_DEBUG) && !defined(CORRADE_STANDARD_ASSERT)
    Utility::Debug{} << Utility::Debug::color << Utility::Debug::packed << pixels.flipped<0>();
    #endif
}
