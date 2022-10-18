// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SchoolDirector.generated.h"

#define NUM_COLLISION_VECTORS	40
#define PI_VAL					3.1415

USTRUCT()
struct FBoidData
{
	GENERATED_BODY()
public:
	int32 id;
	FVector location;
	FVector velocity;
	FVector deltaV;
};

UCLASS()
class BOIDSALGORITHM_API ASchoolDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASchoolDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) class USceneComponent* rootComp = nullptr;
	UPROPERTY() UInstancedStaticMeshComponent* ISMComp = nullptr;
	UPROPERTY(EditDefaultsOnly) UStaticMesh* unitMesh = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector areaExtent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 maxUnits = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float maxVelocity = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float maxSteerF = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float fCohesion = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float distCohesion = 1300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float fSeparation = 110.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float distSeparation = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float fAlignment = 80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float distAlignment = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float distAvoidance = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float fAvoidance = 1300.f;

	UPROPERTY() TArray<FBoidData> boidData;

	TArray<FVector> spherePoints;

	static bool CollisionCheck(UWorld* world, FVector loc, FVector end);

};
