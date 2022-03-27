#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BUIInventoryPanel.generated.h"

UCLASS(Abstract)
class UBUIInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UPROPERTY( meta = ( BindWidget ) )
	class UListView* InventoryListView;
};
