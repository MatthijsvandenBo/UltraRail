#include "BiomeAsset/Factories/BiomeAssetGraphTabFactory.h"

#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Apps/BiomeAssetEditorApp.h"
#include "GraphEditor.h"
#include "EditionUpgradeHelper.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

FBiomeAssetGraphTabFactory::FBiomeAssetGraphTabFactory(TSharedPtr<FBiomeAssetEditorApp> InApp)
	: FWorkflowTabFactory(Constants::GraphTabIdentifier, InApp)
{
	App = InApp;
	TabLabel = FText::FromName(Constants::GraphTabName);
	ViewMenuDescription = FText::FromString(TEXT("Displays the graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the graph editor"));
}

TSharedRef<SWidget> FBiomeAssetGraphTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	const auto Pin = App.Pin();
	
	return SNew(SVerticalBox)
	+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.HAlign(HAlign_Fill)
		[
			SNew(SGraphEditor)
				.IsEditable(true)
				.GraphToEdit(Pin->GetWorkingGraph())
		];
}

FText FBiomeAssetGraphTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("The graph-editor view"));
}
