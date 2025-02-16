// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <InputAction.h>
#include "Spike/SpikeActor.h"
#include "SpikeInstComponent.generated.h"


class AJetCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JET_API USpikeInstComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpikeInstComponent();

	// Spike Actor 类
	UPROPERTY(EditDefaultsOnly, Category = "Spike")
	TSubclassOf<class ASpikeActor> SpikeClass;

	AJetCharacter* Character;

	ASpikeActor* Spike;

	USkeletalMeshComponent* SkeletalMesh;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InstComp")
	bool bIsHoldingKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InstComp")
	bool bSpikeInstalled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InstComp")
	float KeyPressDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InstComp")
	float MaxKeyPressDuration;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PlantAction;

	// 安装炸弹
	void StartInstallSpike();
	void StopInstallSpike();

	UFUNCTION(BlueprintCallable, Category = "Spike")
	void Install();
	
	FVector GetGroundNormal(FVector SpawnLocation);
	// 辅助函数：计算生成位置

	FVector CalculateSpawnLocation();

	// 辅助函数：检测地面
	FVector GetGroundHeight(FVector SpawnLocation);
};
