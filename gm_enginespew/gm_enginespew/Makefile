# GMod Lua module Makefile

SOURCE_DIR = ../../../Valve/SDK/2007

MODULE_COMPILER = g++
MODULE_DEPENDENCIES = \
	tier0_i486.so

MODULE_NAME = gm_enginespew

$(MODULE_NAME)_i486.dll : $(MODULE_NAME)_i486.obj $(MODULE_DEPENDENCIES)
	$(MODULE_COMPILER) -fPIC -shared -o ../Release/$(MODULE_NAME)_i486.dll $(MODULE_NAME)_i486.obj -lm -ldl $(MODULE_DEPENDENCIES) $(MODULE_DEPENDENCIES)

$(MODULE_NAME)_i486.obj : $(MODULE_NAME).cpp
	$(MODULE_COMPILER) -c $(MODULE_NAME).cpp -o $(MODULE_NAME)_i486.obj -I$(SOURCE_DIR) -I$(SOURCE_DIR)/public -I$(SOURCE_DIR)/public/tier0 -I$(SOURCE_DIR)/public/tier1 -I$(SOURCE_DIR)/public/game/server -I$(SOURCE_DIR)/game/shared -I$(SOURCE_DIR)/game/server -I$(SOURCE_DIR)/game/client -I$(SOURCE_DIR)/common -DVPROF_LEVEL=1 -DSWDS -D_LINUX -DLINUX -DNDEBUG -fpermissive -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp -D_snprintf=snprintf -D_vsnprintf=vsnprintf -D_alloca=alloca -Dstrcmpi=strcasecmp
