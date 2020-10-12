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
#include "Modules/ModuleManager.h"
#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(RoadRunnerRuntime, Log, All);

////////////////////////////////////////////////////////////////////////////////
// Module for runtime components of the RoadRunnerImporter
class FRoadRunnerRuntimeModule : public IModuleInterface
{
public:
	// IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
