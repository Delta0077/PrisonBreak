// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input Component found %s"),*GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Input Component not found %s"),*GetOwner()->GetName());
	// }
	
	// InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	// if (InputComponent)
	// {
	// 	//UE_LOG(LogTemp, Warning, TEXT("Input Component found %s"),*GetOwner()->GetName());
	// 	InputComponent->BindAction("Fire", IE_Pressed, this, &UGrabber::Fire);
	// 	InputComponent->BindAction("Fire", IE_Released, this, &UGrabber::Release1);
	// }
	
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics handle component found on %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));

	FVector PlayerViewpointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
	OUT PlayerViewpointLocation, 
	OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// If we hit something then attach the physics handle.
	if (HitResult.GetActor())
	{
		// attaching physics handle.
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released!"));
	//TODO remove/release the physics handle.
	PhysicsHandle->ReleaseComponent();
}

// void UGrabber::Fire()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Fired a bullet!"));
// }

// void UGrabber::Release1()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Fire Button Released!"));
// }

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewpointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
	OUT PlayerViewpointLocation, 
	OUT PlayerViewPointRotation
	);	

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewPointRotation.Vector() * Reach;
	// If the physic handle is attach.
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the Object we are holding.
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
		
	}
	
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
	OUT PlayerViewpointLocation, 
	OUT PlayerViewPointRotation
	);	

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewPointRotation.Vector() * Reach;
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	AActor* ActorThatHit = Hit.GetActor();

	if (ActorThatHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace has hit : %s"), *(ActorThatHit->GetName()));
	}
	return Hit;
}