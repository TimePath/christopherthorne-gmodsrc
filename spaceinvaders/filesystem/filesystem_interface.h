#ifndef FILESYSTEM_INTERFACE_H
#define FILESYSTEM_INTERFACE_H

#include "ifilesystem.h"

#ifdef FILESYSTEM
__declspec(dllexport) extern IFileSystem *filesystem;
#else
__declspec(dllimport) IFileSystem *filesystem;
#endif

#define FileSystem() filesystem

#endif // FILESYSTEM_INTERFACE_H