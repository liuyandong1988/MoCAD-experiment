// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include "CoreMinimal.h"

class FXmlNode;
class UTexture;
class UMaterialInstanceConstant;

////////////////////////////////////////////////////////////////////////////////
// Helper functions for importing from RoadRunner
class ROADRUNNERIMPORTER_API FRoadRunnerImportUtil
{
public:
	// Holds strings from the road runner metadata file
	struct MaterialInfo
	{
		FString Name;
		FString DiffuseMap;
		FString NormalMap;
		FString SpecularMap;
		FString DiffuseColor;
		FString SpecularColor;
		FString SpecularFactor;
		FString TransparencyMap;
		FString TransparencyFactor;
		FString Roughness;
		FString Emission;
		FString TextureScaleU;
		FString TextureScaleV;
		FString TwoSided;
		FString DrawQueue;
		FString ShadowCaster;
		FString IsDecal;
		FString SegmentationType;
	};

	static TArray<MaterialInfo> ParseMaterialXml(FXmlNode* matList);
	static UTexture* FindTexture(FString absFilePath, FString packagePath);
	static void SetTextureParameter(UMaterialInstanceConstant* material, const FName& paramName, const FString& baseFilePath, const FString& texturePath, const FString& packagePath);
	static void SetColorParameter(UMaterialInstanceConstant* material, const FName& paramName, const FString& colorString, float alphaVal);
	static void SetScalarParameter(UMaterialInstanceConstant* material, const FName& paramName, const FString& valueString);
	static FString ConvertMaterialName(const FString mtlName);

	static bool TempFolderExists();
	static void CleanUpTempFolder();

	static void FixUpRedirectors();

	static const FString TempFolderPath;
};