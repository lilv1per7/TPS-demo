// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"//静态工具类
#include "BehaviorTree/BlackboardComponent.h"//黑板类 SetValueAsVector() ClearValue()

bool AShooterAIController::IsDead() const
{
    // 1. 获取当前控制器控制的 Pawn，并尝试转换为 ShooterCharacter 类型
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    
    // 2. 检查转换是否成功（即这个 Pawn 真的是 ShooterCharacter 或其子类）
    if (ControlledCharacter != nullptr)
    {
        // 3. 调用 ShooterCharacter 的 IsDead() 函数，获取真实死亡状态
        return ControlledCharacter->IsDead();
    }

    // 4. 如果转换失败（Pawn 不是 ShooterCharacter），默认返回 true（已死亡）
    // 这是一种安全设计：如果控制器没控制到有效角色，就当它“死了”
    return true;
}


void AShooterAIController::BeginPlay(){
    Super::BeginPlay();
    //运行行为树
    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);

        APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());//将AI初始位置存入黑板
        //GetBlackboardComponent()是AAIController类的非静态函数，此处通过this->隐形调用父类函数
    }

    
}

void AShooterAIController::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    /*
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);  //找到玩家控制的角色
    //如果AI视线扫描的玩家角色 
    if(LineOfSightTo(PlayerPawn)){
        //SetFocus(PlayerPawn);  //AI在视觉上和逻辑上锁定目标。
        //MoveToActor(PlayerPawn, AcceptanceRadius);  //设置AI跟随。（跟随目标与最终距离的半径cm）
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation()); //将玩家位置存入黑板 set
        //行为树执行Chase
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());//将玩家最后位置存入黑板
    }else{
        //ClearFocus(EAIFocusPriority::Gameplay);  //清空AI视觉锁定
        //StopMovement();  //停止AI移动
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation")); //清空PlayerLocation 行为树上执行Investigate
    }
   */
}
