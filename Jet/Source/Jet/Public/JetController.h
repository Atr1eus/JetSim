// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <HUD/RestartWidget.h>
#include <HUD/HealthWidget.h>
#include "JetController.generated.h"



class URestartWidget;
class UHealthtWidget;
/**
 * 
 */
UCLASS()
class JET_API AJetController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URestartWidget> RestartWidgetClass;

	void ShowRestartWidget();
	void HideRestartWidget();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHealthWidget> HealthWidgetClass;

	void UpdateHealthPercent(float HealthPercent);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<URestartWidget> RestartWidget;
	
	UPROPERTY()
	TObjectPtr<UHealthWidget> HealthWidget;
};
