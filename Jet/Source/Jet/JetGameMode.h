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
	void CountdownTimer(); // ��ʱ������ʱ
	void OnSpikePlanted();
	void OnBotKilled(); // ��ɫ��ɱ Bot ʱ����
	void EndGame(bool bPlayerWon); // ������Ϸ
};



