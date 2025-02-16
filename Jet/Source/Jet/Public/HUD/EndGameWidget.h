// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class JET_API UEndGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EndGameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;
	
	// 设置文本内容（Victory 或 Defeated）
	UFUNCTION(BlueprintCallable, Category = "EndGameUI")
	void SetVictoryOrDefeated(bool bIsVictory);

	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnExitClicked();
};
