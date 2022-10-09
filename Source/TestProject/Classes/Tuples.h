#pragma once

struct FReturnTupleBoolInt
{
	bool Success = true; int8 Index = -1;
};

struct FReturnTupleSlotNativeID {
	bool NativeFromInventory = false;
	bool NativeFromContainer = false;
	uint8 Index = 0;
};