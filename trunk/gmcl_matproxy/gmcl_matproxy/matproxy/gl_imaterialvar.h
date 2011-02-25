#ifndef GL_IMATERIALVAR_H
#define GL_IMATERIALVAR_H

#include "GMLuaModuleEx.h"
#include "imaterialvar.h"

EXT_META_ID( IMaterialVar, 1 );

EXT_META_FUNCTION( IMaterialVar, GetName );

EXT_META_FUNCTION( IMaterialVar, GetTextureValue );
EXT_META_FUNCTION( IMaterialVar, SetTextureValue );

EXT_META_FUNCTION( IMaterialVar, GetFloatValue );
EXT_META_FUNCTION( IMaterialVar, SetFloatValue );

EXT_META_FUNCTION( IMaterialVar, GetIntValue );
EXT_META_FUNCTION( IMaterialVar, SetIntValue );

EXT_META_FUNCTION( IMaterialVar, GetStringValue );
EXT_META_FUNCTION( IMaterialVar, SetStringValue );

EXT_META_FUNCTION( IMaterialVar, GetMaterialValue );
EXT_META_FUNCTION( IMaterialVar, SetMaterialValue );

EXT_META_FUNCTION( IMaterialVar, GetVecValue );
EXT_META_FUNCTION( IMaterialVar, SetVecValue );
EXT_META_FUNCTION( IMaterialVar, SetVecComponentValue );

EXT_META_FUNCTION( IMaterialVar, GetMatrixValue );
EXT_META_FUNCTION( IMaterialVar, SetMatrixValue );

EXT_META_FUNCTION( IMaterialVar, VectorSize );

EXT_META_FUNCTION( IMaterialVar, IsDefined );
EXT_META_FUNCTION( IMaterialVar, SetUndefined );

EXT_META_FUNCTION( IMaterialVar, GetOwningMaterial );
EXT_META_FUNCTION( IMaterialVar, GetType );
EXT_META_FUNCTION( IMaterialVar, IsTexture );

#endif // GL_IMATERIALVAR_H