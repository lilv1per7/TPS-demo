// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverWall.h"
#include "Math/UnrealMathUtility.h"//声明FMath::VInterpConstantTo

// Sets default values for this component's properties
UMoverWall::UMoverWall()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoverWall::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	
}


// Called every frame
void UMoverWall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation = GetOwner()->GetActorLocation();//获取现在的位置
	FVector TargetLocation = OriginalLocation + MoveOffset;
    float Speed = MoveOffset.Length() / MoveTime;
    //插值函数FMath::VInterpConstantTo，用来做线形匀速插值移动，返回最后的位置
    FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);//计算新位置
    GetOwner()->SetActorLocation(NewLocation);//更新actor到新位置
	

	// ...
}

