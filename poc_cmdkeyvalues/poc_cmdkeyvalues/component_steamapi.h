#ifndef COMPONENT_STEAMAPI_H
#define COMPONENT_STEAMAPI_H

// Components
#include "component.h"

// Forward definitions
class ISteamUser009;

// Globals
extern ISteamUser009 *g_pSteamUser;

// Component registration
CREATE_COMPONENT( SteamAPI );

#endif // COMPONENT_STEAMAPI_H