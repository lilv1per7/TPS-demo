// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h" // UE 定时器管理头文件，用于 GetWorldTimerManager
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    HUD = CreateWidget(this, HUDClass); //定义HUD 通过控制器调用CreateWidget返回Widget指针
    if(HUD!=nullptr){
        HUD->AddToViewport();//添加准星到视口
    }
}

void AShooterPlayerController::GameHasEnded(class AActor *EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport(); //清除HUD的视口UI

    if (bIsWinner)
    {
        UUserWidget *WinScreen = CreateWidget(this, WinScreenClass);
        if (WinScreen != nullptr)
        {
            WinScreen->AddToViewport(); //将胜利结算弹到玩家视口
        }
    }
    else
    {
        UUserWidget *LoseScreen = CreateWidget(this, LoseScreenClass);//：动态创建一个用户界面 Widget实例指针
        if (LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();//将这个 Widget 添加到当前玩家的视口
        }
    }

   
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
    // 设置定时器
    // GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
    //在 RestartDelay 秒后调用 RestartLevel 函数（重启关卡）。
}
