#include "PulseEngine/core/PulseObject/PulseObject.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"

#include <iostream>


int main()
{
    std::cout << GenerateGUIDFromPath("helloteam") << std::endl;
    std::cout << GenerateGUIDFromPathAndMap("helloteam", "map_01") << std::endl;

    std::cin.get();

    return 0;
}