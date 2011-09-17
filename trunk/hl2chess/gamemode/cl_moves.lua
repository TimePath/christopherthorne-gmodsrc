CL_CHESS_MOVE_MATERIAL = Material( "models/shiny" )
CL_CHESS_MOVE_COLOR = Color( 0, 230, 0, 255 )

----------------------------------------------------
-- Hook:
--   PreDrawOpaqueRenderables
-- Purpose:
--   Renders legal chess piece moves.
----------------------------------------------------
hook.Add( "PostDrawOpaqueRenderables", "RenderLegalMoves", function()
	if ( not CHESS_BOARD_MAP ) then return end

	for x, t in pairs( CHESS_BOARD_MAP )  do
		for y, v in pairs( t ) do
			local draw_vec = v.ent:GetPos()

			cam.Start3D2D( draw_vec, Angle( 0, 0, 0 ), 1 )
			
			if ( ( not CHESS_PIECE_SELECTED ) or ( v.ent ~= CHESS_BOARD_MAP[CHESS_PIECE_SELECTED.x][CHESS_PIECE_SELECTED.y].ent ) ) then
				surface.SetDrawColor( v.cteam_color )
				surface.DrawRect( -10, -10, 20, 20 )
			end
			
			cam.End3D2D()
		end
	end

	for x, t in pairs( CHESS_BOARD_MAP )  do
		for y, v in pairs( t ) do
			local draw_vec = v.ent:GetPos()
			draw_vec.z = 3

			cam.Start3D2D( draw_vec, Angle( 0, 0, 0 ), 1 )

			if ( ( CHESS_PIECE_SELECTED ) and ( v.ent == CHESS_BOARD_MAP[CHESS_PIECE_SELECTED.x][CHESS_PIECE_SELECTED.y].ent ) ) then
				for i = 1, 5 do
					surface.DrawCircle( 0, 0, 20 + i, v.cteam_color )
				end
	
				render.SetMaterial( CL_CHESS_MOVE_MATERIAL )

				-- TODO: cache selected piece's legal moves
				for k, move in pairs( GetLegalMoves( CHESS_PIECE_SELECTED.x, CHESS_PIECE_SELECTED.y ) ) do
					local padding

					if( CHESS_BOARD_MAP[move.x][move.y] ) then
						padding = math.sin( CurTime() * 5 ) * 15 + 25
					else
						padding = 20
					end

					draw.RoundedBox( 4, ( move.x - CHESS_PIECE_SELECTED.x ) * 128 - 64 + padding / 2, ( move.y - CHESS_PIECE_SELECTED.y ) * -128 - 64 + padding / 2, 128 - padding, 128 - padding, CL_CHESS_MOVE_COLOR )
				end
			end
		
			cam.End3D2D()
		end
	end
end )