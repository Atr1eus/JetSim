// Fill out your copyright notice in the Description page of Project Settings.


#include "JetController.h"
#include <HUD/RestartWidget.h>
#include "HUD/HealthWidget.h"

void AJetController::ShowRestartWidget()
{
	if (RestartWidgetClass != nullptr)
	{
		SetPause(true);
		SetInputMode(FInputModeUIOnly());

		bShowMouseCursor = true;
		RestartWidget = CreateWidget<URestartWidget>(this, RestartWidgetClass);
		RestartWidget->AddToViewport();
	}
}

// 实际上是销毁
void AJetController::HideRestartWidget()
{
	RestartWidget->RemoveFromParent();
	RestartWidget->Destruct();
	SetPause(false);
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void AJetController::UpdateHealthPercent(float HealthPercent)
{
	if (HealthWidget != nullptr)
	{
		HealthWidget->UpdateHealthPercent(HealthPercent);
	}
}

void AJetController::BeginPlay()
{
	Super::BeginPlay();

	if (HealthWidgetClass != nullptr) 
	{
		HealthWidget = CreateWidget<UHealthWidget>(this, HealthWidgetClass);
		HealthWidget->AddToViewport();
	}
	
}
