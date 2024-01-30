// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Math/Vector2D.h"
#include "BoidsData_Fish.generated.h"

UENUM(BlueprintType)
enum class EFishName : uint8 {
	BLOODPARROT UMETA(DisplayName = "BloodParrot"),
	DISCUS UMETA(DisplayName = "Discus"),
	TETRA UMETA(DisplayName = "Tetra"),
	FIREMOUTH UMETA(DisplayName = "FireMouth"),
	PEARLGOURAMI UMETA(DisplayName = "PearlGourami")
};


USTRUCT(Atomic, BlueprintType)
struct FBoidsWeight {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CohensionWeight;					
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AlignmentWeight;					
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SeparationWeight;					
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BoundWeight;						
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ObstacleWeight;					
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnemyAvoidenceWeight;				
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EgoWeight	;						
};

USTRUCT(Atomic, BlueprintType)
struct FFishSpec {
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D MinMaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AdditionalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxNeighborNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NeighborDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NeighborSenseFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SenseNeighborTimeRepeat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SenseEnemyTimeRepeat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnemySenseDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ObstacleSenseDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UpdateEgoTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BoundDistance;
};

USTRUCT(BlueprintType)
struct FBoidsFishDT : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBoidsWeight BoidWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FFishSpec FishSpec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ACA_FishBase> FishClass;					
};
