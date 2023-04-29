#pragma once
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "PauseUI.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API UPauseUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//functions
	void NativeConstruct() override;
	//ui properties
	UPROPERTY(meta = (BindWidget))
		class UImage* background_;
};
