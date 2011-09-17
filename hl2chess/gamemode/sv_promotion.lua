----------------------------------------------------
-- Command:
--   hl2chess_promote_piece
-- Purpose:
--   Client wants to promote a pawn.
----------------------------------------------------
concommand.Add( "hl2chess_promote_piece", function( ply, cmd, args )
	if ( not ValidEntity( ply ) ) then return end
	if ( not CHESS_GAME_ACTIVE ) then return end
	if ( not CHESS_PENDING_PAWN_PROMOTION_PLY ) then return end
	if ( ply ~= CHESS_PENDING_PAWN_PROMOTION_PLY ) then return end
	if ( #args < 1 ) then return end
	
	local new_piece_type = tonumber( args[1] )

	--print( "hl2chess_promote_piece: ", PieceIndexToName( new_piece_type ) )

	-- Update board
	CHESS_BOARD_MAP[CHESS_PENDING_PAWN_PROMOTION_X][CHESS_PENDING_PAWN_PROMOTION_Y].ctype = new_piece_type

	-- Notify client
	umsg.Start( "hl2chess_promoted_piece", ply.Opponent )
	umsg.Char( CHESS_PENDING_PAWN_PROMOTION_X )
	umsg.Char( CHESS_PENDING_PAWN_PROMOTION_Y )
	umsg.Char( new_piece_type )
	umsg.End()
	
	-- Clear globals
	CHESS_PENDING_PAWN_PROMOTION_PLY = nil
	CHESS_PENDING_PAWN_PROMOTION_X = nil
	CHESS_PENDING_PAWN_PROMOTION_Y = nil
	
	-- Resume game
	SwitchTurns()
end )