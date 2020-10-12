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
#include "Factories/FbxFactory.h"
#include "RoadRunnerFbxFactory.generated.h"

////////////////////////////////////////////////////////////////////////////////
// Attempts to import using the custom RoadRunner scene import factory.
// If it fails, go back to the default fbx importer.
// Note: This factory is needed to overwrite the normal fbx import with
// our scene importer since Factory priority doesn't work with FbxSceneImportFactory
UCLASS(hidecategories = Object)
class ROADRUNNERIMPORTER_API URoadRunnerFbxFactory : public UFbxFactory
{
	GENERATED_UCLASS_BODY()

	// UFactory Interface
	virtual UObject* FactoryCreateFile(UClass* inClass, UObject* inParent, FName inName, EObjectFlags flags, const FString& filename, const TCHAR* parms, FFeedbackContext* warn, bool& outCancelled) override;
	virtual bool FactoryCanImport(const FString& filename) override;
};
