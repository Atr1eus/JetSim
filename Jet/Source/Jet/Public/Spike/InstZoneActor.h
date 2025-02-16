// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Jet/JetCharacter.h>

#include "InstZoneActor.generated.h"


UCLASS()
class JET_API AInstZoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInstZoneActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // 触发区域
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spike")
    class UBoxComponent* TriggerBox;

    // 玩家是否在区域内
    //bool bIsPlayerInZone;
    TObjectPtr<AJetCharacter> PlayerCharacter;

    // 检测玩家进入区域的函数
    UFUNCTION()
    void OnPlayerEnterZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    // 触发安装炸弹的函数
    UFUNCTION(BlueprintCallable, Category = "Spike")
    void TriggerInstallSpike();

    // 用于提示UI显示
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    bool bShowInstallPrompt;
	
};
