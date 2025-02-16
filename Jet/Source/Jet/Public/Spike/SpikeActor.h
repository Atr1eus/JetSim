// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include <Components/BoxComponent.h>
#include "SpikeActor.generated.h"


UCLASS()
class JET_API ASpikeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpikeActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* SpikeSkeletalMesh;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimSequence* InstallAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    USoundBase* PlantSound;

    FTimerHandle InstallTimerHandle;

    // 炸弹是否已经安装
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    bool bIsSpikeInstalled;

    // 安装时需要的时间
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    float InstallTime;

    // 爆炸时间
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    float ExplosionTime;

    // 拆卸时间
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    float DefuseTime;

    // 倒计时
    float CountdownTimer;

    // 安装进度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    float InstallProgress;

    // 爆炸函数
    UFUNCTION(BlueprintCallable, Category = "Spike")
    void Explode();

    // 触发安装的函数
    UFUNCTION(BlueprintCallable, Category = "Spike")
    void InstallSpike();

    //拆解
    UFUNCTION(BlueprintCallable, Category = "Spike")
    void DefuseSpike();

    UFUNCTION(BlueprintCallable, Category = "Spike")
    void OnInstallComplete();

    // 计时器更新函数
    void UpdateTimer(float DeltaTime);
	
};
