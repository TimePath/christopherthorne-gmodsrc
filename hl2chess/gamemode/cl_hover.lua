CL_CHESS_HOVER_OFFSET = Vector( 0, 0, 100 )
CL_CHESS_HOVER_FONT = "DefaultFixed"
CL_CHESS_LAST_CURSOR_POSITION = { x = 0, y = 0 }

----------------------------------------------------
-- Hook:
--   HUDPaint
-- Purpose:
--   Draws the name of the chess piece at the
--   cursor position.
----------------------------------------------------
hook.Add( "HUDPaint", "DrawPieceName", function()
	if ( not StoredLocalPlayer ) then return end

	-- Get cursor position
	local x, y = gui.MousePos()
	
	-- Update if the cursor moved
	if ( ( CL_CHESS_LAST_CURSOR_POSITION.x ~= x ) or ( CL_CHESS_LAST_CURSOR_POSITION.y ~= y ) ) then
		-- Store current cursor position
		CL_CHESS_LAST_CURSOR_POSITION.x = x
		CL_CHESS_LAST_CURSOR_POSITION.y = y
		
		-- Get board coordinates
		-- Calling this every frame seems like a bad idea, although there is no noticeable FPS drop
		local board_x, board_y = GetCursorBoardPosition( StoredLocalPlayer:GetCursorAimVector() )
		
		-- Validate board coordinates
		if ( ( IsValidBoardCoordinate( board_x ) ) and ( IsValidBoardCoordinate( board_y ) ) ) then
			CL_CHESS_HOVER_X = board_x
			CL_CHESS_HOVER_Y = board_y
		else
			CL_CHESS_HOVER_X = nil
			CL_CHESS_HOVER_Y = nil
		end
	end

	-- Don't continue without hover coordinates
	if ( ( not CL_CHESS_HOVER_X ) or ( not CL_CHESS_HOVER_Y ) ) then return end

	-- Chess piece at coordinates
	local piece = CHESS_BOARD_MAP[CL_CHESS_HOVER_X][CL_CHESS_HOVER_Y]

	-- Ensure there is a piece at hover coordinates
	if ( not piece ) then return end

	-- Set drawing font		
	surface.SetFont( CL_CHESS_HOVER_FONT )

	-- Drawing position
	local draw_vec = ( piece.ent:GetPos() + CL_CHESS_HOVER_OFFSET ):ToScreen()
	-- Piece name
	local name = PieceIndexToName( piece.ctype )
	-- Retrieve text size
	local w, h = surface.GetTextSize( name )

	-- Draw container box
	draw.RoundedBox( 4, draw_vec.x - w / 2 - 4.5, draw_vec.y - h / 2 + 2, w + 9, 16, COLOR_BLACK_A150 )
	-- Draw piece name
	draw.SimpleText( name, CL_CHESS_HOVER_FONT, draw_vec.x, draw_vec.y, COLOR_WHITE, TEXT_ALIGN_CENTER )
end )