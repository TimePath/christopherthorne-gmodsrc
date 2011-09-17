CL_CHESS_CAM_PITCH = 60
CL_CHESS_CAM_Y = 520
CL_CHESS_CAM_Z = 790

----------------------------------------------------
-- Function:
--   SetupClientView
-- Purpose:
--   Calculates the player's view origin and angles.
----------------------------------------------------
function SetupClientView()
	local cam_y
	local cam_yaw

	if ( CL_CHESS_TEAM == CHESSTEAMTYPE_VILLAINS ) then
		cam_y = CL_CHESS_CAM_Y
		cam_yaw = 270
	else
		cam_y = CL_CHESS_CAM_Y * -1
		cam_yaw = 90
	end

	CL_CHESS_VIEW = { origin = Vector( 0, cam_y, CL_CHESS_CAM_Z ), angles = Angle( CL_CHESS_CAM_PITCH, cam_yaw, 0 ) }
end

----------------------------------------------------
-- Hook:
--   CalcView
-- Purpose:
--   Sets the player's view.
----------------------------------------------------
hook.Add( "CalcView", "SetClientView", function( ply, pos, angles, fov )
	return CL_CHESS_VIEW
end )