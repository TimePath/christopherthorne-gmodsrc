#ifndef GL_KEYVALUES_H
#define GL_KEYVALUES_H

#include "GMLuaModuleEx.h"
#include "keyvalues.h"

EXT_META_ID( KeyValues, 2 );

EXT_META_FUNCTION( KeyValues, GetName );
EXT_META_FUNCTION( KeyValues, SetName );

EXT_META_FUNCTION( KeyValues, FindKey );
EXT_META_FUNCTION( KeyValues, CreateNewKey );
EXT_META_FUNCTION( KeyValues, AddSubKey );
EXT_META_FUNCTION( KeyValues, RemoveSubKey );

EXT_META_FUNCTION( KeyValues, GetFirstSubKey );
EXT_META_FUNCTION( KeyValues, GetNextKey );
EXT_META_FUNCTION( KeyValues, SetNextKey );

EXT_META_FUNCTION( KeyValues, GetFirstTrueSubKey );
EXT_META_FUNCTION( KeyValues, GetNextTrueSubKey );

EXT_META_FUNCTION( KeyValues, GetFirstValue );
EXT_META_FUNCTION( KeyValues, GetNextValue );

EXT_META_FUNCTION( KeyValues, GetInt );
EXT_META_FUNCTION( KeyValues, SetInt );

EXT_META_FUNCTION( KeyValues, GetFloat );
EXT_META_FUNCTION( KeyValues, SetFloat );

EXT_META_FUNCTION( KeyValues, GetString );
EXT_META_FUNCTION( KeyValues, SetString );

EXT_META_FUNCTION( KeyValues, GetColor );
EXT_META_FUNCTION( KeyValues, SetColor );

EXT_META_FUNCTION( KeyValues, IsEmpty );

#endif // GL_KEYVALUES_H