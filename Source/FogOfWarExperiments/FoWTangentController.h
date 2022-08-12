// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoWControllerBase.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/DecalComponent.h"
#include "PixelFormat.h"
#include "FoWTangentController.generated.h"

#define RGBA_REVERSE_ORDER 1

template <typename T>
struct FVectorAbgr
{
#if RGBA_REVERSE_ORDER == 0
	T A;
	T B;
	T G;
	T R;
#else
	T R;
	T G;
	T B;
	T A;
#endif

	FVectorAbgr(T r, T g, T b, T a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}

	FVectorAbgr()
	{
		A = 0;
		B = 0;
		G = 0;
		R = 0;
	}
};

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
	UMaterialInterface* textureDumpingMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstanceDynamic* textureDumpingMaterialDynamic;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* visionDrawingMaterial;

	// THIS UPROPERTY IS MANDATORY TO PREVENT UMaterialInstanceDynamic BEING GARBAGE COLLECTED!
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* visionDrawingMaterialDynamic;

	UPROPERTY(Transient)
	UDecalComponent* decalComponent;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EPixelFormat> arrayAsRgbaTexturePixelFormat = EPixelFormat::PF_A32B32G32R32F;

	UPROPERTY(EditDefaultsOnly)
	FVector2f arrayAsRgbaTextureSizeDefault = FVector2f(64, 64);

	UPROPERTY(Transient)
	UTexture2D* arrayAsRgbaTexture;

	int32 textureLocationParameterIndex;
	int32 textureSizeParameterIndex;
	int32 sourcesNumParameterIndex;
	int32 blockersNumParameterIndex;
	int32 arrayAsRGBATextureSizeParameterIndex;

	void CreateVisionDrawingMaterial();	

	void FillBinaryArray(void* data);
	void FillArrayAsRgbaTexture();

	UFUNCTION(Exec)
	void DumpTextureData();

	void DumpArray(FVectorAbgr<float>* pixels, int count);
	
protected:
	virtual void RenderFogOfWar() override;

	virtual void BeginPlay() override;
};
