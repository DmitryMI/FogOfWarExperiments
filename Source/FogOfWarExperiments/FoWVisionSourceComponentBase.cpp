// Fill out your copyright notice in the Description page of Project Settings.


#include "FoWVisionSourceComponentBase.h"
#include "FoWControllerBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFoWVisionSourceComponentBase::UFoWVisionSourceComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFoWVisionSourceComponentBase::BeginPlay()
{
	Super::BeginPlay();

	auto controller = GetFoWController();
	if (controller != nullptr)
	{
		controller->RegisterVisionSource(this);
	}
}

void UFoWVisionSourceComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UFoWVisionSourceComponentBase::GetVisionRange()
{
	return visionRange;
}


void UFoWVisionSourceComponentBase::SetVisionRange(float value)
{
	visionRange = value;
}
