include( "sn3_base_incoming.lua" )
include( "sn3_base_outgoing.lua" )

local function FilterGameEvent( netchan, read, write, hookname )
	local bits = read:ReadUBitLong( 11 )
	local data = read:ReadBits( bits )

	if ( !read:IsOverflowed() ) then
		local buffer = sn3_bf_read( data, bits )
		local event = IGameEventManager2():UnserializeEvent( buffer )
		
		buffer:FinishReading()

		local result = hook.Call( hookname, nil, netchan, event )

		if ( result != false ) then
			write:WriteUBitLong( svc_GameEvent, 6 )

			if ( type( result ) == "IGameEvent" ) then
				local serialized_data = UCHARPTR( 2048 )
				local serialized_buffer = sn3_bf_write( serialized_data, 2048 )

				IGameEventManager2():SerializeEvent( event, serialized_buffer )
				
				write:WriteUBitLong( serialized_buffer:GetNumBitsWritten(), 11 )
				write:WriteBits( serialized_buffer:GetBasePointer(), serialized_buffer:GetNumBitsWritten() )
				
				serialized_buffer:FinishWriting()
				serialized_data:Delete()
			else
				write:WriteUBitLong( bits, 11 )
				write:WriteBits( data, bits )
			end
		end
		
		event:Delete()
	end

	data:Delete()
end

if ( CLIENT ) then
	FilterIncomingMessage( svc_GameEvent, function( netchan, read, write )
		FilterGameEvent( netchan, read, write, "ProcessGameEvent" )
	end )
else
	FilterOutgoingMessage( svc_GameEvent, function( netchan, read, write )
		FilterGameEvent( netchan, read, write, "SendGameEvent" )
	end )
end