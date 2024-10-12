#include "BiomeAsset/Factories/BiomeAssetPrimaryTabFactory.h"

#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Apps/BiomeAssetEditorApp.h"

FBiomeAssetPrimaryTabFactory::FBiomeAssetPrimaryTabFactory(TSharedPtr<FBiomeAssetEditorApp> InApp)
	: FWorkflowTabFactory(Constants::PrimaryTabIdentifier, InApp)
{
	App = InApp;
	TabLabel = FText::FromString(TEXT("Primary"));
	ViewMenuDescription = FText::FromString(TEXT("Displays the primary view."));
	ViewMenuTooltip = FText::FromString(TEXT("Show the primary view."));
}

TSharedRef<SWidget> FBiomeAssetPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(STextBlock).Text(FText::FromString(TEXT("This is a text widget created for the custom tab!")));
}

FText FBiomeAssetPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("The primary view"));
}
