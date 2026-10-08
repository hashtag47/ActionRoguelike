// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGamePlayInterface.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	// eye forward 10m
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.f);

	//FHitResult Hit;
	// find the first thing that blocks the line trace - ray cast
	// But this is like pixel scale aiming
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	// SweepMultiByObjectType is used to detect multiple objects along a path, rather than just the first one.
	// It can be useful for detecting all potential interactable objects in a given area,
	// especially if they are overlapping or close together.
	// And it's sphere shaped detector, so it's more forgiving than a line trace.
	// Quaternion: FQuat::Identity means no rotation, so the sphere will be aligned with the world axes.
	// Could be sphere, box, etc...
	TArray<FHitResult> Hits;

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius); // Set the shape to a sphere with a radius of 30 units.

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit: Hits)
	{
		AActor* HitActor = Hit.GetActor();
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);

		if (HitActor) {
			// USGamePlayInterface is the reflection type used for checks like this one. 
			// ISGamePlayInterface holds the actual functions.
			if (HitActor->Implements<USGamePlayInterface>())
			{
				// This cast in Unreal Engine is a safe way to convert a UObject pointer to a specific type.
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);
				// We only want to interact with the first valid actor we find,
				// not all the actors that the sweep hits.
				break;
			}
		}
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}