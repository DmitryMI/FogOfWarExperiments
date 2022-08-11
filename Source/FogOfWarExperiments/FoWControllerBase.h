// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FowVisionSourceComponentBase.h"
#include "FowVisionBlockerComponentBase.h"
#include "FoWControllerBase.generated.h"

UCLASS()
class FOGOFWAREXPERIMENTS_API AFoWControllerBase : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float screenRayLength = 100000.0f;

	UPROPERTY(EditAnywhere)
	float fogOfWarMinZ = -500.0f;

	UPROPERTY(EditAnywhere)
	float viewportBoxMinZ = 0.0f;

	UPROPERTY(Transient)
	TArray<UFoWVisionSourceComponentBase*> visionSources;

	UPROPERTY(Transient)
	TArray<UFoWVisionBlockerComponentBase*> visionBlockers;

public:	
	AFoWControllerBase();

protected:
	virtual void BeginPlay() override;	

	virtual void GetViewportTransform(FBox& OutBoundingBoxWorld, FRotator& OutWorldRotation, FVector& OutCameraLocation, FRotator& OutCameraRotation);

	virtual AActor* GetFogOfWarActor();

	virtual void AdjustFogOfWarActorTransform(const FBox& viewportWorldBox, const FRotator& viewportWorldRotation);

	virtual void RenderFogOfWar();

	FVector GetFogOfWarCenter();

	FBox GetBoundingBox(const FRotator& rotation, const FVector& pivot, const TArray<FVector>& points);

	FPlane GetGroundPlane();

	FVector IntersectGroundPlane(const FVector& start, const FVector& dir);	

	void DrawDebugViewportTransform(const FBox& OutBoundingBoxWorld, const FRotator& worldRotation, const FVector& cameraLocation, const FRotator& cameraRotation);

	const TArray<UFoWVisionBlockerComponentBase*>& GetVisionBlockers() const;

	const TArray<UFoWVisionSourceComponentBase*>& GetVisionSources() const;

public:	
	virtual void Tick(float DeltaTime) override;

	void RegisterVisionComponent(UFoWComponentBase* component);
	void UnregisterVisionComponent(UFoWComponentBase* component);

	void RegisterVisionSource(UFoWVisionSourceComponentBase* source);
	void RegisterVisionBlocker(UFoWVisionBlockerComponentBase* blocker);
	void UnregisterVisionSource(UFoWVisionSourceComponentBase* source);
	void UnregisterVisionBlocker(UFoWVisionBlockerComponentBase* blocker);
};
