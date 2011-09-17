----------------------------------------------------
-- Command:
--   hl2chess_finishedanimation
-- Purpose:
--   Client is ready for the next turn.
----------------------------------------------------
concommand.Add( "hl2chess_finishedanimation", function( ply, cmd, args )
	-- Check player is valid
	if ( !ValidEntity( ply ) ) then return end	
	
	if ( SV_PENDING_ANIM_ACK ) then
		if ( SV_PENDING_FINAL_ANIM_ACK ) then
			-- Both players have sent the ack
			SV_PENDING_ANIM_ACK = nil
			SV_PENDING_FINAL_ANIM_ACK = nil

			if ( CHESS_GAME_WINNING_TEAM ) then
				EndChessGame( CHESS_GAME_WINNING_TEAM )
				
				CHESS_GAME_WINNING_TEAM = nil
			else
				if ( CHESS_PENDING_PAWN_PROMOTION_PLY ) then
					for k, v in pairs( CHESS_PLAYERS ) do
						umsg.Start( "hl2chess_promote_piece", v )
						umsg.Char( CHESS_PENDING_PAWN_PROMOTION_X )
						umsg.Char( CHESS_PENDING_PAWN_PROMOTION_Y )

						if ( CHESS_PENDING_PAWN_PROMOTION_PLY == v ) then
							umsg.Bool( true )
						else
							umsg.Bool( false )
						end
						
						umsg.End()
					end
				else
					-- Switch to next player's turn
					SwitchTurns()
				end
			end
		else
			-- Received one ack, waiting for the other
			SV_PENDING_FINAL_ANIM_ACK = true
		end
	end
end )