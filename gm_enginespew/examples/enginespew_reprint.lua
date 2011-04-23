require( "enginespew" )

IN_SPEW = false

local function _print( ... )
	IN_SPEW = true

	print( ... )
	
	IN_SPEW = false
end

hook.Add( "EngineSpew", "Test", function( spewType, msg, group, level )
	if IN_SPEW then return end -- prevent infinite loop
	
	_print( "EngineSpew", spewType, string.gsub( msg, "\n", "" ), group, level )
end )