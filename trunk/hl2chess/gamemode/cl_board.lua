CL_CHESS_BOARD_RECTS = {}
CL_CHESS_COLOR_SWITCH = false

----------------------------------------------------
-- Function:
--   SwitchChessColor
-- Purpose:
--   Switches the alternating board colour.
----------------------------------------------------
function SwitchChessColor()
	CL_CHESS_COLOR_SWITCH = not CL_CHESS_COLOR_SWITCH
end

----------------------------------------------------
-- Function:
--   GetChessColor
-- Purpose:
--   Retrieves an alternating board colour.
----------------------------------------------------
function GetChessColor()
	if ( CL_CHESS_COLOR_SWITCH ) then
		CL_CHESS_COLOR_SWITCH = false
		
		return COLOR_WHITE
	else
		CL_CHESS_COLOR_SWITCH = true
		
		return COLOR_BLACK
	end
end

----------------------------------------------------
-- Hook:
--   PreDrawOpaqueRenderables
-- Purpose:
--   Draws the chess board.
----------------------------------------------------
hook.Add( "PreDrawOpaqueRenderables", "DrawBoard", function()
	cam.Start3D2D( Vector( 0, 0, 0 ), Angle( 0, 0, 0 ), 1 )
	
	for x, t in pairs( CL_CHESS_BOARD_RECTS ) do
		for y, v in pairs( t ) do
			surface.SetDrawColor( v.color )
			surface.DrawRect( v.x, v.y, 128, 128 )
		end
	end

	cam.End3D2D()
end )

for x = 0, 7 do
	SwitchChessColor()

	CL_CHESS_BOARD_RECTS[x] = {}

	for y = 0, 7 do
		local vec = BoardToWorldPosition( x, y )
		
		CL_CHESS_BOARD_RECTS[x][y] = {}
		CL_CHESS_BOARD_RECTS[x][y].x = vec.x - 64
		CL_CHESS_BOARD_RECTS[x][y].y = vec.y - 64
		CL_CHESS_BOARD_RECTS[x][y].color = GetChessColor()
	end
end