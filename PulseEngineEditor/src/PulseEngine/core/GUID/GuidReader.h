#ifndef GUIDREADER_H
#define GUIDREADER_H

#include "Common/common.h"
#include "dllExport.h"

class Entity;
class Mesh;

class MA_DLL_API GuidReader
{
    public:
        static Entity* GetEntityFromGuid(std::size_t Guid);
        static Mesh* GetMeshFromGuid(std::size_t guid);
        static std::size_t InsertIntoCollection(const std::string& filePath);
};

#endif