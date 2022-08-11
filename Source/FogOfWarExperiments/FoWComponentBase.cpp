// Fill out your copyright notice in the Description page of Project Settings.

#include "FoWComponentBase.h"
#include "Kismet/GameplayStatics.h"
#include "FoWControllerBase.h"

// Sets default values for this component's properties
UFoWComponentBase::UFoWComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFoWComponentBase::BeginPlay()
{
	Super::BeginPlay();

	AFoWControllerBase* controller = GetFoWController();
	if (controller != nullptr)
	{
		controller->RegisterVisionComponent(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Fog Of War Controller not found!"));
	}
}

AFoWControllerBase* UFoWComponentBase::GetFoWController()
{
	AFoWControllerBase* controller = Cast<AFoWControllerBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AFoWControllerBase::StaticClass()));
	return controller;
}

void UFoWComponentBase::EndPlay(const EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);

	AFoWControllerBase* controller = GetFoWController();
	if (controller != nullptr)
	{
		controller->UnregisterVisionComponent(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Fog Of War Controller not found!"));
	}
}


// Called every frame
void UFoWComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

