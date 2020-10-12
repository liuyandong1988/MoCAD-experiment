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
#include "UnrealEd.h"
#include "Factories.h"
#include "UObject/ObjectMacros.h"
#include "Factories/Factory.h"
#include "Factories/FbxSceneImportFactory.h"
#include "RoadRunnerFbxSceneImportFactory.generated.h"

////////////////////////////////////////////////////////////////////////////////
// Import with UFbxSceneImportFactory using the pre/post asset import delegates
// defined in RoadRunnerImporter.
UCLASS(hidecategories = Object)
class ROADRUNNERIMPORTER_API URoadRunnerFbxSceneImportFactory : public UFbxSceneImportFactory
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(Transient)
	class URoadRunnerImporterOptions* RoadRunnerImportOptions;

	// UFactory Interface
	virtual UObject* FactoryCreateFile(UClass* inClass, UObject* inParent, FName inName, EObjectFlags flags, const FString& filename, const TCHAR* parms, FFeedbackContext* warn, bool& outCancelled) override;
	virtual bool FactoryCanImport(const FString& filename) override;

public:
	bool OuterCall = true;
	TArray<UStaticMesh*> ImportedMeshes;
};
