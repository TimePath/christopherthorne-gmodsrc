-- TEMPORARY MODULE DEPENDENCY
--require( "ragdoll" )

----------------------------------------------------
-- Function:
--   BeginSequenceByID
-- Purpose:
--   Starts a sequence by its ID.
----------------------------------------------------
function BeginSequenceByID( ent, sequence_id )
	if ( sequence_id == -1 ) then return false end

	ent:SetCycle( 0 )
	ent:ResetSequence( sequence_id )
end

----------------------------------------------------
-- Function:
--   BeginSequenceByName
-- Purpose:
--   Starts a sequence by its name.
----------------------------------------------------
function BeginSequenceByName( ent, sequence_name )
	ent.cur_seq_name = sequence_name

	BeginSequenceByID( ent, ent:LookupSequence( sequence_name ) )
end

----------------------------------------------------
-- Function:
--   RunNamedChessSequences
-- Purpose:
--   Runs an ordered list of sequences.
----------------------------------------------------
function RunNamedChessSequences( ent, sequences, finished_callback )
	ent.sequence_list = { sequences = sequences, count = #sequences, finished_callback = finished_callback }
end

----------------------------------------------------
-- Function:
--   RunChessIdleSequence
-- Purpose:
--   Runs an entity's idle sequence.
----------------------------------------------------
function RunChessIdleSequence( ent )
	BeginSequenceByID( ent, ent.idle_sequence )
end

----------------------------------------------------
-- Function:
--   RunChessWalkSequence
-- Purpose:
--   Runs an entity's walk sequence.
----------------------------------------------------
function RunChessWalkSequence( ent )
	BeginSequenceByID( ent, ent.walk_sequence )
end

----------------------------------------------------
-- Function:
--   RunChessAttackSequence
-- Purpose:
--   Runs an entity's attack sequence.
----------------------------------------------------
function RunChessAttackSequence( ent, finished_callback )
	RunNamedChessSequences( ent, ent.attack_sequences, finished_callback )
end

----------------------------------------------------
-- Hook:
--   Think
-- Purpose:
--   Animates chess entities.
----------------------------------------------------
hook.Add( "Think", "AnimateChessEntities", function()
	if ( not CHESS_BOARD_MAP ) then return end

	for x, t in pairs( CHESS_BOARD_MAP ) do
		for y, v in pairs( t ) do
			-- Advance to the next animation frame
			v.ent:FrameAdvance()
		
			-- Use custom list if present
			local clist = v.ent.sequence_list

			if ( clist ) then
				if ( clist.running ) then
					local progress = v.ent:GetCycle()

					if ( progress >= clist.cur_seq.ragdoll_progress ) then
						if ( progress < clist.cur_seq.finish ) then
							local enemy = v.ent.pending_attack
	
							if ( enemy ) then
								-- Play attack sound
								v.ent:EmitSound( "physics/body/body_medium_impact_hard" .. math.random( 1, 6 ) .. ".wav" )
								-- Play death sound
								enemy:EmitSound( enemy.death_sound )

								-- Set network position for BecomeRagdollOnClient
								enemy:SetNetworkOrigin( enemy:GetPos() )
								enemy:SetNetworkAngles( enemy:GetAngles() )
								
								-- Create clientside ragdoll
								local ragdoll = enemy:BecomeRagdollOnClient()

								-- Remove the owner entity
								enemy:Remove()

								-- Add some velocity
								if ( ValidEntity( ragdoll ) ) then
									local phys = ragdoll:GetPhysicsObject()
								
									if ( ( phys ) and ( phys:IsValid() ) ) then
										phys:ApplyForceCenter( ( enemy:GetPos() - v.ent:GetPos() ) * 100000 )
									end
								end
								
								-- Remove after 3 seconds
								SafeRemoveEntityDelayed( ragdoll, 3 )
								
								-- No longer pending
								v.ent.pending_attack = nil
							end
						else
							-- Finished
							if ( clist.position < clist.count ) then	
								-- Move to next sequence
								clist.position = clist.position + 1
							else
								local finished_callback = clist.finished_callback

								-- Clear list
								v.ent.sequence_list = nil

								-- All custom sequences finished
								if ( not finished_callback() ) then
									RunChessIdleSequence( v.ent )
								end

								-- Move to next element
								continue
							end
				
							-- Next sequence needs to be initiated
							clist.running = false
						end
					end
					
					if ( clist.cur_seq.sounds ) then
						for index, sound in pairs( clist.cur_seq.sounds ) do
							if ( progress >= sound.progress ) then
								if ( not table.HasValue( v.ent.played_sounds, index ) ) then						
									v.ent:EmitSound( sound.name )
									
									--print( v.ent, "emitting", sound.name )

									table.insert( v.ent.played_sounds, index )
								end
							end
						end
					end
				else
					-- First sequence
					if ( not clist.position ) then clist.position = 1 end

					-- Sequence at current position
					clist.cur_seq = clist.sequences[clist.position]
					
					-- Clear played sounds
					v.ent.played_sounds = {}

					-- Set required missing values to defaults
					if ( not clist.cur_seq.finish ) then clist.cur_seq.finish = 1 end

					-- Begin the sequence
					BeginSequenceByName( v.ent, clist.cur_seq.name )

					-- A sequence is now active
					clist.running = true
				end
			else
				-- Loop sequence
				if ( v.ent:GetCycle() == 1 ) then
					v.ent:SetCycle( 0 )
				end
				
				-- Footsteps
				if ( v.ent:GetSequence() == v.ent.walk_sequence ) then
					if ( ( v.ent.footsteps ) and ( CurTime() >= v.ent.footstep_update ) ) then
						--print( "footstep:", v.ent.footstep_pos, "/", v.ent.footstep_count )

						v.ent:EmitSound( v.ent.footsteps[v.ent.footstep_pos], 50 )
						
						if ( v.ent.footstep_pos >= v.ent.footstep_count ) then
							v.ent.footstep_pos = 1
						else
							v.ent.footstep_pos = v.ent.footstep_pos + 1
						end
						
						v.ent.footstep_update = CurTime() + v.ent.footstep_interval
					end
				end
			end
		end
	end
end )