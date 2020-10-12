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
#include "InputCoreTypes.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Styling/SlateColor.h"
#include "Input/Reply.h"
#include "Widgets/SCompoundWidget.h"
#include "Framework/Docking/TabManager.h"
#include "SlateFwd.h"

class SRoadRunnerImporterOptionWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRoadRunnerImporterOptionWindow)
		: _OwnerWindow()
		, _FullPath(TEXT(""))
		{}
	
		SLATE_ARGUMENT(TSharedPtr<SWindow>, OwnerWindow)
		SLATE_ARGUMENT(FString, FullPath)
		SLATE_ARGUMENT(class URoadRunnerImporterOptions*, ImportOptions)
		SLATE_ARGUMENT(bool, VersionOutOfDate)
	SLATE_END_ARGS()

public:

	SRoadRunnerImporterOptionWindow();
	~SRoadRunnerImporterOptionWindow();
	void Construct(const FArguments& InArgs);
	virtual bool SupportsKeyboardFocus() const override { return true; }

	void CloseFbxSceneOption();

	FReply OnImport()
	{
		ShouldImport_ = true;
		CloseFbxSceneOption();
		return FReply::Handled();
	}

	FReply OnCancel()
	{
		ShouldImport_ = false;
		CloseFbxSceneOption();
		return FReply::Handled();
	}

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override
	{
		if (InKeyEvent.GetKey() == EKeys::Escape)
		{
			return OnCancel();
		}

		return FReply::Unhandled();
	}

	bool ShouldImport() const
	{
		return ShouldImport_;
	}


private:
	bool CanCloseTab();

	bool CanImport() const;

	ECheckBoxState ShouldOverwriteMaterials() const;
	void OnOverwriteMaterialsChanged(ECheckBoxState NewState);

	ECheckBoxState ShouldImportSignals() const;
	void OnImportSignalsChanged(ECheckBoxState NewState);

	EVisibility GetVersionWarningVisibility()
	{
		if (VersionOutOfDate)
			return EVisibility::Visible;
		else
			return EVisibility::Hidden;
	}

private:

	TSharedPtr< SWindow > OwnerWindow;
	FString FullPath;
	class URoadRunnerImporterOptions* ImportOptions;

	bool ShouldImport_;
	bool VersionOutOfDate;
};
