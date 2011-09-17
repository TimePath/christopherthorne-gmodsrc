-- Current players
CHESS_PLAYERS = {}

----------------------------------------------------
-- Function:
--   SetTurnTeam
-- Purpose:
--   Sets the team who's turn it is.
----------------------------------------------------
function SetTurnTeam( team_index )
	-- Update global
	CHESS_CURRENT_TURN_TEAM = team_index

	-- Team players
	local ply_cur = CHESS_PLAYERS[team_index]
	local ply_next = ply_cur.Opponent

	-- Begin message
	umsg.Start( "hl2chess_turn", ply_cur )
	-- Client's turn
	umsg.Char( CHESSTURNTYPE_YOU )
	-- End message
	umsg.End()
	
	-- Begin message
	umsg.Start( "hl2chess_turn", ply_next )
	-- Opponent's turn
	umsg.Char( CHESSTURNTYPE_OPPONENT )
	-- End message
	umsg.End()
end

----------------------------------------------------
-- Function:
--   SwitchTurns
-- Purpose:
--   Switches turns.
----------------------------------------------------
function SwitchTurns()
	if ( CHESS_CURRENT_TURN_TEAM == CHESSTEAMTYPE_HEROES ) then
		SetTurnTeam( CHESSTEAMTYPE_VILLAINS )
	else
		SetTurnTeam( CHESSTEAMTYPE_HEROES )
	end
end

----------------------------------------------------
-- Function:
--   SetupChessClient
-- Purpose:
--   Sets up the game for a client.
----------------------------------------------------
function SetupChessClient( ply, team_index )	
	-- Make player a spectator
	ply:Spectate( OBS_MODE_FIXED )
		
	-- Store team index
	ply.ChessTeam = team_index
		
	-- Store player
	CHESS_PLAYERS[team_index] = ply

	-- Send client their team
	umsg.Start( "hl2chess_initgame", ply )
	umsg.Char( team_index )
	umsg.End()
end

----------------------------------------------------
-- Function:
--   BeginChessGame
-- Purpose:
--   Starts a chess game.
----------------------------------------------------
function BeginChessGame( ply1, ply2 )
	if ( CHESS_GAME_ACTIVE ) then return end

	-- Create chess board
	NewChessBoard()

	-- Create chess pieces
	for k, v in pairs( CHESS_PIECE_INFO ) do
		for k2, v2 in pairs( v.spawns ) do
			local x = v2.x	
			local y = { [CHESSTEAMTYPE_VILLAINS] = 7 - v2.y, [CHESSTEAMTYPE_HEROES] = v2.y }

			for i = CHESSTEAMTYPE_MIN, CHESSTEAMTYPE_MAX do
				-- Update board
				CHESS_BOARD_MAP[x][y[i]] = { cteam = i, ctype = k }
			end
		end
	end

	-- Random team selection
	local random_team = math.random( 0, 1 )	
	local team1, team2

	if ( random_team == 0 ) then
		team1 = CHESSTEAMTYPE_VILLAINS
		team2 = CHESSTEAMTYPE_HEROES
	else
		team1 = CHESSTEAMTYPE_HEROES
		team2 = CHESSTEAMTYPE_VILLAINS
	end

	-- Setup clients
	SetupChessClient( ply1, team1 )
	SetupChessClient( ply2, team2 )
	
	-- Store opponent for ease of access
	ply1.Opponent = ply2
	ply2.Opponent = ply1
	
	-- Start the first turn
	SetTurnTeam( CHESS_START_TEAM )

	-- Game is now active
	CHESS_GAME_ACTIVE = true
end

----------------------------------------------------
-- Function:
--   EndChessGame
-- Purpose:
--   Ends the current chess game.
----------------------------------------------------
function EndChessGame( winner )
	-- Notify players
	umsg.Start( "hl2chess_endgame" )
	
	if ( winner ) then
		umsg.Char( winner )
	else
		umsg.Char( -1 )
	end

	umsg.End()
	
	-- Unspectate players
	for k, v in pairs( CHESS_PLAYERS ) do
		v:UnSpectate()
	end

	-- Reset variables
	CHESS_GAME_ACTIVE = false
	CHESS_BOARD_MAP = nil
	CHESS_PLAYERS = {}
end

----------------------------------------------------
-- Command:
--   hl2chess_cl_initpostentity
-- Purpose:
--   Sent by client after entities have been
--   initialized.
----------------------------------------------------
concommand.Add( "hl2chess_cl_initpostentity", function( ply, cmd, args )
	-- Check player is valid
	if ( !ValidEntity( ply ) ) then return end
	
	-- Only allow the command to be executed once
	if ( ply.InitPostEntity ) then return end

	if ( not CHESS_GAME_ACTIVE ) then
		-- Client can join a game
		umsg.Start( "hl2chess_joingame", ply )
		umsg.End()
	end

	ply.InitPostEntity = true
end )

----------------------------------------------------
-- Command:
--   hl2chess_joingame
-- Purpose:
--   Client wants to join a game.
----------------------------------------------------
concommand.Add( "hl2chess_joingame", function( ply, cmd, args )
	if ( not ValidEntity( ply ) ) then return end
	if ( CHESS_GAME_ACTIVE ) then return end
	
	if ( CHESS_PLAYER_WAITING ) then
		-- Start a game
		BeginChessGame( ply, CHESS_PLAYER_WAITING )
		
		-- Player is no longer waiting
		CHESS_PLAYER_WAITING = nil
	else
		-- Let client know there is no opponent
		umsg.Start( "hl2chess_pendingopponent", ply )
		umsg.End()
			
		-- Store player as waiting
		CHESS_PLAYER_WAITING = ply
	end
end )