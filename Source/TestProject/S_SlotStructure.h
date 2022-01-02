#pragma once


USTRUCT(BlueprintType)
struct FS_SlotStructure
{
	GENERATED_BODY();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	class AItem* ItemStructure;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	uint8 Amount;
};