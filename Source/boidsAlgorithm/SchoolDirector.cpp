// Fill out your copyright notice in the Description page of Project Settings.


#include "SchoolDirector.h"
#include "Components/SceneComponent.h"
#include "CollisionActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "drawdebughelpers.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
// Sets default values
ASchoolDirector::ASchoolDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	rootComp = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	SetRootComponent(rootComp);
}

// Called when the game starts or when spawned
void ASchoolDirector::BeginPlay()
{
	Super::BeginPlay();
	if (unitMesh == nullptr) return;
	ISMComp = NewObject<UInstancedStaticMeshComponent>(this);
	ISMComp->RegisterComponent();
	ISMComp->SetStaticMesh(unitMesh);
	ISMComp->SetFlags(RF_Transactional);
	AddInstanceComponent(ISMComp);
	boidData.SetNum(maxUnits);

	for (int32 i = 0; i < maxUnits; i++)
	{
		float randX = FMath::FRandRange(-areaExtent.X, areaExtent.X);
		float randY = FMath::FRandRange(-areaExtent.Y, areaExtent.Y);
		float randZ = FMath::FRandRange(-areaExtent.Z, areaExtent.Z);
		FVector spawnPosition(randX, randY, randZ);
		FRotator spawnRotation(FMath::FRandRange(0.f, 360.f), FMath::FRandRange(0.f, 360.f), FMath::FRandRange(0.f, 360.f));
		FVector scale3d(1.f, 1.f, 1.f);
		FTransform spawnTransform;
		spawnTransform.SetLocation(spawnPosition);
		spawnTransform.SetRotation(spawnRotation.Quaternion());
		spawnTransform.SetScale3D(scale3d);
		boidData[i].id = ISMComp->AddInstance(spawnTransform);
		boidData[i].location = spawnPosition;
		boidData[i].velocity = (spawnRotation.Vector() * maxVelocity).GetClampedToMaxSize(maxVelocity);
		//boidData[i].velocity.Z = 0.f;
		boidData[i].deltaV = FVector::ZeroVector;
	}

	//https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere
	float phi = PI_VAL * (3.f - FMath::Sqrt(5.f));
	for (int32 i = 0; i < NUM_COLLISION_VECTORS; i++) {
		float y = 1 - (i / float(NUM_COLLISION_VECTORS - 1)) * 2;
		float radius = FMath::Sqrt(1 - y * y);
		float theta = phi * i;
		float x = FMath::Cos(theta);
		float z = FMath::Sin(theta);
		spherePoints.Add(FVector(x, y, z));
	}
}

// Called every frame
void ASchoolDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!ISMComp)
	{
		return;
	}
	ParallelFor(maxUnits, [&](int32 i)
	{
		FVector sep, coh, agn;
		int32 sepN, cohN, agnN;
		sep = coh = agn = FVector::ZeroVector;
		sepN = cohN = agnN = 0;
		for (int32 j = 0; j < maxUnits; j++)
		{
			if (i ^ j)
			{
				float dstToTarget = FVector::Distance(boidData[i].location, boidData[j].location);
				if (dstToTarget <= distCohesion)
				{
					cohN ++;
					coh += boidData[j].location;
				}
				if (dstToTarget <= distSeparation)
				{
					sepN ++ ;
					sep += boidData[j].location - boidData[i].location;
				}
				if (dstToTarget <= distAlignment)
				{
					agnN ++ ;
					agn += boidData[j].velocity;
				}
			}
		}
		if (cohN)
		{
			coh /= cohN;
			coh -= boidData[i].location;
			coh.Normalize();
		}
		if (sepN)
		{
			sep /= sepN;
			sep *= -1.f;
			sep.Normalize();
		}
		if (agnN)
		{
			agn /= agnN;
			agn.Normalize();
		}
		//avoidance
		FVector avoid = FVector::ZeroVector;
		int32 avoidCnt = 0;
		for (const FVector point : spherePoints) {
			FVector t_vec = boidData[i].location + point;
			if (CollisionCheck(GetWorld(), boidData[i].location, boidData[i].location + point * distAvoidance)) {
				avoid += point;
				avoidCnt++;
			}
		}
		/*if (i == 0) {
			UE_LOG(LogTemp, Warning, TEXT("Avoid cnt: %d"), avoidCnt);
		}*/
		if (avoidCnt) {
			avoid /= avoidCnt;
			avoid *= -1.f;
			avoid.Normalize();
		}



		boidData[i].deltaV = (fCohesion * coh + fSeparation * sep + fAlignment * agn) + avoid * fAvoidance;
		boidData[i].deltaV = boidData[i].deltaV.GetClampedToMaxSize(maxSteerF);
		//boidData[i].deltaV.Z = 0.f;
	});
	ParallelFor(maxUnits, [&](int32 i)
	{
		boidData[i].velocity += (boidData[i].deltaV * DeltaTime);
		boidData[i].velocity = boidData[i].velocity.GetClampedToMaxSize(maxVelocity);
		//boidData[i].velocity.Z = 0.f;
		boidData[i].location = boidData[i].location + boidData[i].velocity * DeltaTime;
		
		if (boidData[i].location.X > areaExtent.X || boidData[i].location.X < -areaExtent.X)
		{
			boidData[i].velocity.X *= -1.f;
			//boidData[i].location.X *= -1.f;
		}
		if (boidData[i].location.Y >= areaExtent.Y || boidData[i].location.Y < -areaExtent.Y)
		{
			boidData[i].velocity.Y *= -1.f;
			//boidData[i].location.Y *= -1.f;
		}
		if (boidData[i].location.Z >= areaExtent.Z || boidData[i].location.Z < -areaExtent.Z)
		{
			boidData[i].velocity.Z *= -1.f;
			//boidData[i].location.Z *= -1.f;
		}
	});
	for(int32 i=0;i<maxUnits;i++)
	{
		FTransform transform;
		ISMComp->GetInstanceTransform(boidData[i].id, transform);
		transform.SetLocation(boidData[i].location);
		FVector vel = boidData[i].velocity;
		vel.Normalize();
		/*if (i == 0) {
			UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), vel.X, vel.Y, vel.Z);
			DrawDebugLine(GetWorld(), boidData[i].location, boidData[i].location + boidData[i].velocity * 1000.f,FColor::Red, false, 0.1f, 0, 4.f);
		}*/
		FQuat rQuat = (boidData[i].velocity.Rotation() + FRotator(0.f, -90.f, 0.f)).Quaternion();
		rQuat.Normalize();
		transform.SetRotation(rQuat);
		ISMComp->UpdateInstanceTransform(boidData[i].id, transform, false, (i == maxUnits - 1), false);
	}
}

bool ASchoolDirector::CollisionCheck(UWorld* world, FVector start, FVector end)
{
	if (!world)
		return false;
	FHitResult hit;
	//DrawDebugLine(world, start, end, FColor::Red, false, -1.f, 0, 1.f);
	world->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_GameTraceChannel1);
	return hit.bBlockingHit;
}