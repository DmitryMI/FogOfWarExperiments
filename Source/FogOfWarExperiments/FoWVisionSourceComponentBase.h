// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoWComponentBase.h"
#include "FoWVisionSourceComponentBase.generated.h"

class AFoWControllerBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOGOFWAREXPERIMENTS_API UFoWVisionSourceComponentBase : public UFoWComponentBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float visionRange;

public:	
	// Sets default values for this component's properties
	UFoWVisionSourceComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		float GetVisionRange();

	UFUNCTION(BlueprintCallable)
		void SetVisionRange(float value);
};
