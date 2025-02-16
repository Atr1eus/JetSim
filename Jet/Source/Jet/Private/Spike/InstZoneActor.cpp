// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike/InstZoneActor.h"
#include <Spike/SpikeActor.h>


// Sets default values
AInstZoneActor::AInstZoneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bShowInstallPrompt = false; // 初始时不显示提示
}

// Called when the game starts or when spawned
void AInstZoneActor::BeginPlay()
{
	Super::BeginPlay();
    // 获取玩家角色，确保正确获取到角色
    AJetCharacter* JetCharacter = Cast<AJetCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (JetCharacter)
    {
        PlayerCharacter = JetCharacter;
        PlayerCharacter->bIsPlayerInZone = false;
    }
}

// Called every frame
void AInstZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // 如果玩家进入区域，显示提示
    if (PlayerCharacter->bIsPlayerInZone)
    {
        bShowInstallPrompt = true;
    }
    else
    {
        bShowInstallPrompt = false;
    }
}

void AInstZoneActor::OnPlayerEnterZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 当玩家进入区域时触发安装炸弹
    if (OtherActor && OtherActor->IsA(APlayerController::StaticClass()))
    {
        PlayerCharacter->bIsPlayerInZone = true;
    }
}

void AInstZoneActor::TriggerInstallSpike()
{
    // 如果玩家在区域内，可以安装炸弹
    if (PlayerCharacter->bIsPlayerInZone)
    {
        ASpikeActor* Spike = GetWorld()->SpawnActor<ASpikeActor>(ASpikeActor::StaticClass(), GetActorLocation(), GetActorRotation());
        if (Spike)
        {
            Spike->InstallSpike();
        }
    }
}

