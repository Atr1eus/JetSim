// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Component/HealthComponent.h"
#include "Interface/HealthInterface.h"


#include "BotCharacter.generated.h"

class UAnimSequence; // 引入AnimSequence类

// 定义敌人行为类型
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

	// 获取速度并更新动画参数
	void UpdateMovementAnimation();

	//Actions敌人行动
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

	EEnemyBehavior CurrentBehavior;  // 当前行为
	FVector PatrolStartLocation;
	FVector PatrolEndLocation;
	bool bMovingRight;

	FTimerHandle BehaviorTimerHandle;
	FTimerHandle AttackTimerHandle;

	// 其他变量
	float PatrolDistance = 100.0f;  // 巡逻范围
	float AttackRange = 100.0f;     // 攻击范围
	float BehaviorChangeInterval = 3.0f;  // 行为切换间隔
	float AttackInterval = 2.0f;    // 攻击间隔
	bool bIsShooting;

	// 添加 HealthComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealthComponent* HealthComponent;

	// 实现 IHealthInterface 接口的 Damage 和 Death 函数
	void Damage_Implementation() override;
	void Death_Implementation() override;
	
	void DestroyBot();
};
