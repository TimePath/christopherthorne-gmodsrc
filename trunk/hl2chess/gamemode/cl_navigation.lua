----------------------------------------------------
-- Function:
--   MoveChessPieceEntity
-- Purpose:
--   Moves a chess piece to the specified coordinate.
----------------------------------------------------
function MoveChessPieceEntity( src_x, src_y, dst_x, dst_y )
	local src_piece = CHESS_BOARD_MAP[src_x][src_y]
	
	if ( not src_piece ) then return end

	-- Source vector
	local source_vec = src_piece.ent:GetPos()

	-- Target vector
	local target_vec = BoardToWorldPosition( dst_x, dst_y )
	
	-- No Z movement
	target_vec.z = source_vec.z

	-- Displacement
	local dvec = target_vec - source_vec

	-- Already at destination
	if ( ( dvec.x == 0 ) and ( dvec.y == 0 ) ) then return end

	-- Store final destination
	src_piece.ent.chess_destination = target_vec

	local dst_piece = CHESS_BOARD_MAP[dst_x][dst_y]
	
	if ( dst_piece ) then
		-- Schedule target piece attack
		src_piece.ent.pending_attack = dst_piece.ent
	end

	CHESS_CURRENT_MOVE_ENTITY = src_piece.ent
	CHESS_CURRENT_MOVE_TARGET = target_vec
	CHESS_CURRENT_MOVE_LENGTH = dvec:Length()
	CHESS_CURRENT_MOVE_NORMAL = dvec:Normalize()
	
	-- Calculate angle to destination
	local deg_yaw = math.Rad2Deg( math.atan( dvec.y / dvec.x ) )

	if ( dvec.x < 0 ) then deg_yaw = deg_yaw + 180 end

	-- Face the destination
	src_piece.ent:SetAngles( Angle( 0, deg_yaw, 0 ) )

	-- Enable movement sequence
	RunChessWalkSequence( src_piece.ent )
end

----------------------------------------------------
-- Hook:
--   Think
-- Purpose:
--   Moves chess entities.
----------------------------------------------------
hook.Add( "Think", "MoveChessEntities", function()
	local ent = CHESS_CURRENT_MOVE_ENTITY

	if ( not ent ) then return end

	-- lower FrameTime() == lower delta
	local delta = CHESS_CURRENT_MOVE_NORMAL * ent.chess_move_speed * FrameTime()
	
	ent:SetPos( ent:GetPos() + delta )

	local distance = ( CHESS_CURRENT_MOVE_TARGET - ent:GetPos() ):Length2D()

	if ( distance <= delta:Length2D() ) then
		--print( "MoveChessEntities: finished" )

		-- Move to exact target position
		CHESS_CURRENT_MOVE_ENTITY:SetPos( CHESS_CURRENT_MOVE_TARGET )

		-- Tell server the animation has finished
		RunConsoleCommand( "hl2chess_finishedanimation" )

		-- Return to idle sequence
		RunChessIdleSequence( CHESS_CURRENT_MOVE_ENTITY )
			
		CHESS_CURRENT_MOVE_ENTITY = nil
	elseif ( ent.pending_attack ) then
		if ( distance < ent.attack_distance ) then	
			--print( "MoveChessEntities: attack" )

			RunChessAttackSequence( ent, function()	
				-- Resume movement sequence
				RunChessWalkSequence( ent )
				
				-- Resume movement
				CHESS_CURRENT_MOVE_ENTITY = ent
					
				-- Playing a sequence
				return true
			end )
			
			CHESS_CURRENT_MOVE_ENTITY = nil
			
			return
		end
	end
end )