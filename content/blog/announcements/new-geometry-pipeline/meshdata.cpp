#include <Corrade/Containers/Optional.h>
#include <Corrade/Utility/ConfigurationGroup.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/FunctionsBatch.h>
#include <Magnum/Math/Half.h>
#include <Magnum/Math/Range.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/GenerateNormals.h>
#include <Magnum/MeshTools/RemoveDuplicates.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Trade/AbstractSceneConverter.h>
#include <Magnum/Vk/Vulkan.h>

#pragma GCC diagnostic ignored "-Wunused-variable"

using namespace Magnum;

int main() {
{
UnsignedInt vertexCount{};
// packed
struct Vertex {
    Vector3h position;
    Vector2b normal;
    UnsignedShort:16;
    UnsignedShort objectId;
};

Containers::Array<char> indexData;
Containers::Array<char> vertexData;

Trade::MeshIndexData indices{MeshIndexType::UnsignedShort, indexData};
Trade::MeshData meshData{MeshPrimitive::Triangles,
    std::move(indexData), indices,
    std::move(vertexData), {
        Trade::MeshAttributeData{Trade::MeshAttribute::Position,
            VertexFormat::Vector3h, offsetof(Vertex, position),
            vertexCount, sizeof(Vertex)},
        Trade::MeshAttributeData{Trade::MeshAttribute::Normal,
            VertexFormat::Vector2bNormalized, offsetof(Vertex, normal),
            vertexCount, sizeof(Vertex)},
        Trade::MeshAttributeData{Trade::MeshAttribute::ObjectId,
            VertexFormat::UnsignedShort, offsetof(Vertex, objectId),
            vertexCount, sizeof(Vertex)}
    }
};
// packed

// compile
GL::Mesh mesh = MeshTools::compile(meshData);
Shaders::Phong{}.draw(mesh);
// compile

// access
Containers::StridedArrayView1D<const UnsignedShort> objectIds =
    meshData.attribute<UnsignedShort>(Trade::MeshAttribute::ObjectId);
for(UnsignedShort objectId: objectIds) {
    // …
}
// access

// asarray
Range3D aabb = Math::minmax(meshData.positions3DAsArray());
// asarray
}

{
// interleave
Containers::ArrayView<const Vector3h> positions;
Containers::ArrayView<const Vector2b> normals;
Containers::ArrayView<const UnsignedShort> objectIds;
Containers::ArrayView<const UnsignedShort> indices;

Trade::MeshData meshData = MeshTools::interleave(
    Trade::MeshData{MeshPrimitive::Triangles,
        {}, indices, Trade::MeshIndexData{indices}, UnsignedInt(positions.size())},
    {Trade::MeshAttributeData{Trade::MeshAttribute::Position, positions},
     Trade::MeshAttributeData{Trade::MeshAttribute::Normal, normals},
     Trade::MeshAttributeData{Trade::MeshAttribute::ObjectId, objectIds}}
);
// interleave
}

{
Trade::MeshData mesh{MeshPrimitive::Points, 0};
// meshtools-chained
Trade::MeshData optimized = MeshTools::compressIndices(
                                MeshTools::removeDuplicates(
                                    MeshTools::interleave(mesh)));
// meshtools-chained
}

{
Trade::MeshData mesh{MeshPrimitive::Points, 0};
PluginManager::Manager<Trade::AbstractSceneConverter> manager;
// meshoptimizer
Containers::Pointer<Trade::AbstractSceneConverter> meshoptimizer =
    manager.loadAndInstantiate("MeshOptimizerSceneConverter");

meshoptimizer->convertInPlace(mesh);
// meshoptimizer

// meshoptimizer-simplify
meshoptimizer->configuration().setValue("simplify", true);
meshoptimizer->configuration().setValue("simplifyTargetIndexCountThreshold", 0.5f);
Containers::Optional<Trade::MeshData> simplified = meshoptimizer->convert(mesh);
// meshoptimizer-simplify
}

{
Containers::StridedArrayView1D<const UnsignedInt> data;
// custom-format
Trade::MeshAttributeData normals{Trade::MeshAttribute::Normal,
    vertexFormatWrap(VK_FORMAT_A2R10G10B10_UNORM_PACK32), data};
// custom-format
}

{
// meshlets-struct
struct meshopt_Meshlet {
    unsigned int vertices[64];
    unsigned char indices[126][3];
    unsigned char triangle_count;
    unsigned char vertex_count;
};
// meshlets-struct

Containers::Array<char> meshletData;
UnsignedInt meshletCount{};
// meshlets-data
/* Pick any numbers that don't conflict with your other custom attributes */
constexpr auto Meshlet = meshPrimitiveWrap(0xabcd);
constexpr auto MeshletVertices = Trade::meshAttributeCustom(1);
constexpr auto MeshletIndices = Trade::meshAttributeCustom(2);
constexpr auto MeshletTriangleCount = Trade::meshAttributeCustom(3);
constexpr auto MeshletVertexCount = Trade::meshAttributeCustom(4);

Trade::MeshData meshlets{Meshlet, std::move(meshletData), {
    Trade::MeshAttributeData{MeshletVertices, VertexFormat::UnsignedInt,
        offsetof(meshopt_Meshlet, vertices), 0, sizeof(meshopt_Meshlet), 64},
    Trade::MeshAttributeData{MeshletIndices, VertexFormat::Vector3ub,
        offsetof(meshopt_Meshlet, indices), 0, sizeof(meshopt_Meshlet), 126},
    Trade::MeshAttributeData{MeshletTriangleCount, VertexFormat::UnsignedByte,
        offsetof(meshopt_Meshlet, triangle_count), 0, sizeof(meshopt_Meshlet)},
    Trade::MeshAttributeData{MeshletVertexCount, VertexFormat::UnsignedByte,
        offsetof(meshopt_Meshlet, vertex_count), 0, sizeof(meshopt_Meshlet)},
}, meshletCount};
// meshlets-data

std::size_t i = 0;
// meshlets-access
Containers::StridedArrayView1D<const UnsignedByte> triangleCount =
    meshlets.attribute<UnsignedByte>(MeshletTriangleCount);
Containers::StridedArrayView2D<const Vector3ub> indices =
    meshlets.attribute<Vector3ub[]>(MeshletIndices);
for(Vector3ub triangle: indices[i].prefix(triangleCount[i])) {
    // do something with each triangle of meshlet i …
}
// meshlets-access
}

{
Trade::MeshData chairData{MeshPrimitive::Points, 0},
    treeData{MeshPrimitive::Points, 0};
// shared-buffer
/* Shared for all meshes */
Containers::ArrayView<const char> indexData;
Containers::ArrayView<const char> vertexData;
GL::Buffer indices{indexData};
GL::Buffer vertices{indexData};

GL::Mesh chair = MeshTools::compile(chairData, indices, vertices);
GL::Mesh tree = MeshTools::compile(treeData, indices, vertices);
// …
// shared-buffer
}

}
