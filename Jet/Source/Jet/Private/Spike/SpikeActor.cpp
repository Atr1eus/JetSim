// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike/SpikeActor.h"
#include <Components/BoxComponent.h>


// Sets default values
ASpikeActor::ASpikeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 初始化碰撞组件
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;  // 设置根组件为碰撞组件

	// 初始化 Skeletal Mesh 组件
	SpikeSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SpikeSkeletalMesh"));
	SpikeSkeletalMesh->SetupAttachment(RootComponent);  // 将 Skeletal Mesh 附加到根组件

	// 配置碰撞
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 开启碰撞查询和物理模拟
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);  // 设置碰撞类型
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);  // 设置与所有通道的碰撞响应为 Block
	CollisionComponent->SetSimulatePhysics(false);
	//Initialization
	bIsSpikeInstalled = false;
	InstallTime = 4.0f; // 安装需要4秒
	ExplosionTime = 45.0f; // 爆炸时间为45秒
	CountdownTimer = ExplosionTime;
	InstallProgress = 0.0f;
	DefuseTime = 7.0f;

}

void ASpikeActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpikeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpikeActor::Explode()
{
	// 这里你可以添加你的爆炸逻辑（伤害、特效等）
	UE_LOG(LogTemp, Warning, TEXT("Spike Exploded!"));

	// 销毁炸弹
	Destroy();
}

void ASpikeActor::InstallSpike()
{
	if (!bIsSpikeInstalled)
	{
		// 启动安装进程
		InstallProgress = 0.0f;
		GetWorld()->GetTimerManager().SetTimer(InstallTimerHandle, this, &ASpikeActor::OnInstallComplete, InstallTime, false);
	}
}

void ASpikeActor::DefuseSpike()
{

}

void ASpikeActor::OnInstallComplete()
{
	bIsSpikeInstalled = true;
	CountdownTimer = ExplosionTime; // 重置倒计时
	GetWorld()->GetTimerManager().ClearTimer(InstallTimerHandle);
	// 执行安装完成后的动作
	UE_LOG(LogTemp, Warning, TEXT("Spike installation complete"));

}

void ASpikeActor::UpdateTimer(float DeltaTime)
{
	// 增加倒计时
	if (CountdownTimer > 0)
	{
		CountdownTimer -= DeltaTime;
	}
	else
	{
		Explode();
	}
}

