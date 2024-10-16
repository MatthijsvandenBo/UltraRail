#include "BiomeAsset/Factories/BiomeAssetPropertiesTabFactory.h"

#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Apps/BiomeAssetEditorApp.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

FBiomeAssetPropertiesTabFactory::FBiomeAssetPropertiesTabFactory(TSharedPtr<FBiomeAssetEditorApp> InApp)
	: FWorkflowTabFactory(Constants::PropertiesTabIdentifier, InApp)
{
	App = InApp;
	TabLabel = FText::FromName(Constants::PropertiesTabName);
	ViewMenuDescription = FText::FromString(TEXT("Displays the properties view."));
	ViewMenuTooltip = FText::FromString(TEXT("Show the properties view."));
}

TSharedRef<SWidget> FBiomeAssetPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	const TSharedPtr<FBiomeAssetEditorApp> Pin = App.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bShowObjectLabel = true;
	DetailsViewArgs.bSearchInitialKeyFocus = false;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.NotifyHook = nullptr;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bShowModifiedPropertiesOption = true;
	DetailsViewArgs.bShowScrollBar = false;

	const TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(Pin->GetWorkingAsset());
	
	return SNew(SVerticalBox) + SVerticalBox::Slot()
		.FillHeight(1.f)
		.HAlign(HAlign_Fill)
		[
			DetailsView.ToSharedRef()
		];
}

FText FBiomeAssetPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("The properties view"));
}
