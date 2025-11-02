//this main is a simple test to experiment if the dlls are working fines

// #include "Windows.h"
#include "PulseEngine/core/Math/Frustum/Frustum.h"
#include "PulseEngine/core/Math/Frustum/AABB.h"
#include "PulseEngine/core/Math/Vector.h"

int main()
{
    Frustum* frust = new Frustum;
    AABB collideTest = AABB(PulseEngine::Vector3(10.0f), PulseEngine::Vector3(15.0f));
    std::cout << frust->IntersectsAABB(collideTest) << std::endl;

    std::cin.get();

    delete frust;

    return 0;
}