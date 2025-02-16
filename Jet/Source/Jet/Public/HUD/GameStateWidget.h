// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStateWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class JET_API UGameStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountDownText;

	UPROPERTY(meta = (BindWidget))
	UImage* SpikeIcon;

	UFUNCTION(BlueprintCallable)
	void UpdateGameState(int32 TimeRemaining, bool bSpikePlanted);
	
};
