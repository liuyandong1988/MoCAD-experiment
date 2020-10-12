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

#include <Runtime/Launch/Resources/Version.h>
#if ENGINE_MINOR_VERSION < 17
#error The RoadRunner import plugin only supports Unreal Engine version 4.17 and up.
#endif

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include <UnrealEd.h>


////////////////////////////////////////////////////////////////////////////////
// Imports FBX files with RoadRunner metadata
//	- Parses metadata XML lookaside file to set material properties and other attributes
//	- Material instances are created from the base materials located in the plugin's content folder
//  - Sets up signal components after importing
class FRoadRunnerImporterModule : public IModuleInterface
{
public:
	// Asset processing delegates
	static void RoadRunnerPostProcessing(UFactory* inFactory, UObject* inCreateObject);

	// IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static const int PluginVersion = 3;
	static const int32 TransparentRenderQueue = 1000;
};
