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
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "RoadRunnerImporterOptions.generated.h"

////////////////////////////////////////////////////////////////////////////////
// Options for the RoadRunner importer plugin.
UCLASS(config=EditorPerProjectUserSettings, HideCategories=Object, MinimalAPI)
class URoadRunnerImporterOptions : public UObject
{
	GENERATED_UCLASS_BODY()
	

	// Determines if the plugin will overwrite the previous materials attached
	// with new materials instanced from the base materials.
	UPROPERTY(EditAnywhere, config, category = ImportOptions)
	uint32 OverwriteMaterials : 1;

	// Determines if the plugin should import traffic signal visuals.
	// Currently only compatible with the import as Blueprint option.
	UPROPERTY(EditAnywhere, config, category=ImportOptions)
	uint32 ImportSignals : 1;
};
