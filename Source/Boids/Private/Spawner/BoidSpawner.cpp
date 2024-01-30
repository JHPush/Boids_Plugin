// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/BoidSpawner.h"
#include "Fishes/CA_FishBase.h"
#include "DataSource/BoidsData_Fish.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoidSpawner::ABoidSpawner()
{
	rootComp = CreateDefaultSubobject<USceneComponent>(FName("root"));
	RootComponent = rootComp;
	static ConstructorHelpers::FObjectFinder<UDataTable> FishDTObj(TEXT("/Boids/DT_FishDatas.DT_FishDatas"));
	if (FishDTObj.Succeeded()) {
		//UE_LOG(BOIDS_LOG, Log, TEXT("Fish DT Found !!"));
		DT_FishDatas = FishDTObj.Object;
	}
}

// Called when the game starts or when spawned
void ABoidSpawner::BeginPlay()
{
	Super::BeginPlay();

	StartSpawn();
}

void ABoidSpawner::StartSpawn() {
	if (!HasAuthority()) return;
	SpawnedFishDT = GetDTDatas();
	
	for(int i = 0; i< spawnedNum; i++){
		FTransform spawnTrans(FRotator(FMath::RandRange(0.f, 180.f)), 
		FVector(this->GetActorLocation().X + FMath::RandRange(0.f, spawnedDistance),
			this->GetActorLocation().Y + FMath::RandRange(0.f, spawnedDistance),
			this->GetActorLocation().Z + FMath::RandRange(0.f, spawnedDistance)
			)
		);
		auto spawnedFish = Cast<ACA_FishBase>(
			UGameplayStatics::BeginDeferredActorSpawnFromClass(
			this,
			SpawnedFishDT.FishClass.Get(),
			spawnTrans,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
			this
			)
		);
		if(spawnedFish != nullptr){
			spawnedFish->FishSetup(SpawnedFishDT, isEnemy);
			UGameplayStatics::FinishSpawningActor(spawnedFish, spawnTrans);
		}
	}

}

FBoidsFishDT ABoidSpawner::GetDTDatas() {
	if (!DT_FishDatas) {
		UE_LOG(BOIDS_LOG, Error, TEXT("DT_BoidsFish is EmptyPointer in BoidSpawner_GetDTDatas()"));
		return FBoidsFishDT();
	}

	FName rowName = FName(*GetEFishNameToDisplayName(SpawnedFishName));
	
	auto rowSt = DT_FishDatas->FindRow<FBoidsFishDT>(rowName, "");
	return *rowSt;
}

FString ABoidSpawner::GetEFishNameToDisplayName(EFishName name) const {
	const UEnum* fishEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EFishName"), true);
	
	if (!fishEnum) return FString("InValid");

	return fishEnum->GetDisplayNameTextByIndex((int)name).ToString();
}