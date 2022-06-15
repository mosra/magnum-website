// g++ main.cpp -o generator -std=c++11 -lMagnumTextureTools -lMagnumDebugTools -lMagnum -lCorradeUtility && ./generator

#include <algorithm>
#include <random>
#include <Corrade/Containers/Array.h>
#include <Corrade/Containers/Pair.h>
#include <Corrade/Containers/StringStl.h>
#include <Corrade/Utility/FormatStl.h>
#include <Corrade/Utility/Path.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Range.h>
#include <Magnum/TextureTools/Atlas.h>
#include <Magnum/DebugTools/ColorMap.h>

using namespace Magnum;

int main() {
    constexpr Int maxSize = 7;
    constexpr Int padding = 4;
    constexpr Int layerPadding = 4;
    constexpr Float displaySizeDivisor = 1.5f;

    /* Generate a random set of textures and colors */
    std::random_device rd; // TODO some seed?!
    std::uniform_int_distribution<Int> sizeDist{2, maxSize};
    std::uniform_int_distribution<Int> colorDist{0, 256};

    /* Random set of texture sizes would be stupid, so have a say on how many
       is actually generated */
    std::size_t sizeCount[]{
        0,
        0,
        9, /* 4 */
        7, /* 8 */
        4, /* 16 */
        5, /* 32 */
        6, /* 64 */
        2, /* 128 */
    };
    std::size_t count = 0;
    for(std::size_t i: sizeCount)
        count += i;

    Containers::Array<Vector2i> sizes{NoInit, count};
    Containers::Array<Color4ub> colors{NoInit, count};
    Vector2i size;
    std::size_t currentSize = 0;
    std::size_t currentSizeCount = 0;
    for(std::size_t i = 0; i != count; ++i) {
        while(currentSizeCount >= sizeCount[currentSize]) {
            ++currentSize;
            currentSizeCount = 0;
        }

        ++currentSizeCount;

        sizes[i] = Vector2i{1 << currentSize};
        colors[i] = DebugTools::ColorMap::turbo()[colorDist(rd)];

        if(i) size.x() += padding;
        size.x() += sizes[i].x();
        size.y() = Math::max(size.y(), sizes[i].y());
    }

    /* Shuffle the sizes at least */
    std::shuffle(sizes.begin(), sizes.end(), rd);

    Containers::Pair<Int, Containers::Array<Vector3i>> packed = TextureTools::atlasArrayPowerOfTwo(Vector2i{1 << maxSize}, sizes);
    for(std::size_t i = 0; i != count; ++i) {
        Debug{} << Debug::color << colors[i] << Debug::packed << sizes[i] << Debug::packed << packed.second()[i];
    }

    Debug{} << "layer count:" << packed.first();

    /* Input */
    {
        Range2Di viewBox{{}, size};

        std::string out;
        Utility::formatInto(out, out.size(), R"(<svg class="m-image" style="width: {4}px; height: {5}px;" viewBox="{0} {1} {2} {3}" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
)",
            viewBox.left(), viewBox.bottom(), viewBox.sizeX(), viewBox.sizeY(), viewBox.sizeX()/displaySizeDivisor, viewBox.sizeY()/displaySizeDivisor);

        Int x = 0;
        for(std::size_t i = 0; i != count; ++i) {
            Utility::formatInto(out, out.size(), R"(  <rect x="{}" y="{}" width="{}" height="{}" style="fill:#{:.2x}{:.2x}{:.2x}"/>
)",
                x, (1 << maxSize) - sizes[i].y() - 1, sizes[i].x(), sizes[i].y(), colors[i].r(), colors[i].g(), colors[i].b());

            x += sizes[i].y() + padding;
        }

        Utility::formatInto(out, out.size(), R"(</svg>
)");

        CORRADE_INTERNAL_ASSERT_OUTPUT(Utility::Path::write("input.svg", Containers::StringView{out}));
    }

    /* Packed */
    {
        Range2Di viewBox{{}, {packed.first()*(1 << maxSize) + (packed.first() - 1)*layerPadding, 1 << maxSize}};

        std::string out;
        Utility::formatInto(out, out.size(), R"(<svg class="m-image" style="width: {4}px; height: {5}px;" viewBox="{0} {1} {2} {3}" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
)",
            viewBox.left(), viewBox.bottom(), viewBox.sizeX(), viewBox.sizeY(), viewBox.sizeX()/displaySizeDivisor, viewBox.sizeY()/displaySizeDivisor);

        for(std::size_t i = 0; i != packed.second().size(); ++i) {
            const Vector2i offset = packed.second()[i].xy();
            const Int layer = packed.second()[i].z();

            Utility::formatInto(out, out.size(), R"(  <rect x="{}" y="{}" width="{}" height="{}" style="fill:#{:.2x}{:.2x}{:.2x}"/>
)",
                layer*((1 << maxSize) + layerPadding) + offset.x(), (1 << maxSize) - sizes[i].y() - offset.y(), sizes[i].x(), sizes[i].y(), colors[i].r(), colors[i].g(), colors[i].b());
        }

        Utility::formatInto(out, out.size(), R"(</svg>
)");

        CORRADE_INTERNAL_ASSERT_OUTPUT(Utility::Path::write("packed.svg", Containers::StringView{out}));
    }
}

// [snippet]
Containers::Array<Vector3i> atlasArrayPowerOfTwo(
    const Vector2i& layerSize, Containers::ArrayView<const Vector2i> sortedSizes
) {
    Containers::Array<Vector3i> output{NoInit, sortedSizes.size()};

    /* Start with the whole first layer free */
    Int layer = 0;
    UnsignedInt free = 1;
    Vector2i previousSize = layerSize;
    for(std::size_t i = 0; i != sortedSizes.size(); ++i) {
        /* No free slots left, go to the next layer. Then, what's free, is one
           whole layer. */
        if(!free) {
            ++layer;
            free = 1;
            previousSize = layerSize;
        }

        /* Multiply number of free slots based on area difference from previous
           size. If the size is the same, nothing changes. */
        free *= (previousSize/sortedSizes[i]).product();

        /* Calculate slot index and coordinates from the slot index */
        const UnsignedInt sideSlotCount = layerSize.x()/sortedSizes[i].x();
        const UnsignedInt layerDepth = Math::log2(sideSlotCount);
        const UnsignedInt slotIndex = sideSlotCount*sideSlotCount - free;
        Vector2i coordinates;
        for(UnsignedInt i = 0; i < layerDepth; ++i) {
            if(slotIndex & (1 << 2*(layerDepth - i - 1)))
                coordinates.x() += layerSize.x() >> (i + 1);
            if(slotIndex & (1 << (2*(layerDepth - i - 1) + 1)))
                coordinates.y() += layerSize.y() >> (i + 1);
        }

        /* Write the final coordinates to the output */
        output[i] = {coordinates, layer};
        previousSize = sortedSizes[i];
        --free;
    }

    return output;
}
// [/snippet]
