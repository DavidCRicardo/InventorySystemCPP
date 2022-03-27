#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BUIInventoryEntry.generated.h"

UCLASS(Abstract)
class UBUIInventoryEntry : public UUserWidget, public IUserObjectListEntry
{
private:
	GENERATED_BODY()
protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NameLabel;

	UPROPERTY(meta=(BindWidget))
	class UImage* IconImage;
};
