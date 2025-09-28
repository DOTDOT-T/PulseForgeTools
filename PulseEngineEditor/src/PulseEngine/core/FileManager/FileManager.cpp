#include "FileManager.h"

std::string FileManager::GetCollectionByExtension(const std::string& fileName)
{
    std::string collectionType = "guidCollection";
    
    if (fileName.substr(fileName.length() - 4) == ".mat") collectionType += "Materials";
    if (fileName.substr(fileName.length() - 6) == ".pmesh") collectionType += "Meshes";
    if (fileName.substr(fileName.length() - 8) == ".pEntity") collectionType += "Entities";

    collectionType += ".puid";

    return collectionType;
}