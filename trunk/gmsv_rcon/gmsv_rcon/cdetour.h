#ifndef CDETOUR_H
#define CDETOUR_H

#include "detours/include/detours.h"
#include "CRConServer.h"
#include "CServerRemote.h"

#define _DEFINE1(c, ret, name, args) \
	ret c##_##name##_H##args; \
	static ret (CDetour::* c##_##name##_T)args

#define _DEFINE2(c, ret, name, args) \
	typedef ret (CDetour::* *##c##_##name##_t)args

#define _DEFINE3(c, ret, name, args) \
	ret (CDetour::* CDetour::##c##_##name##_T)args = NULL

class CDetour
{
public:
	_DEFINE1(CServerRemoteAccess, void, WriteDataRequest, (CRConServer *, unsigned int, const void *, int));
	_DEFINE1(CServerRemoteAccess, void, LogCommand, (unsigned int, const char *));
	_DEFINE1(CServerRemoteAccess, void *, CheckPassword, (CRConServer *, unsigned int, int, const char *));

	_DEFINE1(CRConServer, bool, IsPassword, (const char *));
};

_DEFINE2(CServerRemoteAccess, void, WriteDataRequest, (CRConServer *, unsigned int, const void *, int));
_DEFINE2(CServerRemoteAccess, void, LogCommand, (unsigned int, const char *));
_DEFINE2(CServerRemoteAccess, void *, CheckPassword, (CRConServer *, unsigned int, int, const char *));

_DEFINE2(CRConServer, bool, IsPassword, (const char *));

#endif //CDETOUR_H