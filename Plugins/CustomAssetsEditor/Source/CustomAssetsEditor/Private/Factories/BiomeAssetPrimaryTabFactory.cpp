#include "Factories/BiomeAssetPrimaryTabFactory.h"
#include "Apps/BiomeAssetEditorApp.h"

FBiomeAssetPrimaryTabFactory::FBiomeAssetPrimaryTabFactory(TSharedPtr<FBiomeAssetEditorApp> App)
	: FWorkflowTabFactory(FName("BiomeAssetPrimaryTab"), App)
{
	this->App = App;
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
