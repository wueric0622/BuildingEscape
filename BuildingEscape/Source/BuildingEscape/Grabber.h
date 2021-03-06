// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	APlayerController* Player;
	UPROPERTY(EditAnywhere)
		float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	///拿取被射線打到的物件
	void Grab();
	///放開被射線打到的物件
	void Release();
	///找尋並定義PhysicsHandleComponent
	void FindPhysicsHandleComponent();
	///找尋並設置按鍵事件
	void SetupInputComponent();
	///獲得Raytrace結果
	FHitResult GetPhysicsInReach() const;
	///獲得射線的末端
	FVector GetReachLineEnd() const;
	///獲得射線的開始點
	FVector GetReachLineStart() const;
};
