// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrelActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SMagicProjectile.h"

// Sets default values
ASExplosiveBarrelActor::ASExplosiveBarrelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	MeshComp->SetNotifyRigidBodyCollision(true);
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
	ForceComp->SetAutoActivate(false);
	ForceComp->Radius = 750.f;
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->bImpulseVelChange = true;
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ASExplosiveBarrelActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Bind here rather than in the constructor,
	// constructor bindings can be lost on Blueprint
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrelActor::OnActorHit);
}

void ASExplosiveBarrelActor::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!Cast<ASMagicProjectile>(OtherActor))
	{
		return;
	}
	ForceComp->FireImpulse();

	OtherActor->Destroy();
}

// Called when the game starts or when spawned
void ASExplosiveBarrelActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASExplosiveBarrelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

