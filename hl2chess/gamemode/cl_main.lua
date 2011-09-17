-- Client chess team
CL_CHESS_TEAM = 0

----------------------------------------------------
-- Function:
--   SetChessTurnType
-- Purpose:
--   Current turn type.
----------------------------------------------------
function SetChessTurnType( turn_type )
	if ( turn_type == CHESSTURNTYPE_YOU ) then		
		SetChessMessage( "It's your move!", false )
	elseif ( turn_type == CHESSTURNTYPE_OPPONENT ) then
		SetChessMessage( "Waiting for opponent to move", true )
	elseif ( turn_type == CHESSTURNTYPE_PENDING ) then
		SetChessMessage( "", true )
	end
	
	CL_CHESS_TURN = turn_type
end

----------------------------------------------------
-- Function:
--   CreateChessEntity
-- Purpose:
--   Creates a chess entity with the given info.
----------------------------------------------------
function CreateChessEntity( mdl_data, x, y, yaw )
	-- Create entity
	local ent = ClientsideModel( mdl_data.model )
	ent:SetPos( BoardToWorldPosition( x, y ) )
	ent:SetAngles( Angle( 0, yaw, 0 ) )
	ent:Spawn()
				
	-- Movement speed
	ent.chess_move_speed = mdl_data.move_speed
				
	-- Animation data
	ent.idle_sequence = ent:LookupSequence( mdl_data.idle )
	ent.walk_sequence = ent:LookupSequence( mdl_data.walk )
	ent.attack_sequences = mdl_data.attack
	ent.attack_distance = mdl_data.attack_distance
				
	-- Sounds
	ent.death_sound = mdl_data.death_sound

	-- Footsteps
	if ( mdl_data.footsteps ) then
		ent.footsteps = mdl_data.footsteps
		ent.footstep_interval = mdl_data.footstep_interval
		ent.footstep_update = 0
		ent.footstep_pos = 1	
		ent.footstep_count = #mdl_data.footsteps
	end

	-- Start idle sequence
	RunChessIdleSequence( ent )
	
	return ent
end

----------------------------------------------------
-- User Message:
--   hl2chess_joingame
-- Purpose:
--   Let client know they can join the game.
----------------------------------------------------
usermessage.Hook( "hl2chess_joingame", function( um )
	SetChessMessage( "Press F1 to play." )
end )

----------------------------------------------------
-- User Message:
--   hl2chess_initgame
-- Purpose:
--   A game has begun.
----------------------------------------------------
usermessage.Hook( "hl2chess_initgame", function( um )
	local team_index = um:ReadChar()
	
	--print( "hl2chess_initgame: " .. TeamIndexToName( team_index ) )

	-- Store client team
	CL_CHESS_TEAM = team_index

	-- Create chess board
	NewChessBoard()

	-- Create chess piece entities
	for k, v in pairs( CHESS_PIECE_INFO ) do
		for k2, v2 in pairs( v.spawns ) do
			local x = v2.x	
			local y = { [CHESSTEAMTYPE_VILLAINS] = 7 - v2.y, [CHESSTEAMTYPE_HEROES] = v2.y }
			local yaw = { [CHESSTEAMTYPE_VILLAINS] = 270, [CHESSTEAMTYPE_HEROES] = 90 }

			for i = CHESSTEAMTYPE_MIN, CHESSTEAMTYPE_MAX do
				-- Colour to use for rendering
				if ( i == CL_CHESS_TEAM ) then
					color = COLOR_GREEN
				else
					color = COLOR_RED
				end

				-- Select a random model
				local ent = CreateChessEntity( table.Random( v[i] ), x, y[i], yaw[i] )

				-- Update board
				CHESS_BOARD_MAP[x][y[i]] = { ent = ent, cteam = i, ctype = k, cteam_color = color }
			end
		end
	end

	-- Set client view
	SetupClientView()

	-- Check if the user goes first
	if ( CL_CHESS_TEAM == CHESS_START_TEAM ) then
		SetChessTurnType( CHESSTURNTYPE_YOU )
	else
		SetChessTurnType( CHESSTURNTYPE_OPPONENT )
	end

	-- Display cursor
	gui.EnableScreenClicker( true )
	
	-- Game is now active
	CHESS_GAME_ACTIVE = true
end )

----------------------------------------------------
-- User Message:
--   hl2chess_endgame
-- Purpose:
--   The game has come to an end.
----------------------------------------------------
usermessage.Hook( "hl2chess_endgame", function( um )
	-- Winning team
	local team = um:ReadChar()

	if ( team == -1 ) then
		SetChessMessage( "Game aborted. Press F1 to play again." )
	elseif ( team == CL_CHESS_TEAM ) then
		SetChessMessage( "You win! Press F1 to play again." )
	else
		SetChessMessage( "You lose! Press F1 to play again." )
	end
	
	-- Remove chess board entities
	for x, t in pairs( CHESS_BOARD_MAP )  do
		for y, v in pairs( t ) do
			if ( ValidEntity( v.ent ) ) then
				v.ent:Remove()
			end
		end
	end

	-- Hide cursor
	gui.EnableScreenClicker( false )
	
	-- Reset variables
	CHESS_GAME_ACTIVE = false
	CHESS_BOARD_MAP = nil
	CL_CHESS_TEAM = nil
	CL_CHESS_VIEW = nil
end )

----------------------------------------------------
-- User Message:
--   hl2chess_pendingopponent
-- Purpose:
--   Client is waiting for an opponent.
----------------------------------------------------
usermessage.Hook( "hl2chess_pendingopponent", function( um )
	-- Let the user know they are waiting
	SetChessMessage( "Waiting for opponent", true )
end )

----------------------------------------------------
-- User Message:
--   hl2chess_yourturn
-- Purpose:
--   It's the client's turn.
----------------------------------------------------
usermessage.Hook( "hl2chess_turn", function( um )
	SetChessTurnType( um:ReadChar() )
end )

----------------------------------------------------
-- User Message:
--   hl2chess_board_update
-- Purpose:
--   Handles board updates.
----------------------------------------------------
usermessage.Hook( "hl2chess_board_update", function( um )
	local src_x = um:ReadChar()
	local src_y = um:ReadChar()
	local dst_x = um:ReadChar()
	local dst_y = um:ReadChar()
	
	-- Move entity at source to destination
	MoveChessPieceEntity( src_x, src_y, dst_x, dst_y )

	-- Update board
	CHESS_BOARD_MAP[dst_x][dst_y] = CHESS_BOARD_MAP[src_x][src_y]
	CHESS_BOARD_MAP[src_x][src_y] = nil
	
	-- Set pending turn
	SetChessTurnType( CHESSTURNTYPE_PENDING )
end )

----------------------------------------------------
-- Function:
--   PawnPromotionEffect
-- Purpose:
--   Launches a pawn into the sky.
----------------------------------------------------
function PawnPromotionEffect( x, y )
	-- Entity at specified position
	local ent = CHESS_BOARD_MAP[x][y].ent

	-- z-coordinate, for rising
	local dz = 0.1
	-- yaw, for spinning
	local dyaw = 5

	hook.Add( "Think", "FlyAway", function()
		-- Move
		ent:SetRenderOrigin( ent:GetPos() + Vector( 0, 0, dz ) )
		-- Spin
		ent:SetRenderAngles( ent:GetAngles() + Angle( 0, dyaw, 0 ) )
		
		-- Increase rate of rising
		dz = dz * 1.5
		-- Increase spin angle
		dyaw = dyaw + 5
		
		-- Stop once above the viewing position
		if ( dz > CL_CHESS_CAM_Z ) then
			hook.Remove( "Think", "FlyAway" )
		end
	end )
end

usermessage.Hook( "hl2chess_promote_piece", function( um )
	local x = um:ReadChar()
	local y = um:ReadChar()
	local mine = um:ReadBool()
	
	--print( "promoting pawn: ", x, y, mine )

	if ( mine ) then
		ShowPromotionMenu( x, y )
	else
		SetChessMessage( "Waiting for opponent to promote their pawn", true )
	end

	PawnPromotionEffect( x, y )
end )

----------------------------------------------------
-- Hook:
--   Think
-- Purpose:
--   Checks for key input.
----------------------------------------------------
hook.Add( "Think", "KeyListener", function()
	if ( input.IsKeyDown( KEY_F1 ) ) then
		if ( not F1_DOWN ) then
			F1_DOWN = true

			RunConsoleCommand( "hl2chess_joingame" )
		end
	else
		F1_DOWN = false
	end
end )