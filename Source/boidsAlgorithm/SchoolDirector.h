// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SchoolDirector.generated.h"

USTRUCT()
struct FBoidData
{
	GENERATED_BODY()
public:
	int32 id;
	FVector location;
	FRotator rotation;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 maxUnits = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float maxVelocity = 2500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float maxSteerF = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float fCohesion = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float distCohesion = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float fSeparation = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float distSeparation = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float fAlignment = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float distAlignment = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float collisionRadius = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float fCollision = 1500.f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite) float


	UPROPERTY() TArray<FBoidData> boidData;
	UPROPERTY() TArray<class ACollisionActor*> collisions;


};
