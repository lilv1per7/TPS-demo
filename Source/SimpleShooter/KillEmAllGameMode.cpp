// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"


void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled) 
{
    Super::PawnKilled(PawnKilled);
    //  通过cast是否能够成功将controller转换为PlayerControlle，检查是不是玩家阵亡
    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
        //PlayerController->GameHasEnded(nullptr, false);
        EndGame(false);
    }
    //检测AI是否全部阵亡
    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!Controller->IsDead())
        {
            return;
        }
    }
    
    EndGame(true);


}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) 
{
    for (AController* Controller : TActorRange<AController>(GetWorld())) //自动遍历世界里所有 Controller，即玩家控制器+AI控制器
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner; //判断是否为玩家是为为AI？再判断该角色是否胜利？
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner); //对玩家：调用 APlayerController的重写 → 显示失败/胜利 UI。
        //对AI：调用 AAIController 的版本（通常空），但至少通知它们“游戏停了”
    }
}
