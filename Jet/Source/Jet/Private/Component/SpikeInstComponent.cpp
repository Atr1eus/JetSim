// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SpikeInstComponent.h"
#include "Jet/JetCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Jet/JetGameMode.h>


// Sets default values for this component's properties
USpikeInstComponent::USpikeInstComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
    bIsHoldingKey = false;
    KeyPressDuration = 0.0f;
    MaxKeyPressDuration = 4.0f;
    bSpikeInstalled = false;
}


// Called when the game starts
void USpikeInstComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        APawn* Pawn = PlayerController->GetPawn();
        Character = Cast<AJetCharacter>(Pawn);
        if (Character != nullptr)
        {
            if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
            {
                // Plant spike�°�
                EnhancedInputComponent->BindAction(PlantAction, ETriggerEvent::Started, this, &USpikeInstComponent::StartInstallSpike);
                EnhancedInputComponent->BindAction(PlantAction, ETriggerEvent::Completed, this, &USpikeInstComponent::StopInstallSpike);
            }
        }       
    }
}


// Called every frame
void USpikeInstComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
    // ���㰴������ʱ��
    if (bIsHoldingKey)
    {
        KeyPressDuration += DeltaTime;
        if (KeyPressDuration >= MaxKeyPressDuration && !bSpikeInstalled)
        {
            bSpikeInstalled = true;
            AJetGameMode* GameMode = Cast<AJetGameMode>(GetWorld()->GetAuthGameMode());
            if (GameMode)
            {
                GameMode->OnSpikePlanted();
            }
            SkeletalMesh->Stop();
        }
    }
}

void USpikeInstComponent::StartInstallSpike()
{
    
    if (Character->bIsPlayerInZone && !bSpikeInstalled)
    {
        bIsHoldingKey = true;
        KeyPressDuration = 0.0f;
        Character->GetCharacterMovement()->DisableMovement();  // ���ý�ɫ�ƶ�
        UE_LOG(LogTemp, Warning, TEXT("planting."));
        Install();
        SkeletalMesh = Spike->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMesh != nullptr)
        {
            UAnimSequence* InstallationAnimSequence = Spike->InstallAnimation; // ��ȡ��������
            if (InstallationAnimSequence)
            {
                SkeletalMesh->SetAnimation(InstallationAnimSequence);
                SkeletalMesh->Play(true); // true ����ѭ������
            }
        }
        if (Spike->PlantSound != nullptr)
        {
            USoundBase* InstallationSound = Spike->PlantSound; // ��ȡ��װ����
            if (InstallationSound)
            {
                UGameplayStatics::SpawnSoundAttached(
                    InstallationSound,           // Ҫ���ŵ�����
                    SkeletalMesh,                // ����Ŀ��
                    NAME_None,                   // ���ŵĲ���� (None ����Ĭ��)
                    FVector::ZeroVector,         // λ��ƫ��
                    EAttachLocation::KeepRelativeOffset,
                    false                         // �Ƿ�ѭ������
                );
            }
        }
    }
}

void USpikeInstComponent::StopInstallSpike()
{
    Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    if (!bSpikeInstalled && Spike != nullptr)
    {
        Spike->Destroy();
        Spike = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("SpikeActor destroyed due to insufficient hold time."));
    }

    // ֹͣ��ʱ
    bIsHoldingKey = false;
}

void USpikeInstComponent::Install()
{
    if (Character == nullptr || Character->GetController() == nullptr)
    {
        return;
    }
    if (SpikeClass != nullptr)
    {
        UWorld* const World = GetWorld();
        if (World != nullptr)
        {
            // ��ȡ��ҵĿ�����
            APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
            if (PlayerController)
            {

                // ��������λ��
                FVector SpawnLocation = CalculateSpawnLocation();
                // ��ȡ���淨�߲�������ת
                FVector GroundNormal = GetGroundNormal(SpawnLocation);
                FRotator FinalRotation = GroundNormal.Rotation();

                // ȷ��������泯��
                FinalRotation.Pitch += 270.0f;

                // ���� SpikeActor
                FActorSpawnParameters ActorSpawnParams;
                ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

                Spike = World->SpawnActor<ASpikeActor>(SpikeClass, SpawnLocation, FinalRotation, ActorSpawnParams);
                if (Spike)
                {
                    UE_LOG(LogTemp, Warning, TEXT("SpikeActor spawned successfully!"));
                    Spike->SetActorHiddenInGame(false);
                    Spike->SetActorEnableCollision(true);
                    Spike->InstallSpike();
                    //bSpikeInstalled = true;
                }
            }  
        }
    }
}

FVector USpikeInstComponent::GetGroundNormal(FVector SpawnLocation)
{
    FVector Start = SpawnLocation;
    FVector End = Start - FVector(0, 0, 1000); // ����1000��λ���м��

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(Character); // ��������

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
    {
        // ���ص��淨��
        return HitResult.Normal;
    }
    else
    {
        // ���û��⵽���棬����һ��Ĭ�Ϸ���
        return FVector(0, 0, 1); // Ĭ�Ϸ������� Z �ᷨ��
    }
}

FVector USpikeInstComponent::CalculateSpawnLocation()
{
    FVector ForwardVector = Character->GetActorForwardVector();
    FVector SpawnLocation = Character->GetActorLocation() + ForwardVector * 100.0f; 

    return GetGroundHeight(SpawnLocation);
}

FVector USpikeInstComponent::GetGroundHeight(FVector SpawnLocation)
{
    FVector Start = SpawnLocation;
    FVector End = Start - FVector(0, 0, 1000); // ����1000��λ���м��

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(Character); // ��������

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
    {
        // ��ȡ����߶�
        SpawnLocation.Z = HitResult.ImpactPoint.Z;
    }
    else
    {
        SpawnLocation.Z = 0.0f;
        UE_LOG(LogTemp, Warning, TEXT("No ground detected, setting default Z position."));
    }

    return SpawnLocation;
}

