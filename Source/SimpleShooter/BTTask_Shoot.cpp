// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h" //可以省略，因为AIController的父类AController中已经包括#include "BehaviorTree/BlackboardComponent.h" 
#include "AIController.h"  // 用于 GetPawn


UBTTask_Shoot::UBTTask_Shoot(){
    //给自定义节点命名
    NodeName = TEXT("Shoot");
}

//ExecuteTask是 BT 任务节点的核心入口点。当 BT 运行到这个节点时，会调用这个函数执行任务逻辑。
EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory); //重写函数都必须先调用父类函数，确保父类的基础行为先执行

    // 获取当前 AI 控制的 Pawn
    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;  // 没有 Pawn，失败
    }

    // 将 Pawn 转换为你的 ShooterCharacter 类
    AShooterCharacter* Character = Cast<AShooterCharacter>(AIPawn);
    if (!Character)
    {
        return EBTNodeResult::Failed;  // 没有 character，失败
    }
    Character->Shoot();

    return EBTNodeResult::Succeeded; //返回执行成功
}
