----------------------------------------------------
-- Enum:
--   CHESSTEAMTYPE
-- Purpose:
--   Chess piece teams.
----------------------------------------------------

CHESSTEAMTYPE_MIN = 1
CHESSTEAMTYPE_VILLAINS = 1
CHESSTEAMTYPE_HEROES = 2
CHESSTEAMTYPE_MAX = 2

function TeamNameToIndex( name )
	if ( name == "Villains" ) then return CHESSTEAMTYPE_VILLAINS end
	if ( name == "Heroes" ) then return CHESSTEAMTYPE_HEROES end
end

function TeamIndexToName( index )
	if ( index == CHESSTEAMTYPE_VILLAINS ) then return "Villains" end
	if ( index == CHESSTEAMTYPE_HEROES ) then return "Heroes" end
end

----------------------------------------------------
-- Enum:
--   CHESSPIECETYPE
-- Purpose:
--   Chess piece types.
----------------------------------------------------

CHESSPIECETYPE_MIN = 1
CHESSPIECETYPE_KING = 1
CHESSPIECETYPE_QUEEN = 2
CHESSPIECETYPE_ROOK = 3
CHESSPIECETYPE_BISHOP = 4
CHESSPIECETYPE_KNIGHT = 5
CHESSPIECETYPE_PAWN = 6
CHESSPIECETYPE_MAX = 6

function PieceNameToIndex( name )
	if ( name == "King" ) then return CHESSPIECETYPE_KING end
	if ( name == "Queen" ) then return CHESSPIECETYPE_QUEEN end
	if ( name == "Rook" ) then return CHESSPIECETYPE_ROOK end
	if ( name == "Bishop" ) then return CHESSPIECETYPE_BISHOP end
	if ( name == "Knight" ) then return CHESSPIECETYPE_KNIGHT end
	if ( name == "Pawn" ) then return CHESSPIECETYPE_PAWN end
end

function PieceIndexToName( index )
	if ( index == CHESSPIECETYPE_KING ) then return "King" end
	if ( index == CHESSPIECETYPE_QUEEN ) then return "Queen" end
	if ( index == CHESSPIECETYPE_ROOK ) then return "Rook" end
	if ( index == CHESSPIECETYPE_BISHOP ) then return "Bishop" end
	if ( index == CHESSPIECETYPE_KNIGHT ) then return "Knight" end
	if ( index == CHESSPIECETYPE_PAWN ) then return "Pawn" end
end

----------------------------------------------------
-- Enum:
--   CHESSMOVETYPE
-- Purpose:
--   Special chess movement types - used by pawns.
----------------------------------------------------

CHESSMOVETYPE_FREEONLY = 1
CHESSMOVETYPE_ENEMYONLY = 2

----------------------------------------------------
-- Enum:
--   CHESSTURNTYPE
-- Purpose:
--   Current turn type.
----------------------------------------------------

CHESSTURNTYPE_YOU = 0
CHESSTURNTYPE_OPPONENT = 1
CHESSTURNTYPE_PENDING = 2