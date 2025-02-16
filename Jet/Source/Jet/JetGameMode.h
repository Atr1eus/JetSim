// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JetGameMode.generated.h"

class AJetHUD;
UCLASS(minimalapi)
class AJetGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AJetGameMode();

	FTimerHandle GameTimerHandle;
	int32 RemainingTime;
	bool bSpikePlanted;
	AJetHUD* MyHUD;

protected:
	virtual void BeginPlay() override;

public:
	void CountdownTimer(); // 计时器倒计时
	void OnSpikePlanted();
	void OnBotKilled(); // 角色击杀 Bot 时调用
	void EndGame(bool bPlayerWon); // 结束游戏
};



