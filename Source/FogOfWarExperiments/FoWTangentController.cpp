// Fill out your copyright notice in the Description page of Project Settings.


#include "FoWTangentController.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/Texture2D.h"
#include "MaterialShared.h"


void AFoWTangentController::CreateVisionDrawingMaterial()
{
	visionDrawingMaterialDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), visionDrawingMaterial, "VisionDrawingMaterialInstanceDynamic");
	
	bool paramInitOk = true;
	
	paramInitOk &= visionDrawingMaterialDynamic->InitializeVectorParameterAndGetIndex("TextureWorldLocation", FLinearColor::Black, textureLocationParameterIndex);
	paramInitOk &= visionDrawingMaterialDynamic->InitializeVectorParameterAndGetIndex("TextureWorldSize", FLinearColor::Black, textureSizeParameterIndex);

	paramInitOk &= visionDrawingMaterialDynamic->InitializeScalarParameterAndGetIndex("SourcesNum", 0, sourcesNumParameterIndex);
	paramInitOk &= visionDrawingMaterialDynamic->InitializeScalarParameterAndGetIndex("BlockersNum", 0, blockersNumParameterIndex);

	arrayAsRgbaTexture = UTexture2D::CreateTransient(arrayAsRgbaTextureSizeDefault.X, arrayAsRgbaTextureSizeDefault.Y, arrayAsRgbaTexturePixelFormat);
	visionDrawingMaterialDynamic->SetTextureParameterValue("ArrayAsRGBATexture", arrayAsRgbaTexture);

	paramInitOk &= visionDrawingMaterialDynamic->InitializeVectorParameterAndGetIndex("ArrayAsRGBATextureSize", FLinearColor::Black, arrayAsRGBATextureSizeParameterIndex);

}

void AFoWTangentController::FillBinaryArray(void* data)
{
	int itemsMax = arrayAsRgbaTexture->GetSizeX() * arrayAsRgbaTexture->GetSizeY();

	if (arrayAsRgbaTexturePixelFormat == EPixelFormat::PF_A32B32G32R32F)
	{
		FVectorAbgr<float>* pixelsAbgr = (FVectorAbgr<float>*)data;

		int index = 0;
		for (auto source : GetVisionSources())
		{
			FVector location = source->GetOwner()->GetActorLocation();
			FVectorAbgr<float> pixel = FVectorAbgr<float>(location.X, location.Y, location.Z, source->GetVisionRange());
			pixelsAbgr[index] = pixel;

			index++;

			if (index == itemsMax)
			{
				UE_LOG(LogTemp, Error, TEXT("Texture is too small to fit all Vision Sources!"))
					break;
			}
		}

		for (auto blocker : GetVisionBlockers())
		{
			FVector location = blocker->GetOwner()->GetActorLocation();
			FVectorAbgr<float> pixel = FVectorAbgr<float>(location.X, location.Y, location.Z, blocker->GetVisionBlockRadius());
			pixelsAbgr[index] = pixel;

			index++;

			if (index == itemsMax)
			{
				UE_LOG(LogTemp, Error, TEXT("Texture is too small to fit all Vision Blockers!"))
					break;
			}
		}

		//DumpArray(pixelsAbgr, index);
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("This pixel format is not implemented!"));
	}
}

void AFoWTangentController::FillArrayAsRgbaTexture()
{
	if (arrayAsRgbaTexture == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("arrayAsRgbaTexture is nullptr!"));
		return;
	}

	FTexture2DMipMap* MipMap = &(arrayAsRgbaTexture->GetPlatformData()->Mips[0]);
	FByteBulkData* ImageData = &MipMap->BulkData;	

	void* RawImageData = ImageData->Lock(LOCK_READ_WRITE);

	FillBinaryArray(RawImageData);

	

	ImageData->Unlock();
	RawImageData = nullptr;

	arrayAsRgbaTexture->UpdateResource();
}

void AFoWTangentController::DumpTextureData()
{
	auto sources = GetVisionSources();
	auto blockers = GetVisionBlockers();

	int count = sources.Num() + blockers.Num();

	FVectorAbgr<float>* data = new FVectorAbgr<float>[sources.Num() + blockers.Num()];

	FillBinaryArray(data);

	DumpArray(data, count);

	delete[] data;
}

void AFoWTangentController::DumpArray(FVectorAbgr<float>* pixels, int count)
{
	FString msg = "Pixels: ";
	for (int i = 0; i < count; i++)
	{
		auto pixel = pixels[i];
		msg += FString::Printf(TEXT("%f, %f, %f, %f, "), pixel.A, pixel.B, pixel.G, pixel.R);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
}

void AFoWTangentController::RenderFogOfWar()
{
	FVector textureWorldLocation = GetFogOfWarActor()->GetActorLocation();

	FVector textureWorldSize = FVector(decalComponent->DecalSize.Z, decalComponent->DecalSize.Y, decalComponent->DecalSize.X);

	textureWorldSize *= 2.0f;

	visionDrawingMaterialDynamic->SetVectorParameterByIndex(textureLocationParameterIndex, FLinearColor(textureWorldLocation));
	visionDrawingMaterialDynamic->SetVectorParameterByIndex(textureSizeParameterIndex, FLinearColor(textureWorldSize));

	FLinearColor arrSize = FLinearColor(arrayAsRgbaTextureSizeDefault.X, arrayAsRgbaTextureSizeDefault.Y, 0, 0);
	visionDrawingMaterialDynamic->SetVectorParameterByIndex(arrayAsRGBATextureSizeParameterIndex, arrSize);
	
	visionDrawingMaterialDynamic->SetScalarParameterByIndex(sourcesNumParameterIndex, GetVisionSources().Num());
	visionDrawingMaterialDynamic->SetScalarParameterByIndex(blockersNumParameterIndex, GetVisionBlockers().Num());

	// We do not set texture parameter here, it is only done once during BeginPlay()

	FillArrayAsRgbaTexture();
}

void AFoWTangentController::BeginPlay()
{
	Super::BeginPlay();

	if (visionDrawingMaterial == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("AFoWTangentController: vision drawing material not specified!"));
		return;
	}

	CreateVisionDrawingMaterial();

	decalComponent = Cast<UDecalComponent>(GetComponentByClass(UDecalComponent::StaticClass()));

	decalComponent->SetMaterial(0, visionDrawingMaterialDynamic);
}
