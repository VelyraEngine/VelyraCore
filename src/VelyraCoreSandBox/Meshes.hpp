#pragma once

#include "Utils.hpp"

namespace Velyra::SandBox {

    Mesh createSphere(U32 rings, U32 sectors);

    Mesh createCube();

    Mesh createCone(U32 sectors, float height = 1.0f, float radius = 0.5f);

    Mesh createCylinder(U32 sectors, float height = 1.0f, float radius = 0.5f);

    Mesh createTorus(U32 majorSegments, U32 minorSegments, float majorRadius = 0.75f, float minorRadius = 0.25f);

    Mesh createPlane(U32 width = 1, U32 depth = 1);

}
