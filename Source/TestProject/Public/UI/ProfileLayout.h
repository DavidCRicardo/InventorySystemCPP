#pragma once

#include "CoreMinimal.h"
#include "WindowLayout.h"
#include "ProfileLayout.generated.h"

UCLASS()
class TESTPROJECT_API UProfileLayout : public UWindowLayout
{
	GENERATED_BODY()
	
public:
	UProfileLayout();

	virtual void RefreshWindow() override;
	virtual void InitializeSlots() override;

protected:
	virtual void NativeConstruct() override;
};
