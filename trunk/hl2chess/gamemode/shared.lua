local function sh_include( ... )
	if ( SERVER ) then
		AddCSLuaFile( ... )
	end
	
	include( ... )
end

sh_include( "sh_enum.lua" )
sh_include( "sh_pieces.lua" )
sh_include( "sh_moves.lua" )
sh_include( "sh_main.lua" )
sh_include( "sh_info.lua" )

------------------------------------------------------------

local function cl_include( ... )
	if ( SERVER ) then
		AddCSLuaFile( ... )
	else
		include( ... )
	end
end

cl_include( "cl_enum.lua" )
cl_include( "cl_anim.lua" )
cl_include( "cl_navigation.lua" )
cl_include( "cl_board.lua" )
cl_include( "cl_moves.lua" )
cl_include( "cl_hud.lua" )
cl_include( "cl_input.lua" )
cl_include( "cl_view.lua" )
cl_include( "cl_hover.lua" )
cl_include( "cl_promotion.lua" )
cl_include( "cl_main.lua" )