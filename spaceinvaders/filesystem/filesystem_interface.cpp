#include "filesystem_interface.h"
#include "cfilesystem.h"

CFileSystem _filesystem;
IFileSystem *filesystem = (IFileSystem *)&_filesystem;