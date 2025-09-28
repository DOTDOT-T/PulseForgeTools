#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Common/common.h"
#include "dllExport.h"


class MA_DLL_API FileManager
{
private:

public:
    static std::string GetCollectionByExtension(const std::string& fileName);
};


#endif