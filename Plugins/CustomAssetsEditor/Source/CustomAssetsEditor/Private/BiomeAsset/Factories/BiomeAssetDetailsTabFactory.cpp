#include "BiomeAsset/Factories/BiomeAssetDetailsTabFactory.h"

#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Apps/BiomeAssetEditorApp.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

FBiomeAssetDetailsTabFactory::FBiomeAssetDetailsTabFactory(TSharedPtr<FBiomeAssetEditorApp> InApp)
	: FWorkflowTabFactory(Constants::DetailsTabIdentifier, InApp)
{
	App = InApp;
	TabLabel = FText::FromName(Constants::DetailsTabName);
	ViewMenuDescription = FText::FromString(TEXT("Displays the details view."));
	ViewMenuTooltip = FText::FromString(TEXT("Show the details view."));
}

TSharedRef<SWidget> FBiomeAssetDetailsTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	// return SNew(STextBlock).Text(FText::FromString(TEXT("This is a text widget created for the custom tab!")));
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

FText FBiomeAssetDetailsTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("The details view"));
}
