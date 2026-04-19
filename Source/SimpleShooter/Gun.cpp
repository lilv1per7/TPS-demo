// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));//创建场景组件root
	SetRootComponent(Root);//将root设置为根组件
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));//创建Mesh网格
	Mesh->SetupAttachment(Root);//将mesh连接到根目录
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{   
	//虚幻引擎提供的一个静态工具函数，用于创建并附加一个粒子特效。它的参数决定了特效“是什么”、“在哪里”以及“跟着谁”。
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
    //触发开枪的声音效果
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
   
	FHitResult Hit;//是一个结构体，用来存放射线撞击到物体后的所有信息

	FVector ShotDirection; //开枪方向
	bool bSuccess = GunTrace(Hit, ShotDirection); //射线检测 并定义Hit与ShotDirection

	if (bSuccess)
	{
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		//给击中的地方加上粒子效果
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		//给击中的地方加上声音效果
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		
		//找到击中的对象
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{   //send damage
			//构造函数初始化FPointDamageEvent类型的实例DamageEvent
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController *OwnerController = GetOwnerController();//调用方法找到玩家控制器
            //通知对方：“你被打了，请根据我给你的信息扣血”。 send damage！
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			//Actor是父类，内存指向：在游戏世界里，那个被射线的物体是由AShooterCharacter类创建出来的实例 因此这里调用的是character中重写的TakeDamage
		}
	}

}
//实现射线检测
bool AGun::GunTrace(FHitResult &Hit, FVector& ShotDirection) 
{   
	//定位控制器
	AController *OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

    //通过控制器调用GetPlayerViewPoint获取玩家视口的location与rotation
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	//让粒子效果朝着射击来源的方向喷射,因此需要取反，且HitResult里面没有rotation信息
	ShotDirection = -Rotation.Vector();
    //计算射线没有碰到任何物体的最终位置 Rotation.Vector()将旋转量转换为一个长度为 1 的方向向量（单位向量）。
	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	//让射击检查轨道忽略掉枪本身以及使用枪的角色
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	//执行射线检测 bSuccess: 如果射线的路径上挡了东西，返回 true；如果一路上什么都没碰到，返回 false
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

//找到玩家控制器
AController* AGun::GetOwnerController() const
{
	//定位持枪人 ，只有 APawn（或其子类 ACharacter）才能拥有控制器并移动。这里我们尝试把ACtor“翻译”成更具体的 Pawn。
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController(); //返回持枪人的控制器
}
