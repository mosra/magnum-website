#include <Corrade/Containers/Optional.h>
#include <Corrade/Containers/StridedArrayView.h>
#include <Corrade/Utility/Arguments.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

using namespace Magnum;
using namespace Magnum::Math::Literals;

namespace {

// blit
void blit(Containers::StridedArrayView2D<const Color3ub> source,
          Containers::StridedArrayView2D<Color3ub> destination) {
    CORRADE_INTERNAL_ASSERT(source.size() == destination.size());
    for(std::size_t i = 0; i != source.size()[0]; ++i)
        for(std::size_t j = 0; j != source.size()[1]; ++j)
            destination[i][j] = source[i][j];
}
// /blit

}

int main(int argc, char** argv) {
    Utility::Arguments args;
    args.addArgument("image")
        .parse(argc, argv);

    PluginManager::Manager<Trade::AbstractImporter> manager;
    Containers::Pointer<Trade::AbstractImporter> importer = manager.loadAndInstantiate("AnyImageImporter");

    importer->openFile(args.value("image"));
    Containers::Optional<Trade::ImageData2D> image_ = importer->image2D(0);

    Trade::ImageData2D& image = *image_;

    Debug{} << "[wait-what]";
// wait-what
Debug{Debug::Flag::Color} << "This is green" << 0x3bd267_rgb
    << "and this greyscale" << UnsignedByte(0x66) << UnsignedByte(0xcc);
// /wait-what
    Debug{} << "[/wait-what]";

    Debug{} << "[image-data]";
// image-data
Debug{} << image.data().prefix(300);
// /image-data
    Debug{} << "[/image-data]";

    Debug{} << "[image-data-hexcolor]";
// image-data-hexcolor
Debug{} << Containers::arrayCast<Color3ub>(image.data().prefix(300));
// /image-data-hexcolor
    Debug{} << "[/image-data-hexcolor]";

    Debug{} << "[image-data-color]";
// image-data-color
Debug{} << Debug::color << Debug::packed
    << Containers::arrayCast<Color3ub>(image.data().prefix(1500));
// /image-data-color
    Debug{} << "[/image-data-color]";

    Debug{} << "[image-pixels]";
// image-pixels
Containers::StridedArrayView2D<Color3ub> pixels =
    Containers::arrayCast<2, Color3ub>(image.pixels());

Debug{} << Debug::color << Debug::packed << pixels;
// /image-pixels
    Debug{} << "[/image-pixels]";

// image-center
Containers::StridedArrayView2D<Color3ub> center =
    pixels.flipped<0>().slice({9, 9}, {29, 29});
// /image-center

    Debug{} << "[image-right]";
Debug{} << Debug::color << Debug::packed <<
// image-right
center.flipped<0>()
  .transposed<0, 1>();
// /image-right
    Debug{} << "[/image-right]";

    Debug{} << "[image-top]";
Debug{} << Debug::color << Debug::packed <<
// image-top
center
    ;
// /image-top
    Debug{} << "[/image-top]";

    Debug{} << "[image-left]";
Debug{} << Debug::color << Debug::packed <<
// image-left
center.flipped<1>()
  .transposed<0, 1>();
// /image-left
    Debug{} << "[/image-left]";

    Debug{} << "[border]";
// border
constexpr Color3ub borderData[]{
    0xe288ba_rgb, 0xeab6e7_rgb, 0xf5d4dc_rgb
};
Containers::StridedArrayView1D<const Color3ub> pink{borderData};

Debug{} << "It's pink!!" << Debug::color << pink;
// /border
    Debug{} << "[/border]";

    Debug{} << "[broadcast]";
// broadcast
Containers::StridedArrayView2D<const Color3ub> border =
    pink.slice<2>().broadcasted<1>(image.size().x());

Debug{} << Debug::color << Debug::packed << border;
// /broadcast
    Debug{} << "[/broadcast]";

    Debug{} << "[result]";
// result
/* Left */
blit(border.transposed<0, 1>(),
     pixels.prefix({image.size().y(), pink.size()}));

/* Right */
blit(border.transposed<0, 1>().flipped<1>(),
     pixels.suffix({0, image.size().x() - pink.size()}));

/* Bottom */
blit(border,
     pixels.prefix({pink.size(), image.size().x()}));

/* Top */
blit(border.flipped<0>(),
     pixels.suffix({image.size().y() - pink.size(), 0}));

Debug{} << Debug::color << Debug::packed << pixels.flipped<0>();
// /result
    Debug{} << "[/result]";
}
