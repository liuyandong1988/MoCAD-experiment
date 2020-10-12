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

#include "RoadRunnerImporterOptionWindow.h"
#include "RoadRunnerImporterOptions.h"
#include "RoadRunnerStyle.h"

#include <Widgets/Layout/SBox.h>
#include <Widgets/Layout/SUniformGridPanel.h>
#include <Widgets/Images/SImage.h>
#include <Widgets/Input/SButton.h>
#include <Widgets/Layout/SSplitter.h>
#include <Widgets/Input/SCheckBox.h>
#include <Widgets/Text/STextBlock.h>
#include <IDocumentation.h>
#include <Internationalization.h>
#include <EditorStyleSet.h>

#include <IPluginManager.h>
#include <Brushes/SlateImageBrush.h>
#define LOCTEXT_NAMESPACE "FRoadRunnerImporterModule"

////////////////////////////////////////////////////////////////////////////////

SRoadRunnerImporterOptionWindow::SRoadRunnerImporterOptionWindow()
{
	OwnerWindow = nullptr;
	ImportOptions = nullptr;
	ShouldImport_ = false;
	VersionOutOfDate = false;
}

////////////////////////////////////////////////////////////////////////////////

SRoadRunnerImporterOptionWindow::~SRoadRunnerImporterOptionWindow()
{
	OwnerWindow = nullptr;
	ImportOptions = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void SRoadRunnerImporterOptionWindow::Construct(const FArguments & inArgs)
{
	OwnerWindow = inArgs._OwnerWindow;
	FullPath = inArgs._FullPath;
	ImportOptions = inArgs._ImportOptions;
	VersionOutOfDate = inArgs._VersionOutOfDate;

	this->ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(5)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("ToolPanel.LightGroupBorder"))
					[
						SNew(SImage)
						.Image(FRoadRunnerStyle::GetLogoBrush())
					]
				]
			]
			+ SHorizontalBox::Slot()
			.Padding(5)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBorder)
						.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
						.Padding(3)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("OptionsTitle", "Import Options"))
							.TextStyle(&FEditorStyle::GetWidgetStyle<FTextBlockStyle>("RichTextBlock.Bold"))
						]
					]
					+SVerticalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoHeight()
					[
						SNew(SUniformGridPanel)
						.SlotPadding(10)
						+SUniformGridPanel::Slot(0, 0)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("MaterialOptionText", "Overwrite Materials"))
						]
						+SUniformGridPanel::Slot(1, 0)
						[
							SNew(SCheckBox)
							.IsChecked(this, &SRoadRunnerImporterOptionWindow::ShouldOverwriteMaterials)
							.OnCheckStateChanged(this, &SRoadRunnerImporterOptionWindow::OnOverwriteMaterialsChanged)
						]
						+ SUniformGridPanel::Slot(0, 1)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("SignalOptionText", "Import Signal Visuals*"))
						]
						+ SUniformGridPanel::Slot(1, 1)
						[
							SNew(SCheckBox)
							.IsChecked(this, &SRoadRunnerImporterOptionWindow::ShouldImportSignals)
							.OnCheckStateChanged(this, &SRoadRunnerImporterOptionWindow::OnImportSignalsChanged)
						]
					]
					+SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Bottom)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Disclaimer", "*Only compatible when importing as blueprint"))
					]
				]
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(2)
			.HAlign(HAlign_Left)
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor::Red)
				.Visibility(GetVersionWarningVisibility())
				.Text(LOCTEXT("VersionWarning", "Plugin is out of date, please update."))
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			[
				SNew(SUniformGridPanel)
				.SlotPadding(2)
				+ SUniformGridPanel::Slot(0, 0)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("ImportBox", "Import"))
					.IsEnabled(this, &SRoadRunnerImporterOptionWindow::CanImport)
					.OnClicked(this, &SRoadRunnerImporterOptionWindow::OnImport)
				]
				+ SUniformGridPanel::Slot(1, 0)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("CancelBox", "Cancel"))
					.IsEnabled(this, &SRoadRunnerImporterOptionWindow::CanImport)
					.OnClicked(this, &SRoadRunnerImporterOptionWindow::OnCancel)
				]
			]
		]
	];
}

////////////////////////////////////////////////////////////////////////////////

void SRoadRunnerImporterOptionWindow::CloseFbxSceneOption()
{
	if (OwnerWindow.IsValid())
	{
		//Close the window
		OwnerWindow->RequestDestroyWindow();
	}
	OwnerWindow = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

bool SRoadRunnerImporterOptionWindow::CanCloseTab()
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool SRoadRunnerImporterOptionWindow::CanImport() const
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////

ECheckBoxState SRoadRunnerImporterOptionWindow::ShouldOverwriteMaterials() const
{
	return ImportOptions->OverwriteMaterials ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

////////////////////////////////////////////////////////////////////////////////

void SRoadRunnerImporterOptionWindow::OnOverwriteMaterialsChanged(ECheckBoxState NewState)
{
	const bool bState = (NewState == ECheckBoxState::Checked) ? true : false;
	if (ImportOptions->OverwriteMaterials != bState)
	{
		ImportOptions->OverwriteMaterials = bState;
	}
}

////////////////////////////////////////////////////////////////////////////////

ECheckBoxState SRoadRunnerImporterOptionWindow::ShouldImportSignals() const
{
	return ImportOptions->ImportSignals ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; return ECheckBoxState();
}

////////////////////////////////////////////////////////////////////////////////

void SRoadRunnerImporterOptionWindow::OnImportSignalsChanged(ECheckBoxState NewState)
{
	const bool bState = (NewState == ECheckBoxState::Checked) ? true : false;
	if (ImportOptions->ImportSignals != bState)
	{
		ImportOptions->ImportSignals = bState;
	}
}

#undef LOCTEXT_NAMESPACE
