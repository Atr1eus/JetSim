// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Component/HealthComponent.h"
#include "Interface/HealthInterface.h"


#include "BotCharacter.generated.h"

class UAnimSequence; // ����AnimSequence��

// ���������Ϊ����
UENUM(BlueprintType)
enum class EEnemyBehavior : uint8
{
	MoveTowardsPlayer UMETA(DisplayName = "Move Towards Player"),
	PatrolLeftRight UMETA(DisplayName = "Patrol Left/Right"),
	Idle UMETA(DisplayName = "Idle")
};

UCLASS()
class JET_API ABotCharacter : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABotCharacter();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AJetProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ��ȡ�ٶȲ����¶�������
	void UpdateMovementAnimation();

	//Actions�����ж�
	void MoveTowardsPlayer();
	void PatrolLeftRight();
	void IdleBehavior();

	void SelectRandomBehavior();

	void AttackPlayer();
	void SwitchBehavior();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimSequence* DeathAnimation;

	FTimerHandle DeathTimerHandle;

	class AJetCharacter* PlayerCharacter;

	EEnemyBehavior CurrentBehavior;  // ��ǰ��Ϊ
	FVector PatrolStartLocation;
	FVector PatrolEndLocation;
	bool bMovingRight;

	FTimerHandle BehaviorTimerHandle;
	FTimerHandle AttackTimerHandle;

	// ��������
	float PatrolDistance = 100.0f;  // Ѳ�߷�Χ
	float AttackRange = 100.0f;     // ������Χ
	float BehaviorChangeInterval = 3.0f;  // ��Ϊ�л����
	float AttackInterval = 2.0f;    // �������
	bool bIsShooting;

	// ��� HealthComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealthComponent* HealthComponent;

	// ʵ�� IHealthInterface �ӿڵ� Damage �� Death ����
	void Damage_Implementation() override;
	void Death_Implementation() override;
	
	void DestroyBot();
};
