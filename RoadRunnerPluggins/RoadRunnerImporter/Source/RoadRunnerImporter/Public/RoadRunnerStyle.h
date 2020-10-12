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
#include "Styling/SlateStyle.h"

////////////////////////////////////////////////////////////////////////////////
// Slate style for the RoadRunner importer plugin
class FRoadRunnerStyle
{
public:
	// Setup and cleanup functions
	static void Initialize();
	static void Shutdown();

	// Reloads textures used by slate renderer
	static void ReloadTextures();

	// The Slate style set for the plugin
	static const ISlateStyle& Get();

	// Helper function to get the brush for the VectorZero logo
	static const FSlateBrush* GetLogoBrush();

	static FName GetStyleSetName();

private:
	static TSharedRef< class FSlateStyleSet > Create();
	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};
