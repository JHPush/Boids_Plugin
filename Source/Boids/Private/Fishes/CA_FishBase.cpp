// Fill out your copyright notice in the Description page of Project Settings.

#include "Fishes/CA_FishBase.h"
#include "DataSource/BoidsData_Fish.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACA_FishBase::ACA_FishBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACA_FishBase::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
		return;
	egoVec = FVector(0);
	targetVec = FVector(0);
	world = GetWorld();
	
	if(!mIsEnemy){
		world->GetTimerManager().SetTimer(ChecNeighborTH, this, &ACA_FishBase::CheckNeighbor, mFishSpec.FishSpec.SenseNeighborTimeRepeat, true, 0.1f);
		world->GetTimerManager().SetTimer(CheckEnemyTH, this, &ACA_FishBase::CheckEnemy, mFishSpec.FishSpec.SenseEnemyTimeRepeat, true, 0.1f);
	}
	world->GetTimerManager().SetTimer(FindEgoTH, this, &ACA_FishBase::CheckEgoVector, mFishSpec.FishSpec.UpdateEgoTime, true, 0.1f);
}

// Called every frame
void ACA_FishBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
		return;
	if(additionalSpeed>0)
		additionalSpeed -= DeltaTime;

	if(!mIsEnemy){
		conhensionVec = FindCohensionVector();
		alignmentVec = FindAlignmentVector();
		separationVec = FindSeparationVector();
		enemyAvoidVec = FindEnemyAvoidVector();
		obstacleVec = FindObstacleAvoidVector();
	}

	boundVec = FindBoundVector();
	CalculateMovement(DeltaTime);
}

void ACA_FishBase::FishSetup(FBoidsFishDT spec, bool isEnemy){
	mFishSpec = spec;
	mIsEnemy = isEnemy;
}


void ACA_FishBase::CheckNeighbor()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> Types;
	TEnumAsByte<EObjectTypeQuery> FishType;
	TArray<AActor *, FDefaultAllocator> IgnoreActors;

	IgnoreActors.Empty();
	Neighbors.Empty();
	Enemies.Empty();

	FishType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	
	Types.Add(FishType);

	TArray<AActor *, FDefaultAllocator> ResultPawns;

	UKismetSystemLibrary::SphereOverlapActors(
		world,
		GetActorLocation(),
		mFishSpec.FishSpec.NeighborDistance,
		Types,
		ACA_FishBase::StaticClass(),
		IgnoreActors,
		ResultPawns
	);

	if (ResultPawns.IsEmpty())
		return;
	
	for (int i = 0; i < ResultPawns.Num(); i++)
	{
		if (mFishSpec.FishSpec.MaxNeighborNum < i)
			return;
		if (ResultPawns[i] == this)
			continue;
		if (!mIsEnemy)
			Neighbors.Add(Cast<ACA_FishBase>(ResultPawns[i]));
		else
			Enemies.Add(Cast<ACA_FishBase>(ResultPawns[i]));
	}
}

void ACA_FishBase::CheckEnemy(){
	TArray<TEnumAsByte<EObjectTypeQuery>> Types;
	TEnumAsByte<EObjectTypeQuery> FishType;
	TArray<AActor *, FDefaultAllocator> IgnoreActors;

	IgnoreActors.Empty();
	Enemies.Empty();

	if (!mIsEnemy)
		FishType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	
	Types.Add(FishType);

	TArray<AActor *, FDefaultAllocator> ResultPawns;

	UKismetSystemLibrary::SphereOverlapActors(
		world,
		GetActorLocation(),
		mFishSpec.FishSpec.EnemySenseDistance,
		Types,
		ACA_FishBase::StaticClass(),
		IgnoreActors,
		ResultPawns
	);

	if (ResultPawns.IsEmpty())
		return;
	
	for (int i = 0; i < ResultPawns.Num(); i++)
	{
		if (ResultPawns[i] == this)
			continue;
		Enemies.Add(Cast<ACA_FishBase>(ResultPawns[i]));
	}
}
void ACA_FishBase::CheckEgoVector(){
	speed = FMath::RandRange(mFishSpec.FishSpec.MinMaxSpeed.X, mFishSpec.FishSpec.MinMaxSpeed.Y);

	egoVec = FMath::Lerp<FVector>(egoVec, UKismetMathLibrary::RandomPointInBoundingBox(FVector(0), FVector(.5f,.5f,.5f)),.1f);
	egoVec *= mFishSpec.BoidWeight.EgoWeight;
}

FVector ACA_FishBase::FindCohensionVector()
{
	if(Neighbors.IsEmpty()) return FVector(0);

	FVector mCohension = FVector(0);
	for(auto fish : Neighbors)
		mCohension += fish->GetActorLocation();

	mCohension = (mCohension / Neighbors.Num())- this->GetActorLocation() ;
	mCohension.Normalize();
	
	return mCohension * mFishSpec.BoidWeight.CohensionWeight;
}
FVector ACA_FishBase::FindAlignmentVector()
{
	if(Neighbors.IsEmpty()) return this->GetActorForwardVector();

	FVector mAlignment = FVector(0);

	for(auto fish : Neighbors)
		mAlignment += fish->GetActorForwardVector();

	mAlignment = (mAlignment / Neighbors.Num());
	mAlignment.Normalize();

	return mAlignment * mFishSpec.BoidWeight.AlignmentWeight;

}
FVector ACA_FishBase::FindSeparationVector()
{
	if(Neighbors.IsEmpty()) return FVector(0);
	FVector mSeparation = FVector(0);

	for(auto fish : Neighbors)
		mSeparation += (this->GetActorLocation() - fish->GetActorLocation());
	
	mSeparation /= Neighbors.Num();
	mSeparation.Normalize();

	return mSeparation * mFishSpec.BoidWeight.SeparationWeight;
}
FVector ACA_FishBase::FindBoundVector()
{
	FVector mOffset = this->GetOwner()->GetActorLocation() - this->GetActorLocation();

	if(mFishSpec.FishSpec.BoundDistance < mOffset.Length()){
		mOffset.Normalize();

	return mOffset * mFishSpec.BoidWeight.BoundWeight;
	}
	return FVector(0);
	
}
FVector ACA_FishBase::FindObstacleAvoidVector()
{
	FVector mObstacle = FVector(0);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> Types;
	TEnumAsByte<EObjectTypeQuery> ObstacleType;
	TArray<AActor *, FDefaultAllocator> IgnoreActors;

	ObstacleType =  UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	Types.Add(ObstacleType);

	TArray<AActor *, FDefaultAllocator> ResultObs;
	
	UKismetSystemLibrary::SphereOverlapActors(
		world,
		GetActorLocation(),
		mFishSpec.FishSpec.ObstacleSenseDistance,
		Types,
		nullptr,
		IgnoreActors,
		ResultObs
	);
	if(ResultObs.IsEmpty()) return FVector(0);

	for(auto obstacle : ResultObs)
		mObstacle += (this->GetActorLocation() - obstacle->GetActorLocation());
	
	mObstacle /= ResultObs.Num();
	mObstacle.Normalize();

	return mObstacle * mFishSpec.BoidWeight.ObstacleWeight;

}
FVector ACA_FishBase::FindEnemyAvoidVector()
{
	if(Enemies.IsEmpty()){
		return FVector(0);
	}

	FVector mAvoid = FVector(0);
	additionalSpeed = mFishSpec.FishSpec.AdditionalSpeed;
	
	for(auto enemy : Enemies)
		mAvoid += (this->GetActorLocation() - enemy->GetActorLocation());
	
	mAvoid /= Enemies.Num();
	mAvoid.Normalize();
	return mAvoid;
}

void ACA_FishBase::CalculateMovement(float deltaTime)
{
	if (!mIsEnemy)
		targetVec = (
			conhensionVec +
			alignmentVec +
			separationVec+ 
			enemyAvoidVec +
			boundVec +
			obstacleVec +
			egoVec
			);
	else{
		targetVec = (boundVec +
			obstacleVec +
			egoVec
			);
		}

	targetVec = FMath::Lerp(this->GetActorForwardVector(), targetVec,deltaTime);
	targetVec.Normalize();


	if(targetVec.IsNearlyZero()) 
		targetVec = egoVec;
	FRotator rot = FMath::InterpEaseInOut(
		this->GetActorRotation(), 
		UKismetMathLibrary::FindLookAtRotation(this->GetActorForwardVector(), targetVec),
		deltaTime, 1.3f
		);
	
	this->SetActorRotation(rot);

	FVector vec = (((speed + additionalSpeed) * deltaTime)* targetVec) + this->GetActorLocation();
	this->SetActorLocation(vec);

}