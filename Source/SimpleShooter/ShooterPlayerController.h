// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void GameHasEnded(class AActor *EndGameFocus = nullptr, bool bIsWinner = false) override;

private:
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5;// 重启延迟时间，默认 5 秒

	FTimerHandle RestartTimer;// UE 定时器句柄，用于管理定时事件（无默认值，运行时初始化）

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass; //用于储存UserWidget或其子类的类引用

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;

    UUserWidget *HUD;

};
