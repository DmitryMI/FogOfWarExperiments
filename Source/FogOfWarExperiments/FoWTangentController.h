// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoWControllerBase.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/DecalComponent.h"
#include "FoWTangentController.generated.h"

/**
 * 
 */
UCLASS()
class FOGOFWAREXPERIMENTS_API AFoWTangentController : public AFoWControllerBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	UTextureRenderTarget2D* renderTarget;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* visionDrawingMaterial;

	// THIS UPROPERTY IS MANDATORY TO PREVENT UMaterialInstanceDynamic BEING GARBAGE COLLECTED!
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* visionDrawingMaterialDynamic;

	UPROPERTY(Transient)
	UDecalComponent* decalComponent;

	int32 visionRadiusParameterIndex;
	int32 blockerRadiusParameterIndex;
	int32 textureLocationParameterIndex;
	int32 textureSizeParameterIndex;
	int32 sourceLocationParameterIndex;
	int32 blockerLocationParameterIndex;

	void CreateVisionDrawingMaterial();
	
protected:
	virtual void RenderFogOfWar() override;

	virtual void BeginPlay() override;
};
