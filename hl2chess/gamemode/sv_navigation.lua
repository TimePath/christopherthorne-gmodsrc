----------------------------------------------------
-- Command:
--   hl2chess_movepiece
-- Purpose:
--   Client requested to move a chess piece.
----------------------------------------------------
concommand.Add( "hl2chess_movepiece", function( ply, cmd, args )
	-- Check player is valid
	if ( !ValidEntity( ply ) ) then return end	

	-- Check that a game is in progress
	if ( not CHESS_GAME_ACTIVE ) then return end

	-- Check it's the player's turn
	if ( not ply.ChessTeam == CHESS_CURRENT_TURN_TEAM ) then return end	

	-- Check syntax: <source x> <source y> <destination x> <destination y>
	if ( #args < 4 ) then return end

	-- x-coordinate of source
	local src_x = tonumber( args[1] )
	if ( not IsValidBoardCoordinate( src_x ) ) then return end
	
	-- y-coordinate of source
	local src_y = tonumber( args[2] )
	if ( not IsValidBoardCoordinate( src_y ) ) then return end
	
	-- x-coordinate of destination
	local dst_x = tonumber( args[3] )	
	if ( not IsValidBoardCoordinate( dst_x ) ) then return end
	
	-- y-coordinate of destination
	local dst_y = tonumber( args[4] )
	if ( not IsValidBoardCoordinate( dst_y ) ) then return end

	-- Chess piece at source point
	local src_piece = CHESS_BOARD_MAP[src_x][src_y]	
	if ( not src_piece ) then return end
	
	-- Check if move is legal
	if ( not IsLegalBoardMovement( ply.ChessTeam, src_x, src_y, dst_x, dst_y ) ) then print( "hl2chess_movepiece: illegal move" ) return end
	
	-- Check if a king is being captured
	local xelements = CHESS_BOARD_MAP[dst_x]
	
	if ( xelements ) then
		local piece = xelements[dst_y]
		
		if ( ( piece ) and ( piece.ctype == CHESSPIECETYPE_KING ) ) then
			CHESS_GAME_WINNING_TEAM = ply.ChessTeam
		end
	end
	
	if ( not CHESS_GAME_WINNING_TEAM ) then
		-- Pawn promotion
		local promote = false
		
		if ( src_piece.ctype == CHESSPIECETYPE_PAWN ) then
			if ( src_piece.cteam == CHESSTEAMTYPE_VILLAINS ) then
				if ( dst_y == 0 ) then
					promote = true
				end
			else
				if ( dst_y == 7 ) then
					promote = true
				end
			end
		end
		
		if ( promote ) then
			CHESS_PENDING_PAWN_PROMOTION_PLY = ply
			CHESS_PENDING_PAWN_PROMOTION_X = dst_x
			CHESS_PENDING_PAWN_PROMOTION_Y = dst_y
		end
	end
	
	-- Update board
	CHESS_BOARD_MAP[dst_x][dst_y] = CHESS_BOARD_MAP[src_x][src_y]
	CHESS_BOARD_MAP[src_x][src_y] = nil

	-- Tell opponent about the move
	umsg.Start( "hl2chess_board_update", ply.Opponent )
	umsg.Char( src_x )
	umsg.Char( src_y )
	umsg.Char( dst_x )
	umsg.Char( dst_y )
	umsg.End()
	
	-- Set move type
	SV_PENDING_ANIM_ACK = true
end )