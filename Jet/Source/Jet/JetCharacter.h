// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "Public/Component/HealthComponent.h"
#include "Public/Interface/HealthInterface.h"
#include <Component/SpikeInstComponent.h>

#include "JetCharacter.generated.h"




class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

//生命值
class UHealthComponent;
class AJetHUD;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// 通用技能状态枚举
UENUM(BlueprintType)
enum class ESkillState : uint8
{
	Idle UMETA(DisplayName = "Idle"),            // 技能未使用
	Preparing UMETA(DisplayName = "Preparing"), // 技能准备中（如按住键不放）
	Active UMETA(DisplayName = "Active"),       // 技能激活中
	Cooldown UMETA(DisplayName = "Cooldown"),   // 技能冷却中
	Disabled UMETA(DisplayName = "Disabled")    // 技能被禁用（如受到特定效果）
};

UCLASS(config=Game)
class AJetCharacter : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	//HUD Menu
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EscAction;

	//Skills
	/** Updraft Input Action Q技能凌空*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UpdraftAction;

	/** Tailwind Input Action E技能逐风*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> TailwindAction;

public:
	AJetCharacter();

	AJetHUD* JetHUD;
	//-----------------------Drift 被动：漂移------------------------------
	// 默认重力比例
	float DefaultGravityScale;

	// 滑翔时的重力比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drift", meta = (AllowPrivateAccess = "true"))
	float DriftGravityScale;

	// 滑翔激活速度阈值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drift", meta = (AllowPrivateAccess = "true"))
	float DriftVelocityThreshold;

	// 是否处于滑翔状态
	bool bIsDrifting;

	// 是否正在跳跃（按下跳跃键）
	bool bIsJumpPressed;

	// 更新滑翔状态
	void UpdateDrift(float DeltaTime);

	//-------------------------------Tailwind-----------------------------------
	// 技能状态
	ESkillState TailwindState;

	// 冲刺方向计算
	FVector GetDashDirection() const;

	// Tailwind 技能参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	float TailwindPrepareTime; // 准备状态持续时间

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	float TailwindWindupTime; // 激活前摇时间

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	float DashRange; // 冲刺范围（15 米）

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	int32 KillsToRefresh; // 击杀/助攻数刷新技能所需次数

	int32 CurrentKills; // 当前击杀/助攻次数

	UFUNCTION()
	void IncreaseKillCount();

	//-------------------------Updraft------------------------------
	ESkillState UpdraftState;

	// 状态计时器
	FTimerHandle TailwindPrepareTimer;
	FTimerHandle TailwindWindupTimer;

	// 安装过程的状态
	// 玩家是否在安装区域内
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	bool bIsPlayerInZone;
	bool bIsInstallingSpike;
	UPROPERTY(BlueprintReadWrite, Category = "Game State")
	bool bIsSpikeInstalled;
	float InstallProgress;

protected:
	virtual void BeginPlay() override;

	//TICK
	virtual void Tick(float DeltaTime) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	//生命组件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UHealthComponent> HealthComponent;

	//安包组件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USpikeInstComponent> SpikeComponent;

	// Drift P 飘移
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	// Updarft Q 凌空 实现函数
	void PerformUpdraft(const FInputActionValue& Value);

	// Tailwind E 逐风
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void ActivateTailwind(); // 按下 E 的触发逻辑
	void ExecuteDash();      // 冲刺逻辑
	void CancelTailwind();   // 准备状态结束后取消技能
	void ResetTailwindCharge(); // 击杀/助攻后刷新技能
	

protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	TObjectPtr<USkeletalMeshComponent> GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	TObjectPtr<UCameraComponent> GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


	// 通过 IHealthInterface 继承
	void Damage_Implementation() override;

	void Death_Implementation() override;

};

