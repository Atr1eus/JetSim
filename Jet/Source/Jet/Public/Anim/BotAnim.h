// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BotAnim.generated.h"

class ABotCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class JET_API UBotAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ABotCharacter> BotCharacter;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> BotCharacterMovement;

	// 动画状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsShooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsWalking;

	//更新动画参数
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateAnimationParameters(float NewSpeed, bool bNewIsShooting);
	
	
};
