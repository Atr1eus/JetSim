// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BeginMenuWidget.generated.h"

class UButton;
class UMediaPlayer;
class UMediaSoundComponent;
class UMaterialInstanceDynamic;
class UTexture2D;

/**
 * 
 */
UCLASS()
class JET_API UBeginMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PlayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video")
	UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video")
	UMediaSoundComponent* MediaSoundComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video")
	UMaterialInstanceDynamic* BackgroundMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video")
	UTexture2D* VideoTexture;

protected:
	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnExitClicked();

private:
	void SetupVideoBackground();
	
	
};
