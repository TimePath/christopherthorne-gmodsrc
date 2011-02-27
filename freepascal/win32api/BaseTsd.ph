{$IFNDEF _BASETSD_H_}
{$DEFINE _BASETSD_H_}

type
	INT_PTR = int;
	PINT_PTR = ^int;
	
	UINT_PTR = uint;
	PUINT_PTR = ^uint;
	
type
	LONG_PTR = long;
	PLONG_PTR = ^long;
	
	ULONG_PTR = ulong;
	PULONG_PTR = ^ulong;
	
//
// Legacy thread affinity.
//

type
	KAFFINITY = ULONG_PTR;
	PKAFFINITY = ^KAFFINITY;

{$ENDIF} // _BASETSD_H_