// Fill out your copyright notice in the Description page of Project Settings.


#include "FoWTangentController.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "MaterialShared.h"
#include "Components/DecalComponent.h"

void AFoWTangentController::CreateVisionDrawingMaterial()
{
	visionDrawingMaterialDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), visionDrawingMaterial, "VisionDrawingMaterialInstanceDynamic");

	if (!visionDrawingMaterialDynamic->InitializeVectorParameterAndGetIndex("TextureWorldLocation", FLinearColor::Black, textureLocationParameterIndex))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Cannot initialize parameter TextureWorldLocation"));
	}
	if (!visionDrawingMaterialDynamic->InitializeVectorParameterAndGetIndex("TextureWorldSize", FLinearColor::Black, textureSizeParameterIndex))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Cannot initialize parameter TextureWorldSize"));
	}
	if (!visionDrawingMaterialDynamic->InitializeVectorParameterAndGetIndex("SourceLocationWorld", FLinearColor::Black, sourceLocationParameterIndex))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Cannot initialize parameter SourceLocationWorld"));
	}
	if (!visionDrawingMaterialDynamic->InitializeVectorParameterAndGetIndex("BlockerLocationWorld", FLinearColor::Black, blockerLocationParameterIndex))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Cannot initialize parameter BlockerLocationWorld"));
	}
	if (!visionDrawingMaterialDynamic->InitializeScalarParameterAndGetIndex("VisionRadius", 0, visionRadiusParameterIndex))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Cannot initialize parameter VisionRadius"));
	}
	if (!visionDrawingMaterialDynamic->InitializeScalarParameterAndGetIndex("BlockerRadius", 0, blockerRadiusParameterIndex))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Cannot initialize parameter BlockerRadius"));
	}
}

void AFoWTangentController::RenderFogOfWar()
{
	UCanvas* canvas;
	FVector2D size;
	FDrawToRenderTargetContext context;

	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), renderTarget, FLinearColor(1, 1, 1, 1));

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), renderTarget, canvas, size, context);

	UDecalComponent* decal = Cast<UDecalComponent>(GetComponentByClass(UDecalComponent::StaticClass()));

	FVector textureWorldLocation = GetFogOfWarActor()->GetActorLocation();

	FVector textureWorldSize = FVector(decal->DecalSize.Z, decal->DecalSize.Y, decal->DecalSize.X);

	textureWorldSize *= 2.0f;

	//visionDrawingMaterialDynamic->SetVectorParameterValue("TextureWorldLocation", textureWorldLocation);
	visionDrawingMaterialDynamic->SetVectorParameterByIndex(textureLocationParameterIndex, FLinearColor(textureWorldLocation));

	//visionDrawingMaterialDynamic->SetVectorParameterValue("TextureWorldSize", textureWorldSize);
	visionDrawingMaterialDynamic->SetVectorParameterByIndex(textureSizeParameterIndex, FLinearColor(textureWorldSize));

	for (auto source : GetVisionSources())
	{
		if (source == nullptr)
		{
			continue;
		}
		// float4 TextureWorldLocation (2D)
		// float4 TextureWorldSize (2D)

		// float VisionRadius
		// float4 SourceLocationWorld (3D)
		// 
		// float BlockerRadius
		// float4 BlockerLocationWorld (3D)	
		
		float visionRadius = source->GetVisionRange();
		FVector sourceLocationWorld = source->GetOwner()->GetActorLocation();

		//visionDrawingMaterialDynamic->SetScalarParameterValue("VisionRadius", visionRadius);		
		visionDrawingMaterialDynamic->SetScalarParameterByIndex(visionRadiusParameterIndex, visionRadius);

		//visionDrawingMaterialDynamic->SetVectorParameterValue("SourceLocationWorld", sourceLocationWorld);
		visionDrawingMaterialDynamic->SetVectorParameterByIndex(sourceLocationParameterIndex, FLinearColor(sourceLocationWorld));

		for (auto blocker : GetVisionBlockers())
		{
			if (blocker == nullptr)
			{
				continue;
			}

			float blockerRadius = blocker->GetVisionBlockRadius();
			FVector blockerLocationWorld = blocker->GetOwner()->GetActorLocation();

			//visionDrawingMaterialDynamic->SetScalarParameterValue("BlockerRadius", blockerRadius);			
			visionDrawingMaterialDynamic->SetScalarParameterByIndex(blockerRadiusParameterIndex, blockerRadius);

			//visionDrawingMaterialDynamic->SetVectorParameterValue("BlockerLocationWorld", blockerLocationWorld);		
			visionDrawingMaterialDynamic->SetVectorParameterByIndex(blockerLocationParameterIndex, FLinearColor(blockerLocationWorld));

			FVector2D screenPosition = FVector2D::ZeroVector;
			FVector2D screenSize = FVector2D(renderTarget->SizeX, renderTarget->SizeY);
			FVector2D coordinatePosition = FVector2D::ZeroVector;

			canvas->K2_DrawMaterial(visionDrawingMaterialDynamic, screenPosition, screenSize, coordinatePosition);
		}
	}

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), context);
}

void AFoWTangentController::BeginPlay()
{
	Super::BeginPlay();

	if (visionDrawingMaterial == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("AFoWTangentController: vision drawing material not specified!"));
		return;
	}

	//visionDrawingMaterialDynamic = UMaterialInstanceDynamic::Create(visionDrawingMaterial, this);
	CreateVisionDrawingMaterial();
}
