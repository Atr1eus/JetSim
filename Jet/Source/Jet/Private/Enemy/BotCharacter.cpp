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

	// ����Ĭ����ΪΪͣ�²���
	CurrentBehavior = EEnemyBehavior::Idle;

    // ��������ʼ��HealthComponent
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABotCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    // ������ҽ�ɫ
    PlayerCharacter = Cast<AJetCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

    // ����Ѳ�ߵ���ʼ����յ�
    PatrolStartLocation = GetActorLocation();
    PatrolEndLocation = PatrolStartLocation + FVector(PatrolDistance, 0, 0);

    // ÿ��һ��ʱ�����ѡ����Ϊ
    GetWorld()->GetTimerManager().SetTimer(BehaviorTimerHandle, this, &ABotCharacter::SwitchBehavior, BehaviorChangeInterval, true);

    // ��ʱ�����ƹ���
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

    // �������Ƿ���Թ������
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
    // ��ȡ��ǰ�ٶ�
    float CurrentSpeed = GetVelocity().Size();

    // ��ȡ��ɫ�Ķ���ʵ��
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        // ���ٶȴ��ݸ�����ʵ��
        UBotAnim* BotAnim = Cast<UBotAnim>(AnimInstance);
        if (BotAnim)
        {
            // ����Speed����
            BotAnim->UpdateAnimationParameters(CurrentSpeed, bIsShooting);
        }
    }
}

void ABotCharacter::MoveTowardsPlayer()
{
    if (PlayerCharacter)
    {
        FVector Direction = PlayerCharacter->GetActorLocation() - GetActorLocation();
        Direction.Z = 0;  // ����Z�᲻��
        Direction.Normalize();

        // �õ����泯���
        FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
        SetActorRotation(TargetRotation);

        AddMovementInput(Direction, 0.5f);
    }
}

void ABotCharacter::PatrolLeftRight()
{
    FVector CurrentLocation = GetActorLocation();

    // �����ǰ����ʼλ�ã����ƶ����յ�
    if (FVector::Dist(CurrentLocation, PatrolStartLocation) < 10.0f)
    {
        bMovingRight = true;
    }
    // �����ǰ���յ㣬���ƶ�����ʼλ��
    else if (FVector::Dist(CurrentLocation, PatrolEndLocation) < 10.0f)
    {
        bMovingRight = false;
    }

    // ���ݷ����ƶ�
    FVector TargetLocation = bMovingRight ? PatrolEndLocation : PatrolStartLocation;
    FVector Direction = TargetLocation - CurrentLocation;
    Direction.Z = 0;  // ����Z�᲻��
    Direction.Normalize();

    AddMovementInput(Direction, 0.5f);
}

void ABotCharacter::IdleBehavior()
{
    // ֹͣ�����ƶ�
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
            // ��ȡ��ɫ������λ�úͳ���
            FVector MuzzleLocation = GetActorLocation() + GetActorForwardVector() * MuzzleOffset.X + GetActorUpVector() * MuzzleOffset.Z;
            FRotator MuzzleRotation = GetActorRotation();

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            // �����ӵ�
            AJetProjectile* Projectile = World->SpawnActor<AJetProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // ���������������ӵ��ĳ�ʼ�ٶȵ�
                UE_LOG(LogTemp, Warning, TEXT("Bot fired a projectile!"));
            }
        }
    }
}

void ABotCharacter::SwitchBehavior()
{
    // ���ѡ����Ϊ
    int32 RandomBehavior = FMath::RandRange(0, 2); // 0 = MoveTowardsPlayer, 1 = PatrolLeftRight, 2 = Idle
    CurrentBehavior = static_cast<EEnemyBehavior>(RandomBehavior);
}

void ABotCharacter::Damage_Implementation()
{

}

void ABotCharacter::Death_Implementation()
{
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // �����������ײ
    SetActorEnableCollision(false);  // ȷ�� Actor ����������ײ
    DisableInput(nullptr);
    if (DeathAnimation) // ȷ�� DeathAnimation ����Ч�� UAnimationAsset*
    {
        GetMesh()->PlayAnimation(DeathAnimation, false);// ���Ŷ�����false ��ʾ��ѭ��
        float AnimationDuration = DeathAnimation->GetMaxCurrentTime(); 
        float DestroyDelay = AnimationDuration + 1.0f; // ����������Ϻ� 1 ������

        GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &ABotCharacter::DestroyBot, DestroyDelay, false);
    }
    else
    {
        // ���û�ж�����ֱ������
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


