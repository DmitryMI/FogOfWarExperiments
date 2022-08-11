// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FoWComponentBase.generated.h"

class AFoWControllerBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOGOFWAREXPERIMENTS_API UFoWComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFoWComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AFoWControllerBase* GetFoWController();

	virtual void EndPlay(const EEndPlayReason::Type reason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	
};
