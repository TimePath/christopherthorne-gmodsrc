-- Font to use for message
CHESS_MESSAGE_FONT = "Trebuchet22"

-- Maximum number ellipses to use for waiting text
CHESS_MESSAGE_MAX_ELLIPSES = 3

----------------------------------------------------
-- Function:
--   SetChessMessage
-- Purpose:
--   Sets the message at the top of the screen.
----------------------------------------------------
function SetChessMessage( msg, ellipses )
	CHESS_MESSAGE_TEXT = msg
	
	-- nil msg = don't show any notice
	if ( not CHESS_MESSAGE_TEXT ) then return end
	
	-- Set font for GetTextSize
	surface.SetFont( CHESS_MESSAGE_FONT )
	-- Returns width and height of text
	local w, h = surface.GetTextSize( CHESS_MESSAGE_TEXT )

	-- Store draw data for HUDPaint calls
	CHESS_MESSAGE_DATA = {
		box_x = ScrW() / 2 - w / 2 - 10,
		box_y = 20,
		box_w = w + ( ( ellipses ) and ( 40 ) or ( 20 ) ),
		box_h = h + 13,		
		text_x = ScrW() / 2 - w / 2,
		text_y = 26,
		ellipses = ( ellipses ) and ( { messages = {}, count = 0, update = 0 } ),
	}
	
	if ( ellipses ) then
		for i = 1, CHESS_MESSAGE_MAX_ELLIPSES do
			CHESS_MESSAGE_DATA.ellipses.messages[i] = msg .. string.rep( ".", i )
		end
	else
		CHESS_MESSAGE_DATA.message = msg
	end
end

----------------------------------------------------
-- Hook:
--   HUDPaint
-- Purpose:
--   Draws the current chess message.
----------------------------------------------------
hook.Add( "HUDPaint", "DrawMessage", function()
	if ( CHESS_MESSAGE_TEXT ) then
		if ( CHESS_MESSAGE_DATA.ellipses ) then
			if ( CurTime() >= CHESS_MESSAGE_DATA.ellipses.update ) then
				if ( CHESS_MESSAGE_DATA.ellipses.count == CHESS_MESSAGE_MAX_ELLIPSES ) then
					CHESS_MESSAGE_DATA.ellipses.count = 1
				else
					CHESS_MESSAGE_DATA.ellipses.count = CHESS_MESSAGE_DATA.ellipses.count + 1
				end

				CHESS_MESSAGE_DATA.message = CHESS_MESSAGE_DATA.ellipses.messages[CHESS_MESSAGE_DATA.ellipses.count]
				CHESS_MESSAGE_DATA.ellipses.update = CurTime() + 1
			end
		end

		draw.RoundedBox( 8, CHESS_MESSAGE_DATA.box_x, CHESS_MESSAGE_DATA.box_y, CHESS_MESSAGE_DATA.box_w, CHESS_MESSAGE_DATA.box_h, COLOR_WHITE )
		draw.SimpleText( CHESS_MESSAGE_DATA.message, CHESS_MESSAGE_FONT, CHESS_MESSAGE_DATA.text_x, CHESS_MESSAGE_DATA.text_y, COLOR_BLACK )
	end
end )

----------------------------------------------------
-- Hook:
--   HUDShouldDraw
-- Purpose:
--   Determines what HUD elements to draw.
----------------------------------------------------
hook.Add( "HUDShouldDraw", "HideElements", function( name )
	if ( name == "CHudHealth" ) then return false end
	if ( name == "CHudCrosshair" ) then return false end

	return true
end )