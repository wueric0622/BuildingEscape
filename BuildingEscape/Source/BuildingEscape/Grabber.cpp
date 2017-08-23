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

	///��M�éw�qPhysicsHandleComponent
	FindPhysicsHandleComponent();
	///��M�ó]�m����ƥ�
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

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
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

///�����Q�g�u���쪺����
void UGrabber::Grab()
{
	if (!PhysicsHandle) { return; }
	FHitResult HitResult = GetPhysicsInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	}

}
///��}�Q�g�u���쪺����
void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}
///��oRaytrace���G
FHitResult UGrabber::GetPhysicsInReach() const
{
	FHitResult LineTraceHit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT LineTraceHit, 
		GetReachLineStart(), 
		GetReachLineEnd(), 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParameters
	);
	return LineTraceHit;
}
///��o�g�u������
FVector UGrabber::GetReachLineEnd() const
{
	FVector ViewLocation;
	FRotator ViewRotator;
	Player->GetPlayerViewPoint(OUT ViewLocation, OUT ViewRotator);

	FVector LineTraceDirection = ViewRotator.Vector() * Reach;
	FVector LineTraceEnd = ViewLocation + LineTraceDirection;
	return LineTraceEnd;
}
///��o�g�u���}�l�I
FVector UGrabber::GetReachLineStart() const
{
	FVector ViewLocation;
	FRotator ViewRotator;
	Player->GetPlayerViewPoint(OUT ViewLocation, OUT ViewRotator);

	return ViewLocation;
}

