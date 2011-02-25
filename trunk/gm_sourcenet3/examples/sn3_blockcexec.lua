include( "sn3_base_incoming.lua" )

FilterIncomingMessage( net_StringCmd, function( netchan, read, write )
	local cmd = read:ReadString()

	if ( SERVER ) then
		write:WriteString( cmd )
	else
		print( "Blocking command '" .. cmd .. "'" )
	end
end )