#include <netadr.h>
#include <utllinkedlist.h>

struct CServerRemoteListener //TODO
{
	void *unknown;
	netadr_s addr;
};

class CServerRemoteAccess
{
public:
	void *unknown1;
	void *unknown2;
	void *unknown3;
	void *unknown4;
	void *unknown5;
	void *unknown6;
	void *unknown7;
	void *unknown8;
	void *unknown9;
	void *unknown10;
	
	CUtlLinkedList<CServerRemoteListener *, unsigned int> m_Listeners;
};