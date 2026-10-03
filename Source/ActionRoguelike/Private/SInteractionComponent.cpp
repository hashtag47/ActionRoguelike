// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGamePlayInterface.h"
#include "DrawDebugHelpers.h"

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

	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Green, false, 2.0f, 0, 2.0f);

	AActor* HitActor = Hit.GetActor();
	if (HitActor) {
		// USGamePlayInterface is the reflection type used for checks like this one. 
		// ISGamePlayInterface holds the actual functions.
		if(HitActor->Implements<USGamePlayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);
			ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);
		}
	}
}