// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "UFNNoiseGenerator.h"
#include "VoxelWorld.generated.h"

UCLASS(Blueprintable)
	class DAWNSHALLOW_API AVoxelWorld : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVoxelWorld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GenerateWorld();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUFNNoiseGenerator* NoiseGenerator;

	UPROPERTY(BlueprintReadOnly)
	UProceduralMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	float VoxelSize = 100.0f;

	UPROPERTY(EditAnywhere)
	float Amplitude = 200.0f;

	UPROPERTY(EditAnywhere)
	bool FlatShading = false;
};
