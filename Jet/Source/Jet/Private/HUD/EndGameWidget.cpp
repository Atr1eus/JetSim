// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EndGameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include <JetController.h>

void UEndGameWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UEndGameWidget::OnRestartClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UEndGameWidget::OnExitClicked);
    }
}

void UEndGameWidget::SetVictoryOrDefeated(bool bIsVictory)
{
    if (EndGameText)
    {
        if (bIsVictory)
        {
            EndGameText->SetText(FText::FromString(TEXT("Victory")));
        }
        else
        {
            EndGameText->SetText(FText::FromString(TEXT("Defeated")));
        }
    }
}

void UEndGameWidget::OnRestartClicked()
{
    AJetController* PlayerController = Cast<AJetController>(GetOwningPlayer());
    if (PlayerController != nullptr)
    {
        //Hide
        //PlayerController->DestroyTopmostWidget();
    }
    UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void UEndGameWidget::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
