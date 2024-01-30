// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CA_FishBase.generated.h"

UCLASS()
class BOIDS_API ACA_FishBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACA_FishBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
		UWorld* world;

public:
	UFUNCTION()
	void FishSetup(FBoidsFishDT spec, bool isEnemy);

private:
	UPROPERTY(VisibleAnywhere,Category = "Boids")
	FBoidsFishDT mFishSpec;


private:
	FTimerHandle ChecNeighborTH;
	FTimerHandle CheckEnemyTH;
	FTimerHandle FindEgoTH;

private:
	UFUNCTION()
	void CheckNeighbor();
	UFUNCTION()
	void CheckEnemy();
	UFUNCTION()
	void CheckEgoVector();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool mIsEnemy;

private:
	float additionalSpeed;
	float speed;
protected:
	UPROPERTY(VisibleAnywhere, Category = "Boids", meta = (AllowPrivateAccess = "true"))
	TArray<ACA_FishBase*> Neighbors;
	UPROPERTY(VisibleAnywhere, Category = "Boids", meta = (AllowPrivateAccess = "true"))
	TArray<ACA_FishBase*> Enemies;

private:
UPROPERTY(VisibleAnywhere, Category = "Boids", meta = (AllowPrivateAccess = "true"))
	FVector conhensionVec;
	UPROPERTY(VisibleAnywhere, Category = "Boids", meta = (AllowPrivateAccess = "true"))
	FVector alignmentVec;
	UPROPERTY(VisibleAnywhere, Category = "Boids", meta = (AllowPrivateAccess = "true"))
	FVector separationVec;
	UPROPERTY(VisibleAnywhere, Category = "Boids", meta = (AllowPrivateAccess = "true"))
	FVector boundVec;
	UPROPERTY(VisibleAnywhere, Category = "Boids", meta = (AllowPrivateAccess = "true"))
	FVector obstacleVec;
	UPROPERTY(VisibleAnywhere, Category = "Boids", meta = (AllowPrivateAccess = "true"))
	FVector enemyAvoidVec;
	UPROPERTY(VisibleAnywhere, Category = "Boids", meta = (AllowPrivateAccess = "true"))
	FVector targetVec;
	UPROPERTY(VisibleAnywhere, Category = "Boids", meta = (AllowPrivateAccess = "true"))
	FVector egoVec;

private:
	UFUNCTION()
	FVector FindCohensionVector();
	UFUNCTION()
	FVector FindAlignmentVector();
	UFUNCTION()
	FVector FindSeparationVector();
	UFUNCTION()
	FVector FindBoundVector();
	UFUNCTION()
	FVector FindObstacleAvoidVector();
	UFUNCTION()
	FVector FindEnemyAvoidVector();

	
	UFUNCTION()
	void CalculateMovement(float deltaTime);
};
