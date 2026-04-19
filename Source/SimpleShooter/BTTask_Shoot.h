// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_Shoot : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTTask_Shoot();//构造函数

protected:
    //重写父类UBTTask的ExecuteTask函数 该函数能够在BT运行到此节点时进入并执行
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
};
