include( "sn3_base_incoming.lua" )

FilterIncomingMessage( net_StringCmd, function( netchan, read, write )
	local cmd = read:ReadString()
	
	print( string.format( "Client %s ran command \"%s\"", netchan:GetAddress():ToString(), cmd ) )

	if ( string.Left( cmd, 6 ) == "status" ) then
		print( "Blocked status command" )

		return
	end

	write:WriteUBitLong( net_StringCmd, 6 )
	write:WriteString( cmd )
end )