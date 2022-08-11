// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoWComponentBase.h"
#include "Components/ActorComponent.h"
#include "FoWVisionBlockerComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOGOFWAREXPERIMENTS_API UFoWVisionBlockerComponentBase : public UFoWComponentBase
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	float visionBlockRadius = 100.0f;

public:	
	// Sets default values for this component's properties
	UFoWVisionBlockerComponentBase();

	float GetVisionBlockRadius();
	virtual void SetVisionBlockRadius(float value);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
