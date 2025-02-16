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
                // Plant spike下包
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
    // 计算按键持续时间
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
        Character->GetCharacterMovement()->DisableMovement();  // 禁用角色移动
        UE_LOG(LogTemp, Warning, TEXT("planting."));
        Install();
        SkeletalMesh = Spike->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMesh != nullptr)
        {
            UAnimSequence* InstallationAnimSequence = Spike->InstallAnimation; // 获取动画序列
            if (InstallationAnimSequence)
            {
                SkeletalMesh->SetAnimation(InstallationAnimSequence);
                SkeletalMesh->Play(true); // true 代表循环播放
            }
        }
        if (Spike->PlantSound != nullptr)
        {
            USoundBase* InstallationSound = Spike->PlantSound; // 获取安装声音
            if (InstallationSound)
            {
                UGameplayStatics::SpawnSoundAttached(
                    InstallationSound,           // 要播放的声音
                    SkeletalMesh,                // 附着目标
                    NAME_None,                   // 附着的插槽名 (None 代表默认)
                    FVector::ZeroVector,         // 位置偏移
                    EAttachLocation::KeepRelativeOffset,
                    false                         // 是否循环播放
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

    // 停止计时
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
            // 获取玩家的控制器
            APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
            if (PlayerController)
            {

                // 计算生成位置
                FVector SpawnLocation = CalculateSpawnLocation();
                // 获取地面法线并计算旋转
                FVector GroundNormal = GetGroundNormal(SpawnLocation);
                FRotator FinalRotation = GroundNormal.Rotation();

                // 确保物体底面朝下
                FinalRotation.Pitch += 270.0f;

                // 生成 SpikeActor
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
    FVector End = Start - FVector(0, 0, 1000); // 向下1000单位进行检测

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(Character); // 忽略自身

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
    {
        // 返回地面法线
        return HitResult.Normal;
    }
    else
    {
        // 如果没检测到地面，返回一个默认法线
        return FVector(0, 0, 1); // 默认返回正向 Z 轴法线
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
    FVector End = Start - FVector(0, 0, 1000); // 向下1000单位进行检测

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(Character); // 忽略自身

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
    {
        // 获取地面高度
        SpawnLocation.Z = HitResult.ImpactPoint.Z;
    }
    else
    {
        SpawnLocation.Z = 0.0f;
        UE_LOG(LogTemp, Warning, TEXT("No ground detected, setting default Z position."));
    }

    return SpawnLocation;
}

