// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h" //黑板类 为了调用GetBlackboardComponent() ClearValue() GetSelectedBlackboardKey()

UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue(){
    //给自定义节点命名
    NodeName = TEXT("Clear BlackBoard Value");
}

//ExecuteTask是 BT 任务节点的核心入口点。当 BT 运行到这个节点时，会调用这个函数执行任务逻辑。
EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory); //重写函数都必须先调用父类函数，确保父类的基础行为先执行

    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());//清空Blackboard键

    return EBTNodeResult::Succeeded; //返回执行成功
}
