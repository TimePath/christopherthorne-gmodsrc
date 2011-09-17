AddCSLuaFile( "cl_init.lua" )
AddCSLuaFile( "shared.lua" )

include( "shared.lua" )
include( "sv_anim.lua" )
include( "sv_navigation.lua" )
include( "sv_promotion.lua" )
include( "sv_main.lua" )

----------------------------------------------------
-- Hook:
--   InitPostEntity
-- Purpose:
--   Called after entities have been initialized.
----------------------------------------------------
function GM:InitPostEntity()
	for k, v in pairs( CHESS_PIECE_INFO ) do
		for k2, v2 in pairs( v.spawns ) do
			local x = v2.x
			local y = { [CHESSTEAMTYPE_VILLAINS] = 7 - v2.y, [CHESSTEAMTYPE_HEROES] = v2.y }

			for i = CHESSTEAMTYPE_MIN, CHESSTEAMTYPE_MAX do			
				-- Precache models (for clientside ragdolls)
				for k2, v2 in pairs( v[i] ) do			
					util.PrecacheModel( v2.model )
				end
			end
		end
	end
end

----------------------------------------------------
-- Hook:
--   PlayerLoadout
-- Purpose:
--   Called to give players weapons.
----------------------------------------------------
function GM:PlayerLoadout( ply )
	return true
end

----------------------------------------------------
-- Hook:
--   PlayerSelectSpawn
-- Purpose:
--   Determines player spawn point.
----------------------------------------------------
function GM:PlayerSelectSpawn( ply )
	-- Gets rid of the warning about no spawn points
	return GetWorldEntity()
end

----------------------------------------------------
-- Hook:
--   PlayerDisconnected
-- Purpose:
--   Called when a player leaves the server.
----------------------------------------------------
function GM:PlayerDisconnected( ply )
	-- Waiting player has left
	if ( ply == CHESS_PLAYER_WAITING ) then
		CHESS_PLAYER_WAITING = nil
		return
	end
	
	-- Chess player has left
	for k, v in pairs( CHESS_PLAYERS ) do
		if ( ply == v ) then
			EndChessGame()
			return
		end
	end
end

----------------------------------------------------
-- Hook:
--   PlayerSpawn
-- Purpose:
--   Called when a player spawns.
----------------------------------------------------
function GM:PlayerSpawn( ply )
	-- Prevent players colliding with each other
	ply:SetCollisionGroup( COLLISION_GROUP_DEBRIS )
end