// Copyright Epic Games, Inc. All Rights Reserved.

#include "JetGameMode.h"
#include "JetCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Public/JetController.h"
#include <Kismet/GameplayStatics.h>
#include "Public/Enemy/BotCharacter.h"
#include "Jet/Public/HUD/JetHUD.h"

AJetGameMode::AJetGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	RemainingTime = 90; // ��ʼ 90 ��
	bSpikePlanted = false;
}

void AJetGameMode::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("test"));
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AJetGameMode::CountdownTimer, 1.0f, true);
    MyHUD = Cast<AJetHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void AJetGameMode::CountdownTimer()
{
    RemainingTime--;

    // ��ʾ����ʱ��Ϣ
    UE_LOG(LogTemp, Warning, TEXT("Time Left: %d seconds"), RemainingTime);

    if (MyHUD)
    {
        MyHUD->UpdateGameState(bSpikePlanted, RemainingTime);
    }
    if (RemainingTime <= 0)
    {
        if (bSpikePlanted)
        {
            EndGame(true); // Spike ��ʱ����������Ϸʤ��
        }
        else
        {
            EndGame(false); // ��ʼ 90s ��ʱ����������Ϸʧ��
        }
    }
}

void AJetGameMode::OnSpikePlanted()
{
    if (!bSpikePlanted)
    {
        bSpikePlanted = true;
        RemainingTime = 45; // ���ü�ʱ��Ϊ 45 ��
        UE_LOG(LogTemp, Warning, TEXT("Spike Planted! Timer reset to 45 seconds."));
    }
}

void AJetGameMode::OnBotKilled()
{
    TArray<AActor*> BotActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABotCharacter::StaticClass(), BotActors);

    if (BotActors.Num() == 0)
    {
        EndGame(true); // Bot ȫ����������Ϸʤ��
    }
}

void AJetGameMode::EndGame(bool bPlayerWon)
{
    GetWorldTimerManager().ClearTimer(GameTimerHandle); // ֹͣ��ʱ��

    if (bPlayerWon)
    {
        UE_LOG(LogTemp, Warning, TEXT("Game Over: You Win!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Game Over: You Lose!"));
    }

    if (MyHUD)
    {
        MyHUD->ShowEndGameUI(bPlayerWon);
    }
}


