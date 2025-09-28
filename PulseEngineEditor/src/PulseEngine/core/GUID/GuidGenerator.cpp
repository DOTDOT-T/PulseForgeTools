#include <iostream>
#include <random>
#include "PulseEngine/core/GUID/GuidGenerator.h"

#include <string>
#include <functional>
#include "Common/common.h"


std::size_t GenerateGUIDFromPath(const std::string& filepath)
{
    std::hash<std::string> hasher;
    return hasher(filepath);  // returns same value every time for the same path
}
