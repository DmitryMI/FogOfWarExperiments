// INPUT: float4 Texture World Location
// INPUT: float4 Texture World Size

// INPUT: float SourcesNum
// INPUT: float BlockersNum
// INPUT: float2 ArrayAsRGBATextureSize
// INPUT: Texture ArrayAsRGBATexture

// Array structure:
// [Pixel1 B][Pixel1 G][Pixel1 R][Pixel1 A][Pixel2 B][Pixel2 G][Pixel2 R][Pixel2 A]
// SourcesNum times [Source RGBA pixel], BlockersNum times [Blocker RGBA pixel]

// Source struct:
// 	R: x coordinate
// 	G: y coordinate
// 	B: z coordinate
// 	A: vision radius

// Blocker struct:
// 	R: x coordinate
// 	G: y coordinate
// 	B: z coordinate
// 	A: blocker radius

#define RESULT_VISIBLE 0
#define RESULT_INVISIBLE 1

#define PI 3.1415926535
#define TWO_PI 2 * PI

#define INDEX_TO_U(i, dim) ((fmod(i, dim.x) + 0.5) / dim.x)
#define INDEX_TO_V(i, dim) ((floor(i / dim.x) + 0.5) / dim.y)
#define INDEX_TO_UV(i, dim) (floor2(INDEX_TO_U(i, dim), INDEX_TO_V(i, dim)))

#define DISTANCE2D(v1, v2) (sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.xy) * (v1.y - v2.y)))

#define UNWIND_ANGLE(angleRad) {while(angleRad > PI) {angleRad -= TWO_PI;} while(angleRad < -PI) {angleRad += TWO_PI;}}

#define IS_ANGLE_IN_SECTOR(result, a, min, max) {\
	float minUnwinded = min - a;\
	UNWIND_ANGLE(minUnwinded);\
	float maxUnwinded = max - a;\
	UNWIND_ANGLE(maxUnwinded);\
	result = minUnwinded < 0 && maxUnwinded < 0;\
	}

//float3 blur = Texture2DSample(Tex, TexSampler, UV);

float2 textureSize2D = float2(TextureWorldSize.x, TextureWorldSize.y);
float2 textureLocation2D = float2(TextureWorldLocation.x, TextureWorldLocation.y);

float2 pixelWorldLocation = UV * textureSize2D + textureLocation2D - (textureSize2D / 2);

for(int s = 0; s < SourcesNum; s++)
{	
	int sourceArrayIndex = s;
	
	float2 sourceUv = INDEX_TO_UV(sourceArrayIndex, ArrayAsRGBATextureSize);
	float4 source = Texture2DSample(ArrayAsRGBATexture, TexSampler, sourceUv);
	
	float2 sourceLocation = float2(source.x, source.y, source.z);
	float sourceVisionRadius = source.w;
	
	float pixelSourceDistance = DISTANCE2D(pixelWorldLocation, sourceLocation);
	float pixelSourceAngle = atan2(
		pixelWorldLocation.y - sourceLocation.y,
		pixelWorldLocation.x - sourceLocation.x
		);
	
	if(sourceVisionRadius < pixelSourceDistance)
	{
		// Out of vision range for this source
		continue;
	}
	
	bool isBlockedByAnyBlocker = false;
	
	for(int b = 0; b < BlockersNum; b++)
	{
		int blockerArrayIndex = SourcesNum + b;
		
		float2 blockerUv = INDEX_TO_UV(blockerArrayIndex, ArrayAsRGBATextureSize);
		float4 blocker = Texture2DSample(ArrayAsRGBATexture, TexSampler, blockerUv);
	
		float2 blockerLocation = float2(blocker.x, blocker.y, blocker.z);
		float blockerRadius = blocker.w;
		
		float sourceBlockerDistance = DISTANCE2D(blockerLocation, sourceLocation);
		
		float blockerSourceAngle = atan2(
			blockerLocation.y - sourceLocation.y,
			blockerLocation.x - sourceLocation.x
		);
		
		float blockerSourceAngularSize = asin(blockerRadius / sourceBlockerDistance);
		
		if(pixelSourceDistance < sourceBlockerDistance)
		{
			// Pixel is not blocked for this source by this blocker
			continue;
		}
		
		bool inSector;
		IS_ANGLE_IN_SECTOR(
			inSector,
			pixelSourceAngle,
			blockerSourceAngle - blockerSourceAngularSize,
			blockerSourceAngle + blockerSourceAngularSize
		);
		
		if(inSector)
		{
			// Pixel is behind the blocker
			isBlockedByAnyBlocker = true;
			break;
		}
	}
	
	if(!isBlockedByAnyBlocker)
	{
		return RESULT_VISIBLE;
	}
}

return RESULT_INVISIBLE;