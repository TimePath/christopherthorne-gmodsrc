function GetCursorBoardPosition( vec )
	if ( not CL_CHESS_VIEW ) then return end

	-- Setup trace
	local trace = {}
	trace.start = CL_CHESS_VIEW.origin
	trace.endpos = trace.start + vec * 2048
	trace.mask = MASK_SOLID_BRUSHONLY

	-- Return board coordinates of trace result
	return WorldToBoardPosition( util.TraceLine( trace ).HitPos )
end

----------------------------------------------------
-- Hook:
--   GUIMousePressed
-- Purpose:
--   Handles mouse input.
----------------------------------------------------
hook.Add( "GUIMousePressed", "ChessBoardInput", function( code, vec )
	-- Only listen for input during the client's turn
	if ( CL_CHESS_TURN ~= CHESSTURNTYPE_YOU ) then return end

	-- Get board coordinates
	local x, y = GetCursorBoardPosition( vec )

	-- Validate board coordinates
	if ( not IsValidBoardCoordinate( x ) ) then return end
	if ( not IsValidBoardCoordinate( y ) ) then return end

	-- Check if client selected a chess piece
	if ( ( CHESS_BOARD_MAP[x][y] ) and ( CHESS_BOARD_MAP[x][y].cteam == CL_CHESS_TEAM ) ) then
		--print( "ChessBoardInput: select" )

		CHESS_PIECE_SELECTED = { x = x, y = y }
				
		return
	end
	
	-- If the client didn't click a chess piece, move selected piece to the destination
	if ( CHESS_PIECE_SELECTED ) then
		if ( code == MOUSE_LEFT ) then		
			if ( IsLegalBoardMovement( CL_CHESS_TEAM, CHESS_PIECE_SELECTED.x, CHESS_PIECE_SELECTED.y, x, y ) ) then
				--print( "ChessBoardInput: valid move" )

				-- Notify server of movement
				RunConsoleCommand( "hl2chess_movepiece", CHESS_PIECE_SELECTED.x, CHESS_PIECE_SELECTED.y, x, y )

				-- Tell entity to move to new position
				MoveChessPieceEntity( CHESS_PIECE_SELECTED.x, CHESS_PIECE_SELECTED.y, x, y )

				-- Save movement for calculating legal pawn moves
				if ( CHESS_BOARD_MAP[CHESS_PIECE_SELECTED.x][CHESS_PIECE_SELECTED.y].ctype == CHESSPIECETYPE_PAWN ) then
					CHESS_BOARD_MAP[CHESS_PIECE_SELECTED.x][CHESS_PIECE_SELECTED.y].has_moved = true
				end

				-- Update board
				CHESS_BOARD_MAP[x][y] = CHESS_BOARD_MAP[CHESS_PIECE_SELECTED.x][CHESS_PIECE_SELECTED.y]
				CHESS_BOARD_MAP[CHESS_PIECE_SELECTED.x][CHESS_PIECE_SELECTED.y] = nil

				-- Clear selection
				CHESS_PIECE_SELECTED = nil

				-- Pending turn
				SetChessTurnType( CHESSTURNTYPE_PENDING )
			else
				--print( "ChessBoardInput: illegal move" )
			end
		else
			-- Deselect
			CHESS_PIECE_SELECTED = nil
		end
	end
end )