// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"
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

	//UE_LOG(LogTemp, Warning, TEXT("Grabber reporting!"));

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics handle component found on %s!"), *GetOwner()->GetName());
	}
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
	
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input Component found %s"),*GetOwner()->GetName());
		InputComponent->BindAction("Fire", IE_Pressed, this, &UGrabber::Fire);
		InputComponent->BindAction("Fire", IE_Released, this, &UGrabber::Release1);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released!"));
}

void UGrabber::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fired a bullet!"));
}

void UGrabber::Release1()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Button Released!"));
}

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

	//Draw a line from player showing the reach

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewPointRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewpointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0,
		5.f
	);
	
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
	
	
}

