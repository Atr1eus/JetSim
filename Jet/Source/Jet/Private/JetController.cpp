// Fill out your copyright notice in the Description page of Project Settings.


#include "JetController.h"
#include <HUD/RestartWidget.h>
#include "HUD/HealthWidget.h"
#include "HUD/BeginMenuWidget.h"
#include "HUD/JetHUD.h"
#include <Blueprint/WidgetBlueprintLibrary.h>

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

void AJetController::ShowMenu()
{
	if (BeginMenuWidgetClass != nullptr)
	{
		SetPause(true);
		SetInputMode(FInputModeUIOnly());

		bShowMouseCursor = true;
		BeginMenuWidget = CreateWidget<UBeginMenuWidget>(this, BeginMenuWidgetClass);
		BeginMenuWidget->AddToViewport();
	}
}

void AJetController::DestroyTopmostWidget()
{
	BeginMenuWidget->RemoveFromParent();
	BeginMenuWidget->Destruct();
	SetPause(false);
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void AJetController::BeginPlay()
{
	Super::BeginPlay();

	JetHUD = Cast<AJetHUD>(GetHUD());
	if (JetHUD != nullptr)
	{
		JetHUD->CreateHealthWidget();
		JetHUD->ShowGameState();
		JetHUD->ShowSkillState();
	}
}

void AJetController::UpdateHealthPercent(float HealthPercent)
{
	if (JetHUD != nullptr)
	{
		JetHUD->UpdateHealth(HealthPercent);
	}
}
