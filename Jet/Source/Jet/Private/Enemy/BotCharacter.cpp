// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BotCharacter.h"
#include "../../JetCharacter.h"
#include "Jet/JetProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Anim/BotAnim.h"
#include <Jet/JetGameMode.h>

// Sets default values
ABotCharacter::ABotCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 设置默认行为为停下不动
	CurrentBehavior = EEnemyBehavior::Idle;

    // 创建并初始化HealthComponent
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABotCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    // 查找玩家角色
    PlayerCharacter = Cast<AJetCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

    // 设置巡逻的起始点和终点
    PatrolStartLocation = GetActorLocation();
    PatrolEndLocation = PatrolStartLocation + FVector(PatrolDistance, 0, 0);

    // 每隔一段时间随机选择行为
    GetWorld()->GetTimerManager().SetTimer(BehaviorTimerHandle, this, &ABotCharacter::SwitchBehavior, BehaviorChangeInterval, true);

    // 定时器控制攻击
    GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABotCharacter::AttackPlayer, AttackInterval, true);
}

// Called every frame
void ABotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    switch (CurrentBehavior)
    {
    case EEnemyBehavior::MoveTowardsPlayer:
        MoveTowardsPlayer();
        break;

    case EEnemyBehavior::PatrolLeftRight:
        PatrolLeftRight();
        break;

    case EEnemyBehavior::Idle:
        IdleBehavior();
        break;
    }

    // 检测敌人是否可以攻击玩家
    if (PlayerCharacter && FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation()) <= AttackRange)
    {
        AttackPlayer();
    }
}

// Called to bind functionality to input
void ABotCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABotCharacter::UpdateMovementAnimation()
{
    // 获取当前速度
    float CurrentSpeed = GetVelocity().Size();

    // 获取角色的动画实例
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        // 将速度传递给动画实例
        UBotAnim* BotAnim = Cast<UBotAnim>(AnimInstance);
        if (BotAnim)
        {
            // 更新Speed参数
            BotAnim->UpdateAnimationParameters(CurrentSpeed, bIsShooting);
        }
    }
}

void ABotCharacter::MoveTowardsPlayer()
{
    if (PlayerCharacter)
    {
        FVector Direction = PlayerCharacter->GetActorLocation() - GetActorLocation();
        Direction.Z = 0;  // 保持Z轴不变
        Direction.Normalize();

        // 让敌人面朝玩家
        FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
        SetActorRotation(TargetRotation);

        AddMovementInput(Direction, 0.5f);
    }
}

void ABotCharacter::PatrolLeftRight()
{
    FVector CurrentLocation = GetActorLocation();

    // 如果当前在起始位置，则移动到终点
    if (FVector::Dist(CurrentLocation, PatrolStartLocation) < 10.0f)
    {
        bMovingRight = true;
    }
    // 如果当前在终点，则移动回起始位置
    else if (FVector::Dist(CurrentLocation, PatrolEndLocation) < 10.0f)
    {
        bMovingRight = false;
    }

    // 根据方向移动
    FVector TargetLocation = bMovingRight ? PatrolEndLocation : PatrolStartLocation;
    FVector Direction = TargetLocation - CurrentLocation;
    Direction.Z = 0;  // 保持Z轴不变
    Direction.Normalize();

    AddMovementInput(Direction, 0.5f);
}

void ABotCharacter::IdleBehavior()
{
    // 停止所有移动
    GetCharacterMovement()->StopMovementImmediately();
}

void ABotCharacter::SelectRandomBehavior()
{

}

void ABotCharacter::AttackPlayer()
{
    if (ProjectileClass)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            // 获取角色的世界位置和朝向
            FVector MuzzleLocation = GetActorLocation() + GetActorForwardVector() * MuzzleOffset.X + GetActorUpVector() * MuzzleOffset.Z;
            FRotator MuzzleRotation = GetActorRotation();

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            // 生成子弹
            AJetProjectile* Projectile = World->SpawnActor<AJetProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // 可以在这里设置子弹的初始速度等
                UE_LOG(LogTemp, Warning, TEXT("Bot fired a projectile!"));
            }
        }
    }
}

void ABotCharacter::SwitchBehavior()
{
    // 随机选择行为
    int32 RandomBehavior = FMath::RandRange(0, 2); // 0 = MoveTowardsPlayer, 1 = PatrolLeftRight, 2 = Idle
    CurrentBehavior = static_cast<EEnemyBehavior>(RandomBehavior);
}

void ABotCharacter::Damage_Implementation()
{

}

void ABotCharacter::Death_Implementation()
{
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 禁用网格的碰撞
    SetActorEnableCollision(false);  // 确保 Actor 本身不发生碰撞
    DisableInput(nullptr);
    if (DeathAnimation) // 确保 DeathAnimation 是有效的 UAnimationAsset*
    {
        GetMesh()->PlayAnimation(DeathAnimation, false);// 播放动画，false 表示不循环
        float AnimationDuration = DeathAnimation->GetMaxCurrentTime(); 
        float DestroyDelay = AnimationDuration + 1.0f; // 动画播放完毕后 1 秒销毁

        GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &ABotCharacter::DestroyBot, DestroyDelay, false);
    }
    else
    {
        // 如果没有动画，直接销毁
        GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &ABotCharacter::DestroyBot, 1.0f, false);
    }
    
    PlayerCharacter = Cast<AJetCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (PlayerCharacter)
    {
        PlayerCharacter->IncreaseKillCount();
    }

    UE_LOG(LogTemp, Warning, TEXT("Bot is dead!"));
    

}

void ABotCharacter::DestroyBot()
{
    Destroy();
    AJetGameMode* GameMode = Cast<AJetGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->OnBotKilled();
    }
}


