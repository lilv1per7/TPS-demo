// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;//游戏开始，初始化满血生命值

	//GetWorld()：获取当前 Actor 所在的世界对象指针。
	//用 GunClass（子类类型）生成子类实例，返回 AGun*（父类指针）。
	//赋值给 Gun：子类实例向上转型为父类指针（安全、多态支持）。
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);	
	//根据名字隐藏骨骼中自带的gun
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	//在手上插入生成的gun
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);//将gun的owner设置为shootercharacter
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input用于将功能绑定到输入
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    //绑定输入轴名称 获取输入值AxisValue传给成员函数并调用
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft"), this, &AShooterCharacter::MoveLeft);
	PlayerInputComponent->BindAxis(TEXT("LookLeft"), this, &AShooterCharacter::LookLeft);
	//绑定输入按键 为函数获取输入值AxisValue 父类ACharacter已经定义了jump()
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

//重写TakeDamage
float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) 
{
    // A. 调用父类逻辑并获取“最终修正伤害”。接收伤害。EventInstigator, DamageCauser在gun.cpp中发出伤害时已经被赋值。
    float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    // B. 钳制伤害值：确保扣除的血量不会超过当前剩余血量
    DamageToApply = FMath::Min(Health, DamageToApply);
    // C. 执行减血逻辑
    Health -= DamageToApply;
    // D. 打印日志，方便在输出日志窗口（Output Log）查看剩余血量
    UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);
    // E. 返回实际扣除的伤害值

	if (IsDead())
	{
        //用gamemode指针调用PawnKilled(this)，表示被击杀了
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this); //有角色阵亡则调用这个函数
		}

		DetachFromControllerPendingDestroy(); //剥离控制器权限
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); //使胶囊失去碰撞

	}

    return DamageToApply;
}

//实现前后移动
void AShooterCharacter::MoveForward(float AxisValue){
     AddMovementInput(GetActorForwardVector() * AxisValue);
}
//实现上下看
void AShooterCharacter::LookUp(float AxisValue){
	AddControllerPitchInput(AxisValue);
}
//实现左右移动
void AShooterCharacter::MoveLeft(float AxisValue){
	AddMovementInput(GetActorRightVector() * AxisValue);
}
//实现左右看
void AShooterCharacter::LookLeft(float AxisValue){
	AddControllerYawInput(AxisValue);
}
//实现射击
void AShooterCharacter::Shoot(){
	Gun->PullTrigger();
}

bool AShooterCharacter::IsDead() const{
    return Health == 0 ? true:false;
}

float AShooterCharacter::GetHealthPercent() const{
    return Health / MaxHealth;
}