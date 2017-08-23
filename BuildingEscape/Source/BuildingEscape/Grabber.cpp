// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	Player = GetWorld()->GetFirstPlayerController();

	///找尋並定義PhysicsHandleComponent
	FindPhysicsHandleComponent();
	///找尋並設置按鍵事件
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s PhysicsHandleComponent is found"), *Player->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing PhysicsHandleComponent"), *Player->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s InputComponent is found"), *Player->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing InputComponent"), *Player->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	FVector ViewLocation;
	FRotator ViewRotator;
	Player->GetPlayerViewPoint(OUT ViewLocation, OUT ViewRotator);

	FVector LineTraceDirection = ViewRotator.Vector() * Reach;
	FVector LineTraceEnd = ViewLocation + LineTraceDirection;
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

///拿取被射線打到的物件
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	FHitResult HitResult = GetPhysicsInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}

}
///放開被射線打到的物件
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Release"));
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetPhysicsInReach() const
{
	FVector ViewLocation;
	FRotator ViewRotator;
	Player->GetPlayerViewPoint(OUT ViewLocation, OUT ViewRotator);

	FVector LineTraceDirection = ViewRotator.Vector() * Reach;
	FVector LineTraceEnd = ViewLocation + LineTraceDirection;
	DrawDebugLine(GetWorld(), ViewLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);

	FHitResult LineTraceHit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT LineTraceHit, ViewLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
	AActor* ActorHit = LineTraceHit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("You Hit : %s"), *ActorHit->GetName());
	}
	return LineTraceHit;
}

