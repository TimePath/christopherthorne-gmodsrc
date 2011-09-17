-- Chess board origin (0,0)
-- Relative to world origin
CHESS_BOARD_ORIGIN = Vector( -512, -512, 0 )

-- Team that goes first
CHESS_START_TEAM = CHESSTEAMTYPE_HEROES

-- Determines whether or not a chess game is currently in progress
CHESS_GAME_ACTIVE = false

----------------------------------------------------
-- Function:
--   WorldToBoardPosition
-- Purpose:
--   Converts a world vector to a board position.
----------------------------------------------------
function WorldToBoardPosition( vec )
	-- If z is above 0 then the position is outside of the chess board.
	if ( vec.z > 0 ) then return end

	-- Thanks, thomasfn
	return math.floor( ( ( vec - CHESS_BOARD_ORIGIN ).x / 128 ) ), math.floor( ( vec - CHESS_BOARD_ORIGIN ).y / 128 )
end

----------------------------------------------------
-- Function:
--   BoardToWorldPosition
-- Purpose:
--   Converts a board position to a world vector.
----------------------------------------------------
function BoardToWorldPosition( x, y )
	return CHESS_BOARD_ORIGIN + Vector( x * 128 + 64, y * 128 + 64, 0 )
end

----------------------------------------------------
-- Function:
--   IsValidBoardCoordinate
-- Purpose:
--   Checks if a board coordinate is valid.
----------------------------------------------------
function IsValidBoardCoordinate( v )
	if ( not v ) then return false end
	if ( v < 0 ) then return false end
	if ( v > 7 ) then return false end
	
	return true
end

----------------------------------------------------
-- Function:
--   IsLegalBoardMovement
-- Purpose:
--   Checks if a board movement is legal.
----------------------------------------------------
function IsLegalBoardMovement( pl_team, src_x, src_y, dst_x, dst_y )
	for k, v in pairs( GetLegalMoves( src_x, src_y ) ) do
		if ( ( v.x == dst_x ) and ( v.y == dst_y ) ) then
			return true
		end
	end

	return false
end

----------------------------------------------------
-- Function:
--   NewChessBoard
-- Purpose:
--   Creates an empty chess board.
----------------------------------------------------
function NewChessBoard()
	-- Map of chess board
	-- Contains chess pieces team, type, entity, position (and color on client)
	-- nil means there is nothing at the specified position
	CHESS_BOARD_MAP = {}
	
	-- Empty chess board map
	for x = 0, 7 do
		CHESS_BOARD_MAP[x] = {}
	end
end