#include "Meshes.hpp"

namespace Velyra::SandBox {

    void calculateTangents(Mesh& mesh){
        for (int i = 0; i < mesh.indices.size(); i += 3) {
            auto& v0 = mesh.vertices[mesh.indices[i + 0]];
            auto& v1 = mesh.vertices[mesh.indices[i + 1]];
            auto& v2 = mesh.vertices[mesh.indices[i + 2]];

            const auto edge1 = v1.position - v0.position;
            const auto edge2 = v2.position - v0.position;

            const auto deltaUV1 = v1.texCoord - v0.texCoord;
            const auto deltaUV2 = v2.texCoord - v0.texCoord;

            const float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            auto tangent = glm::vec3();
            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            v0.tangent = tangent;
            v1.tangent = tangent;
            v2.tangent = tangent;
        }
    }

    Mesh createSphere(const U32 rings, U32 sectors){
        Mesh mesh;
        const float R = 1.0f / static_cast<float>(rings - 1);
        const float S = 1.0f / static_cast<float>(sectors - 1);

        for (U32 r = 0; r < rings; r++) {
            for(U32 s = 0; s < sectors; s++) {
                const float y = sin( -M_PI_2 + M_PI * r * R );
                const float x = cos(2 * M_PI * s * S) * sin( M_PI * r * R );
                const float z = sin(2 * M_PI * s * S) * sin( M_PI * r * R );

                auto position = glm::vec3(x, y, z);
                auto normal = glm::normalize(position);
                const auto tangent = glm::normalize(glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f)));
                const auto texCoord = glm::vec2(s * S, r * R);

                mesh.vertices.push_back({position, {x, y, z}, normal, tangent, texCoord});
            }
        }

        for (U32 r = 0; r < rings-1; r++) {
            for (U32 s = 0; s < sectors-1; s++) {
                mesh.indices.push_back(r * sectors + s);
                mesh.indices.push_back(r * sectors + (s + 1));
                mesh.indices.push_back((r + 1) * sectors + (s + 1));

                mesh.indices.push_back(r * sectors + s);
                mesh.indices.push_back((r + 1) * sectors + (s + 1));
                mesh.indices.push_back((r + 1) * sectors + s);
            }
        }
        calculateTangents(mesh);
        return mesh;
    }

    Mesh createCube(){
        Mesh cube;
        cube.vertices = {
                /// front
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// left
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// bottom
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// back
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// right
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// top
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        };
        cube.indices = {
            0, 3, 2,
            0, 2, 1,

            4, 7, 6,
            4, 6, 5,

            8, 11, 10,
            8, 10, 9,

            12, 15, 14,
            12, 14, 13,

            16, 19, 18,
            16, 18, 17,

            20, 23, 22,
            20, 22, 21
        };
        calculateTangents(cube);
        return cube;
    }

    Mesh createCone(const U32 sectors, const float height, const float radius){
        Mesh mesh;
        const float sectorStep = 2.0f * M_PI / sectors;

        // Apex vertex
        const glm::vec3 apex(0.0f, height, 0.0f);
        
        // Generate base vertices
        for (U32 i = 0; i <= sectors; i++) {
            const float angle = i * sectorStep;
            const float x = radius * cos(angle);
            const float z = radius * sin(angle);
            
            const glm::vec3 position(x, 0.0f, z);
            const glm::vec3 baseCenter(0.0f, 0.0f, 0.0f);
            const glm::vec3 toApex = glm::normalize(apex - position);
            const glm::vec3 outward = glm::normalize(position - baseCenter);
            const glm::vec3 normal = glm::normalize(toApex + outward);
            
            const glm::vec2 texCoord(static_cast<float>(i) / sectors, 1.0f);
            const glm::vec3 color(1.0f, 1.0f, 1.0f);
            
            mesh.vertices.push_back({position, color, normal, glm::vec3(0.0f), texCoord});
        }

        // Apex vertex
        const glm::vec3 apexNormal(0.0f, 1.0f, 0.0f);
        mesh.vertices.push_back({apex, glm::vec3(1.0f), apexNormal, glm::vec3(0.0f), glm::vec2(0.5f, 0.0f)});
        const U32 apexIndex = mesh.vertices.size() - 1;

        // Base center vertex
        const glm::vec3 baseCenter(0.0f, 0.0f, 0.0f);
        mesh.vertices.push_back({baseCenter, glm::vec3(1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.5f, 0.5f)});
        const U32 baseCenterIndex = mesh.vertices.size() - 1;

        // Side triangles
        for (U32 i = 0; i < sectors; i++) {
            mesh.indices.push_back(i);
            mesh.indices.push_back(apexIndex);
            mesh.indices.push_back(i + 1);
        }

        // Base triangles
        for (U32 i = 0; i < sectors; i++) {
            mesh.indices.push_back(baseCenterIndex);
            mesh.indices.push_back(i);
            mesh.indices.push_back(i + 1);
        }

        calculateTangents(mesh);
        return mesh;
    }

    Mesh createCylinder(const U32 sectors, const float height, const float radius){
        Mesh mesh;
        const float sectorStep = 2.0f * M_PI / sectors;

        // Generate top and bottom circles
        for (U32 i = 0; i <= sectors; i++) {
            const float angle = i * sectorStep;
            const float x = radius * cos(angle);
            const float z = radius * sin(angle);
            const float u = static_cast<float>(i) / sectors;

            // Top vertex
            const glm::vec3 topPos(x, height, z);
            const glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z));
            mesh.vertices.push_back({topPos, glm::vec3(1.0f), normal, glm::vec3(0.0f), glm::vec2(u, 0.0f)});

            // Bottom vertex
            const glm::vec3 bottomPos(x, 0.0f, z);
            mesh.vertices.push_back({bottomPos, glm::vec3(1.0f), normal, glm::vec3(0.0f), glm::vec2(u, 1.0f)});
        }

        // Center vertices for caps
        const glm::vec3 topCenter(0.0f, height, 0.0f);
        mesh.vertices.push_back({topCenter, glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.5f, 0.5f)});
        const U32 topCenterIndex = mesh.vertices.size() - 1;

        const glm::vec3 bottomCenter(0.0f, 0.0f, 0.0f);
        mesh.vertices.push_back({bottomCenter, glm::vec3(1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.5f, 0.5f)});
        const U32 bottomCenterIndex = mesh.vertices.size() - 1;

        // Side faces
        for (U32 i = 0; i < sectors; i++) {
            const U32 top1 = i * 2;
            const U32 bottom1 = i * 2 + 1;
            const U32 top2 = (i + 1) * 2;
            const U32 bottom2 = (i + 1) * 2 + 1;

            mesh.indices.push_back(top1);
            mesh.indices.push_back(bottom1);
            mesh.indices.push_back(top2);

            mesh.indices.push_back(top2);
            mesh.indices.push_back(bottom1);
            mesh.indices.push_back(bottom2);
        }

        // Top cap
        for (U32 i = 0; i < sectors; i++) {
            mesh.indices.push_back(topCenterIndex);
            mesh.indices.push_back(i * 2);
            mesh.indices.push_back((i + 1) * 2);
        }

        // Bottom cap
        for (U32 i = 0; i < sectors; i++) {
            mesh.indices.push_back(bottomCenterIndex);
            mesh.indices.push_back((i + 1) * 2 + 1);
            mesh.indices.push_back(i * 2 + 1);
        }

        calculateTangents(mesh);
        return mesh;
    }

    Mesh createTorus(const U32 majorSegments, const U32 minorSegments, const float majorRadius, const float minorRadius){
        Mesh mesh;

        for (U32 i = 0; i <= majorSegments; i++) {
            const float theta = 2.0f * M_PI * i / majorSegments;
            const float cosTheta = cos(theta);
            const float sinTheta = sin(theta);

            for (U32 j = 0; j <= minorSegments; j++) {
                const float phi = 2.0f * M_PI * j / minorSegments;
                const float cosPhi = cos(phi);
                const float sinPhi = sin(phi);

                const float x = (majorRadius + minorRadius * cosPhi) * cosTheta;
                const float y = minorRadius * sinPhi;
                const float z = (majorRadius + minorRadius * cosPhi) * sinTheta;

                const glm::vec3 position(x, y, z);

                // Normal calculation
                const glm::vec3 center(majorRadius * cosTheta, 0.0f, majorRadius * sinTheta);
                const glm::vec3 normal = glm::normalize(position - center);

                const glm::vec2 texCoord(
                    static_cast<float>(i) / majorSegments,
                    static_cast<float>(j) / minorSegments
                );

                mesh.vertices.push_back({position, glm::vec3(1.0f), normal, glm::vec3(0.0f), texCoord});
            }
        }

        // Generate indices
        for (U32 i = 0; i < majorSegments; i++) {
            for (U32 j = 0; j < minorSegments; j++) {
                const U32 first = i * (minorSegments + 1) + j;
                const U32 second = first + minorSegments + 1;

                mesh.indices.push_back(first);
                mesh.indices.push_back(second);
                mesh.indices.push_back(first + 1);

                mesh.indices.push_back(second);
                mesh.indices.push_back(second + 1);
                mesh.indices.push_back(first + 1);
            }
        }

        calculateTangents(mesh);
        return mesh;
    }

    Mesh createPlane(const U32 width, const U32 depth){
        Mesh mesh;

        // Generate vertices
        for (U32 z = 0; z <= depth; z++) {
            for (U32 x = 0; x <= width; x++) {
                const float xPos = static_cast<float>(x) / width - 0.5f;
                const float zPos = static_cast<float>(z) / depth - 0.5f;

                const glm::vec3 position(xPos, 0.0f, zPos);
                const glm::vec3 normal(0.0f, 1.0f, 0.0f);
                const glm::vec3 tangent(1.0f, 0.0f, 0.0f);
                const glm::vec2 texCoord(
                    static_cast<float>(x) / width,
                    static_cast<float>(z) / depth
                );

                mesh.vertices.push_back({position, glm::vec3(1.0f), normal, tangent, texCoord});
            }
        }

        // Generate indices
        for (U32 z = 0; z < depth; z++) {
            for (U32 x = 0; x < width; x++) {
                const U32 topLeft = z * (width + 1) + x;
                const U32 topRight = topLeft + 1;
                const U32 bottomLeft = (z + 1) * (width + 1) + x;
                const U32 bottomRight = bottomLeft + 1;

                mesh.indices.push_back(topLeft);
                mesh.indices.push_back(bottomLeft);
                mesh.indices.push_back(topRight);

                mesh.indices.push_back(topRight);
                mesh.indices.push_back(bottomLeft);
                mesh.indices.push_back(bottomRight);
            }
        }

        calculateTangents(mesh);
        return mesh;
    }

}
