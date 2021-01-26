// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it but no pressure plate set!"), *GetOwner()->GetName());
	}

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);		
	}
	else{
		CloseDoor(DeltaTime);
	}		
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
		CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * 1.f);
		FRotator DoorRotation = GetOwner()->GetActorRotation();
		DoorRotation.Yaw = CurrentYaw;
		GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime) //	My Own CloseDoor() logic
{
		CurrentYaw = GetOwner()->GetActorRotation().Yaw;
		InitialYaw -= CurrentYaw;
		InitialYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * 1.f);
		FRotator DoorRotation = GetOwner()->GetActorRotation();
		DoorRotation.Yaw = InitialYaw;
		GetOwner()->SetActorRotation(DoorRotation);
}