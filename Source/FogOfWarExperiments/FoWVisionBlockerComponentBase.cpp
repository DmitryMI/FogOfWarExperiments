// Fill out your copyright notice in the Description page of Project Settings.


#include "FoWVisionBlockerComponentBase.h"

UFoWVisionBlockerComponentBase::UFoWVisionBlockerComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UFoWVisionBlockerComponentBase::GetVisionBlockRadius()
{
	return visionBlockRadius;
}

void UFoWVisionBlockerComponentBase::SetVisionBlockRadius(float value)
{
	visionBlockRadius = value;
}


void UFoWVisionBlockerComponentBase::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UFoWVisionBlockerComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

