// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RestartWidget.h"
#include "Components/Button.h" 
#include <JetController.h>
#include <Kismet/GameplayStatics.h>

void URestartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (RestartButton != nullptr)
	{
		RestartButton->OnClicked.AddDynamic(this, &URestartWidget::OnRestartClicked);
	}
	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &URestartWidget::OnExitClicked);
	}
}

void URestartWidget::OnRestartClicked()
{
	AJetController* PlayerController = Cast<AJetController>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		//Hide
		PlayerController->HideRestartWidget();
	}
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void URestartWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
