// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;//前向声明（forward declaration）。只用到类中的指针或引用优先使用前向声明
//告诉编译器AGun 是一个类，后面可能会用到它的指针或引用

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input将功能绑定到输入
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    //重写TakeDamage函数
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;
    
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void Shoot();

private:
    void MoveForward(float AxisValue);
    void LookUp(float AxisValue);
	void MoveLeft(float AxisValue);
	void LookLeft(float AxisValue);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;//用于存储AGun或其子类的类类型（Class Reference）。(不同类型的枪)

	UPROPERTY()
	AGun* Gun;//父类指针，可指向AGun或其子类实例，表示实际的枪

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;  //满血的生命值

	UPROPERTY(VisibleAnywhere)
	float Health;   //实时生命值

};