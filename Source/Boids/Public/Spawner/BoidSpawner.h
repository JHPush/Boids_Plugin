// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidSpawner.generated.h"

UCLASS()
class BOIDS_API ABoidSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoidSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent *rootComp;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFishName SpawnedFishName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int spawnedNum = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float spawnedDistance = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isEnemy;

private:
	UPROPERTY()
		class UDataTable* DT_FishDatas;

	UPROPERTY()
		FBoidsFishDT SpawnedFishDT;

public:
	UFUNCTION(BlueprintCallable)
		void StartSpawn();

private:
	UFUNCTION()
		FBoidsFishDT GetDTDatas();

	UFUNCTION()
		FString GetEFishNameToDisplayName(EFishName name) const;
};
