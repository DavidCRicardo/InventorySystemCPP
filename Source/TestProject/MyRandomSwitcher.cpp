#include "MyRandomSwitcher.h"

#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>

void UMyRandomSwitcher::NativeConstruct()
{
	Super::NativeConstruct();
	
	ButtonForTab1->OnClicked.AddUniqueDynamic(this, &UMyRandomSwitcher::OnButtonClicked);
	ButtonForTab2->OnClicked.AddUniqueDynamic(this, &UMyRandomSwitcher::OnButtonClicked2);
}

void UMyRandomSwitcher::OnButtonClicked()
{
	My_WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UMyRandomSwitcher::OnButtonClicked2()
{
	My_WidgetSwitcher->SetActiveWidgetIndex(1);
}

