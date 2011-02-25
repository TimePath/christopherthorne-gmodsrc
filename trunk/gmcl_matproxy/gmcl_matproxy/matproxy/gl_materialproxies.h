#ifndef GL_MATERIALPROXIES_H
#define GL_MATERIALPROXIES_H

#define CLIENT_DLL

#include "imaterialsystem.h"
#include "imaterialproxy.h"
#include "imaterialproxyfactory.h"
#include "GMLuaModuleEx.h"

extern IMaterialSystem *g_pMaterialSystem;

extern IMaterialProxyFactory *g_pOldProxyFactory;
extern IMaterialProxyFactory *g_pNewProxyFactory;

extern int g_CurrentCallbackTable;

extern CUtlVector<int> g_LuaMaterialReferences;

///////////////////////////////////////////
/////////// Proxy Object Base /////////////
///////////////////////////////////////////

class CLuaMaterialProxyObject
{
public:
	const char *GetName();
	void SetName( const char *pszName );

	void Release();

private:
	char m_szName[256];
};

///////////////////////////////////////////
//////// CLuaMaterialProxyCallback ////////
///////////////////////////////////////////

class CLuaMaterialProxyCallback : public CLuaMaterialProxyObject
{
public:
	void CreateReferences( ILuaObject *metaCallback ); // Create Lua function references
	void FreeReferences(); // Free Lua function references

public:
	int m_Table; // Function table
	int m_fnInit; // Init function
	int m_fnOnBind; // OnBind function
	int m_fnGetMaterial; // GetMaterial function
};

///////////////////////////////////////////
//////////// CLuaMaterialProxy ////////////
///////////////////////////////////////////

class CLuaMaterialProxy : public CLuaMaterialProxyObject
{
public:
	void Add( CLuaMaterialProxyCallback *pCallback ); // Register callback
	void Remove( CLuaMaterialProxyCallback *pCallback ); // Unregister callback

public:
	CUtlVector<CLuaMaterialProxyCallback *> m_Callbacks; // Lua callbacks
};

///////////////////////////////////////////
/////////// CBaseMaterialProxy ////////////
///////////////////////////////////////////

class CBaseMaterialProxy : public IMaterialProxy
{
public:
	CBaseMaterialProxy( CLuaMaterialProxy *pLuaProxy );

	bool Init( IMaterial *pMaterial, KeyValues *pKeyValues );
	void OnBind( void *pArg );
	void Release() { delete this; };
	IMaterial *GetMaterial();

public:
	CLuaMaterialProxy *m_pLuaProxy;
};

extern CUtlVector<CLuaMaterialProxy *> g_LuaMaterialProxies;

#endif // GL_MATERIALPROXIES_H