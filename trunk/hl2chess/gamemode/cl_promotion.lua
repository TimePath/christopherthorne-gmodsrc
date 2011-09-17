-- A very rushed promotion interface!

local main = vgui.Create( "DFrame" )
main:SetTitle( "Promotion" )
main:SetSize( 160, 142 )
main:SetDraggable( false )
main:ShowCloseButton( false )
main:SetBackgroundBlur( true )
main:Center()
main:SetVisible( false )

local list = vgui.Create( "DListView", main )
list:SetPos( 10, 30 )
list:SetSize( 50, 101 )
list:SetMultiSelect( false )
list:AddColumn( "Upgrade" )

for k, v in pairs( CHESS_PIECE_INFO ) do
	if ( k == CHESSPIECETYPE_PAWN ) then continue end

	local line = list:AddLine( PieceIndexToName( k ) )
	line.ctype = k
end

local promote = vgui.Create( "DButton", main )
promote:SetText( "Promote" )
promote:SetPos( 70, 30 )
promote:SetSize( 80, 101 )

function promote:DoClick()
	local line = list:GetSelected()[1]
	
	if ( not line ) then return end

	RunConsoleCommand( "hl2chess_promote_piece", line.ctype )

	PromoteChessPiece( main.piece_x, main.piece_y, line.ctype )

	main:SetVisible( false )
	main.piece_x = nil
	main.piece_y = nil
end

----------------------------------------------------
-- Function:
--   ShowPromotionMenu
-- Purpose:
--   Displays the promotion menu.
----------------------------------------------------
function ShowPromotionMenu( x, y )
	main:SetVisible( true )
	main.piece_x = x
	main.piece_y = y
end

----------------------------------------------------
-- Function:
--   PromoteChessPiece
-- Purpose:
--   Promotes a chess piece at the specified
--   coordinate.
----------------------------------------------------
function PromoteChessPiece( x, y, ctype )
	--print( "PromoteChessPiece: promoting (" .. x .. "," .. y .. ") to " .. PieceIndexToName( ctype ) )

	local cteam = CHESS_BOARD_MAP[x][y].cteam

	CHESS_BOARD_MAP[x][y].ctype = ctype
	CHESS_BOARD_MAP[x][y].ent = CreateChessEntity( table.Random( CHESS_PIECE_INFO[ctype][cteam] ), x, y, ( cteam == CHESSTEAMTYPE_VILLAINS ) and 90 or 270 )
end

----------------------------------------------------
-- User Message:
--   hl2chess_promoted_piece
-- Purpose:
--   Handles opponent's promotions.
----------------------------------------------------
usermessage.Hook( "hl2chess_promoted_piece", function( um )
	local x = um:ReadChar()
	local y = um:ReadChar()
	local ctype = um:ReadChar()

	PromoteChessPiece( x, y, ctype )
end )