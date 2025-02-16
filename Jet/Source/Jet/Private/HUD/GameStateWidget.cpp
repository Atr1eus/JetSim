// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameStateWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UGameStateWidget::UpdateGameState(int32 TimeRemaining, bool bSpikePlanted)
{
    if (CountDownText)
    {
        CountDownText->SetText(FText::FromString(FString::Printf(TEXT("Time: %d"), TimeRemaining)));
        CountDownText->SetVisibility(bSpikePlanted ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }

    // ����Spikeͼ�����ʾ
    if (SpikeIcon)
    {
        SpikeIcon->SetVisibility(bSpikePlanted ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

