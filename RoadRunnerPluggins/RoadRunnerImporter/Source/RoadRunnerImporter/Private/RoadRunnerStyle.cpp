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

#include "RoadRunnerStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FRoadRunnerStyle::StyleInstance = NULL;

////////////////////////////////////////////////////////////////////////////////
// Create the singleton for the style
void FRoadRunnerStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Unregister the style and clear the singleton
void FRoadRunnerStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

////////////////////////////////////////////////////////////////////////////////

FName FRoadRunnerStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("RoadRunnerStyle"));
	return StyleSetName;
}

////////////////////////////////////////////////////////////////////////////////

const FSlateBrush* FRoadRunnerStyle::GetLogoBrush()
{
	return StyleInstance->GetBrush(TEXT("RoadRunner.Logo"));
}

////////////////////////////////////////////////////////////////////////////////
// Unreal's image brush creation macro
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

////////////////////////////////////////////////////////////////////////////////
// Create the slate style set. Contains an image brush for the VectorZero logo
TSharedRef< FSlateStyleSet > FRoadRunnerStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("RoadRunnerStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("RoadRunnerImporter")->GetBaseDir() / TEXT("Resources"));

	Style->Set("RoadRunner.Logo", new IMAGE_BRUSH(TEXT("vz-logo-mini"), FVector2D(128.f, 72.f)));

	return Style;
}

#undef IMAGE_BRUSH

////////////////////////////////////////////////////////////////////////////////

void FRoadRunnerStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

////////////////////////////////////////////////////////////////////////////////

const ISlateStyle& FRoadRunnerStyle::Get()
{
	return *StyleInstance;
}

////////////////////////////////////////////////////////////////////////////////
