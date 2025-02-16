// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike/InstZoneActor.h"
#include <Spike/SpikeActor.h>


// Sets default values
AInstZoneActor::AInstZoneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bShowInstallPrompt = false; // ��ʼʱ����ʾ��ʾ
}

// Called when the game starts or when spawned
void AInstZoneActor::BeginPlay()
{
	Super::BeginPlay();
    // ��ȡ��ҽ�ɫ��ȷ����ȷ��ȡ����ɫ
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

    // �����ҽ���������ʾ��ʾ
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
    // ����ҽ�������ʱ������װը��
    if (OtherActor && OtherActor->IsA(APlayerController::StaticClass()))
    {
        PlayerCharacter->bIsPlayerInZone = true;
    }
}

void AInstZoneActor::TriggerInstallSpike()
{
    // �������������ڣ����԰�װը��
    if (PlayerCharacter->bIsPlayerInZone)
    {
        ASpikeActor* Spike = GetWorld()->SpawnActor<ASpikeActor>(ASpikeActor::StaticClass(), GetActorLocation(), GetActorRotation());
        if (Spike)
        {
            Spike->InstallSpike();
        }
    }
}

