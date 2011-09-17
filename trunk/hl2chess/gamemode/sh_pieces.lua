-- Chess piece spawn coordinates, models and animations
-- Coordinates are relative to team origin
CHESS_PIECE_INFO = {
	[CHESSPIECETYPE_KING] = {
		spawns = {
			{ x = 4, y = 0 },
		},

		[CHESSTEAMTYPE_VILLAINS] = {
			{
				model = "models/gman_high.mdl",
				idle = "idle_subtle",
				walk = "walk_all",
				attack = {
							{
								name = "throw1",
								ragdoll_progress = 0.3,
								--[[sounds = {
									{
										name = "physics/body/body_medium_impact_hard5.wav",
										progress = 0.28,
									},
								},--]]
							},
						 },
						 
				attack_distance = 40,
				move_speed = 70,
				death_sound = "vo/Citadel/gman_exit10.wav",
			},
		},
		
		[CHESSTEAMTYPE_HEROES] = {
			{
				model = "models/Eli.mdl",
				idle = "idle_subtle",
				walk = "walk_all",
				attack = {
							{
								name = "ThrowItem",
								ragdoll_progress = 0.5,
								finish = 0.7,
								--[[sounds = {
									{
										name = "vo/Citadel/eli_notobreen.wav",
										progress = 0.35,
									},
								},--]]
							},
						 },
				
				attack_distance = 32,
				move_speed = 55,
				death_sound = "vo/k_lab/eli_notquite03.wav",
				
				footstep_interval = 0.6,

				footsteps = {
					"player/footsteps/metal1.wav",
					"player/footsteps/dirt1.wav",
					"player/footsteps/metal2.wav",
					"player/footsteps/dirt2.wav",
				},
			},
		},
	},

	[CHESSPIECETYPE_QUEEN] = {
		spawns = {
			{ x = 3, y = 0 },
		},

		[CHESSTEAMTYPE_VILLAINS] = {
			{
				model = "models/breen.mdl",
				idle = "idle01",
				walk = "run_all_panicked",
				attack = {
							{
								name = "swing",
								ragdoll_progress = 0.3,
								--[[sounds = {
									{
										name = "vo/Breencast/br_welcome06.wav",
										progress = 0,
									},
								},--]]
							},
						 },
				
				attack_distance = 40,
				move_speed = 170,
				death_sound = "vo/Breencast/br_tofreeman06.wav",
				
				footstep_interval = 0.3,
				
				footsteps = {
					"player/footsteps/concrete1.wav",
					"player/footsteps/concrete2.wav",
					"player/footsteps/concrete3.wav",
					"player/footsteps/concrete4.wav",
				},
			},
		},
	
		[CHESSTEAMTYPE_HEROES] = {
			{
				model = "models/alyx.mdl",
				idle = "idle_subtle",
				walk = "run_all",
				attack = {
							{
								name = "MeleeAttack01",
								ragdoll_progress = 0.4,
								--[[sounds = {
									{
										name = "vo/npc/Alyx/al_excuse03.wav",
										progress = 0.25,
									},
								},--]]
							},
						 },
				
				attack_distance = 40,
				move_speed = 170,
				death_sound = "vo/npc/Alyx/ohno_startle03.wav",
				
				footstep_interval = 0.3,
				
				footsteps = {
					"player/footsteps/dirt1.wav",
					"player/footsteps/dirt2.wav",
					"player/footsteps/dirt3.wav",
					"player/footsteps/dirt4.wav",
				},
			},
		},
	},
	
	[CHESSPIECETYPE_ROOK] = {
		spawns = {
			{ x = 0, y = 0 },
			{ x = 7, y = 0 },		
		},

		[CHESSTEAMTYPE_VILLAINS] = {
			{
				model = "models/Zombie/Classic.mdl",
				idle = "Idle01",
				walk = "FireWalk",
				attack = {
							{
								name = "attackB",
								ragdoll_progress = 0.4,
								sounds = {
									{
										name = "npc/zombie/zo_attack1.wav",
										progress = 0,
									},
								},
							},
						 },
				
				attack_distance = 40,
				move_speed = 140,
				death_sound = "npc/zombie/zombie_pain5.wav",
				
				footstep_interval = 0.5,
				
				footsteps = {
					"npc/zombie/foot1.wav",
					"npc/zombie/foot2.wav",
					"npc/zombie/foot3.wav",
				},
			},
		},
		
		[CHESSTEAMTYPE_HEROES] = {
			{
				model = "models/vortigaunt.mdl",
				idle = "Idle01",
				walk = "Run_all",
				attack = {
							{
								name = "MeleeLow",
								ragdoll_progress = 0.45,
								--[[sounds = {
									{
										name = "vo/npc/vortigaunt/onward.wav",
										progress = 0.35,
									},
								},--]]
							},
						 },
				
				attack_distance = 64,
				move_speed = 170,
				death_sound = "vo/npc/vortigaunt/hopeless.wav",
				
				footstep_interval = 0.4,
				
				footsteps = {
					"player/footsteps/concrete1.wav",
					"player/footsteps/concrete2.wav",
					"player/footsteps/concrete3.wav",
					"player/footsteps/concrete4.wav",
				},
			},
		},
	},
	
	[CHESSPIECETYPE_BISHOP] = {
		spawns = {
			{ x = 2, y = 0 },
			{ x = 5, y = 0 },		
		},

		[CHESSTEAMTYPE_VILLAINS] = {
			{
				model = "models/Zombie/Fast.mdl",
				idle = "idle",
				walk = "Run",
				attack = {
							{
								name = "BR2_Attack",
								ragdoll_progress = 0.4,
								sounds = {
									{
										name = "npc/fast_zombie/claw_strike1.wav",
										progress = 0.3,
									},
									
									{
										name = "npc/fast_zombie/claw_strike3.wav",
										progress = 0.4,
									},
								},
							},
						 },
				
				attack_distance = 40,
				move_speed = 190,
				death_sound = "npc/fast_zombie/leap1.wav",
				
				footstep_interval = 0.2,
				
				footsteps = {
					"npc/fast_zombie/foot1.wav",
					"npc/fast_zombie/foot2.wav",
					"npc/fast_zombie/foot3.wav",
					"npc/fast_zombie/foot4.wav",
				},
			},
		},
		
		[CHESSTEAMTYPE_HEROES] = {
			{
				model = "models/Barney.mdl",
				idle = "idle_subtle",
				walk = "run_all_panicked",
				attack = {
							{
								name = "throw1",
								ragdoll_progress = 0.3,
								--[[sounds = {
									{
										name = "vo/npc/Barney/ba_goingdown.wav",
										progress = 0,
									},
								},--]]
							},
						 },
				
				attack_distance = 32,
				move_speed = 170,
				death_sound = "vo/npc/Barney/ba_ohshit03.wav",
				
				footstep_interval = 0.3,
				
				footsteps = {
					"player/footsteps/concrete1.wav",
					"player/footsteps/concrete2.wav",
					"player/footsteps/concrete3.wav",
					"player/footsteps/concrete4.wav",
				},
			},
		},
	},
	
	[CHESSPIECETYPE_KNIGHT] = {
		spawns = {
			{ x = 1, y = 0 },
			{ x = 6, y = 0 },	
		},

		[CHESSTEAMTYPE_VILLAINS] = {
			{
				model = "models/antlion_guard.mdl",
				idle = "ragdoll",
				walk = "walk1",
				attack = {
							{
								name = "shove",
								ragdoll_progress = 0.2,
								sounds = {
									{
										name = "npc/antlion_guard/angry1.wav",
										progress = 0,
									},
								},
							},
						 },

				attack_distance = 128,		
				move_speed = 120,
				death_sound = "npc/antlion_guard/antlion_guard_die1.wav",
				
				footstep_interval = 0.3,
				
				footsteps = {
					"npc/antlion_guard/foot_heavy1.wav",
					"npc/antlion_guard/foot_heavy2.wav",
				},
			},
		},
		
		[CHESSTEAMTYPE_HEROES] = {
			{
				model = "models/dog.mdl",
				idle = "idle01",
				walk = "walk_all",
				attack = {
							{
								name = "pound",
								ragdoll_progress = 0.3,
								--[[sounds = {
									{
										name = "npc/dog/dog_angry1.wav",
										progress = 0,
									},
									
									{
										name = "npc/dog/car_impact1.wav",
										progress = 0.28,
									},
								},--]]
							},
						 },

				attack_distance = 64,
				move_speed = 100,
				death_sound = "npc/dog/dog_scared1.wav",

				footstep_interval = 0.5,
				
				footsteps = {
					"npc/dog/dog_footstep1.wav",
					"npc/dog/dog_footstep2.wav",
					"npc/dog/dog_footstep3.wav",
					"npc/dog/dog_footstep4.wav",
				},
			},
		},
	},
	
	[CHESSPIECETYPE_PAWN] = {
		spawns = {
			{ x = 0, y = 1 },
			{ x = 1, y = 1 },
			{ x = 2, y = 1 },
			{ x = 3, y = 1 },
			{ x = 4, y = 1 },
			{ x = 5, y = 1 },
			{ x = 6, y = 1 },
			{ x = 7, y = 1 },
		},

		[CHESSTEAMTYPE_VILLAINS] = {
			{
				model = "models/Police.mdl",
				idle = "batonidle1",
				walk = "walk_all",
				attack = {
							{
								name = "thrust",
								ragdoll_progress = 0.4,
								sounds = {
									{
										name = "npc/metropolice/vo/moveit.wav",
										progress = 0.1,
									},
								},
							},
						 },
				
				attack_distance = 32,
				move_speed = 80,
				death_sound = "npc/metropolice/die1.wav",
				
				footstep_interval = 0.55,
				
				footsteps = {
					"npc/footsteps/hardboot_generic1.wav",
					"npc/footsteps/hardboot_generic2.wav",
					"npc/footsteps/hardboot_generic3.wav",
					"npc/footsteps/hardboot_generic4.wav",
					"npc/footsteps/hardboot_generic5.wav",
					"npc/footsteps/hardboot_generic6.wav",
					"npc/footsteps/hardboot_generic8.wav",
				},
			},
		},
		
		[CHESSTEAMTYPE_HEROES] = {
			{
				model = "models/Humans/Group01/Female_01.mdl",
				idle = "idle_subtle",
				walk = "walk_all",
				attack = {
							{
								name = "MeleeAttack01",
								ragdoll_progress = 0.4,
								--[[sounds = {
									{
										name = "vo/npc/female01/excuseme01.wav",
										progress = 0.25,
									},
								},--]]
							},
						 },
				
				attack_distance = 32,
				move_speed = 80,
				death_sound = "vo/npc/female01/ow01.wav",
			},
		},
	},
}