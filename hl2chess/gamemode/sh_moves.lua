----------------------------------------------------
-- Function:
--   InsertMove
-- Purpose:
--   Helper function to insert a legal move.
----------------------------------------------------
function InsertMove( t, x, y, src, mtype )
	if ( not IsValidBoardCoordinate( x ) ) then return false end
	if ( not IsValidBoardCoordinate( y ) ) then return false end

	local piece = CHESS_BOARD_MAP[x][y]

	if ( piece ) then
		-- Only legal if the space is free
		if ( mtype == CHESSMOVETYPE_FREEONLY ) then return false end
		
		-- Never move to a position occupied by a team piece
		if ( piece.cteam == src.cteam ) then return false end
	else
		-- Only legal if the space is not free
		if ( mtype == CHESSMOVETYPE_ENEMYONLY ) then return false end
	end

	-- Add to legal moves table
	table.insert( t, { x = x, y = y } )
	
	-- Move was inserted into the table
	return true
end

----------------------------------------------------
-- Function:
--   InsertLegalKingMoves
-- Purpose:
--   Inserts king's moves (1 unit up/down, 1 unit
--   left/right and 1 unit diagonal)
----------------------------------------------------
function InsertLegalKingMoves( t, x, y, src )
	for dx = -1, 1 do
		for dy = -1, 1 do
			-- Skip current position
			if ( ( dx == 0 ) and ( dy == 0 ) ) then continue end
				
			InsertMove( t, x + dx, y + dy, src )
		end
	end
end

----------------------------------------------------
-- Function:
--   InsertLegalQueenMoves
-- Purpose:
--   Inserts queen's moves (up/down, diagonal)
----------------------------------------------------
function InsertLegalQueenMoves( t, x, y, src )
	InsertLegalRookMoves( t, x, y, src )
	InsertLegalBishopMoves( t, x, y, src )
end

----------------------------------------------------
-- Function:
--   InsertLegalRookMoves
-- Purpose:
--   Inserts a rook's moves (up/down, left/right)
----------------------------------------------------
function InsertLegalRookMoves( t, x, y, src )
	for i = -1, 1, 2 do
		for dx = 1, 7 do
			if ( not InsertMove( t, x + dx * i, y, src ) ) then break end		
			if ( CHESS_BOARD_MAP[x + dx * i][y] ) then break end
		end

		for dy = 1, 7 do
			if ( not InsertMove( t, x, y + dy * i, src ) ) then break end	
			if ( CHESS_BOARD_MAP[x][y + dy * i] ) then break end
		end
	end
end

----------------------------------------------------
-- Function:
--   InsertLegalBishopMoves
-- Purpose:
--   Inserts a bishop's moves (diagonal)
----------------------------------------------------
function InsertLegalBishopMoves( t, x, y, src )
	for i = -1, 1, 2 do
		for j = -1, 1, 2 do		
			for dd = 1, 7 do
				if ( not InsertMove( t, x + dd * i * j, y + dd * i, src ) ) then break end		
				if ( CHESS_BOARD_MAP[x + dd * i * j][y + dd * i] ) then break end				
			end
		end
	end
end

----------------------------------------------------
-- Function:
--   InsertLegalKnightMoves
-- Purpose:
--   Inserts a knight's moves (L positions)
----------------------------------------------------
function InsertLegalKnightMoves( t, x, y, src )
	for i = -1, 1, 2 do
		for j = -1, 1, 2 do
			InsertMove( t, x + 2 * i, y + 1 * j, src )
			InsertMove( t, x + 1 * i, y + 2 * j, src )
		end
	end
end

----------------------------------------------------
-- Function:
--   InsertLegalPawnMoves
-- Purpose:
--   Inserts a pawn's moves (conditional up,
--   conditional diagonal)
----------------------------------------------------
function InsertLegalPawnMoves( t, x, y, src )
	local dy = ( src.cteam == CHESSTEAMTYPE_VILLAINS ) and ( -1 ) or ( 1 )

	if ( InsertMove( t, x, y + dy, src, CHESSMOVETYPE_FREEONLY ) ) then
		if ( not src.has_moved ) then
			InsertMove( t, x, y + dy * 2, src, CHESSMOVETYPE_FREEONLY )
		end
	end

	InsertMove( t, x + 1, y + dy, src, CHESSMOVETYPE_ENEMYONLY )
	InsertMove( t, x - 1, y + dy, src, CHESSMOVETYPE_ENEMYONLY )
end

----------------------------------------------------
-- Function:
--   GetLegalMoves
-- Purpose:
--   Returns all legal moves a chess piece can make.
----------------------------------------------------
function GetLegalMoves( x, y )
	local piece = CHESS_BOARD_MAP[x][y]
	local piece_type = piece.ctype
	local t = {}

	if ( piece_type == CHESSPIECETYPE_PAWN ) then
		InsertLegalPawnMoves( t, x, y, piece )
	elseif ( piece_type == CHESSPIECETYPE_KNIGHT ) then
		InsertLegalKnightMoves( t, x, y, piece )
	elseif ( piece_type == CHESSPIECETYPE_BISHOP ) then
		InsertLegalBishopMoves( t, x, y, piece )
	elseif ( piece_type == CHESSPIECETYPE_ROOK ) then
		InsertLegalRookMoves( t, x, y, piece )
	elseif ( piece_type == CHESSPIECETYPE_QUEEN ) then
		InsertLegalQueenMoves( t, x, y, piece )
	elseif ( piece_type == CHESSPIECETYPE_KING ) then
		InsertLegalKingMoves( t, x, y, piece )
	end

	return t
end