include( "shared.lua" )

----------------------------------------------------
-- Hook:
--   InitPostEntity
-- Purpose:
--   Called after entities have been initialized.
----------------------------------------------------
hook.Add( "InitPostEntity", "InitChessBoard", function()
	-- Store local player entity
	StoredLocalPlayer = LocalPlayer()
	
	-- Tell the server the client has finished initializing entities
	RunConsoleCommand( "hl2chess_cl_initpostentity" )
end )