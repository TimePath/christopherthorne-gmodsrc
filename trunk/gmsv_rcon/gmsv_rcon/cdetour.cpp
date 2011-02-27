#include "cdetour.h"

_DEFINE3(CServerRemoteAccess, void, WriteDataRequest, (CRConServer *, unsigned int, const void *, int));
_DEFINE3(CServerRemoteAccess, void, LogCommand, (unsigned int, const char *));
_DEFINE3(CServerRemoteAccess, void *, CheckPassword, (CRConServer *, unsigned int, int, const char *));

_DEFINE3(CRConServer, bool, IsPassword, (const char *));