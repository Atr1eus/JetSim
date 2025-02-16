// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/BotAnim.h"
#include "Enemy/BotCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBotAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BotCharacter = Cast<ABotCharacter>(TryGetPawnOwner());
	if (BotCharacter)
	{
		// ��ȡBotCharacter���ƶ����
		BotCharacterMovement = BotCharacter->GetCharacterMovement();
	}
}

void UBotAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BotCharacterMovement)
	{
		Speed = BotCharacterMovement->Velocity.Size();
	}
}

void UBotAnim::UpdateAnimationParameters(float NewSpeed, bool bNewIsShooting)
{
	Speed = NewSpeed;
	bIsShooting = bNewIsShooting;
}
