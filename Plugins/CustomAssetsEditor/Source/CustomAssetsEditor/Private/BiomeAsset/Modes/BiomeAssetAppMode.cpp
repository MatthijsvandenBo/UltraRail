#include "BiomeAsset/Modes/BiomeAssetAppMode.h"

#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Apps/BiomeAssetEditorApp.h"
#include "BiomeAsset/Factories/BiomeAssetPropertiesTabFactory.h"
#include "BiomeAsset/Factories/BiomeAssetGraphTabFactory.h"

FBiomeAssetAppMode::FBiomeAssetAppMode(TSharedPtr<FBiomeAssetEditorApp> InApp)
	: FApplicationMode(Constants::MainModeName)
{
	App = InApp;
	Tabs.RegisterFactory(MakeShareable(new FBiomeAssetPropertiesTabFactory(InApp)));
	Tabs.RegisterFactory(MakeShareable(new FBiomeAssetGraphTabFactory(InApp)));

	TabLayout = FTabManager::NewLayout(Constants::MainLayoutName)
	->AddArea(
		FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split(
				FTabManager::NewSplitter()
					->SetOrientation(Orient_Horizontal)
					->Split(
						FTabManager::NewStack()
							->SetSizeCoefficient(0.85f)
							->AddTab(Constants::GraphTabIdentifier, ETabState::OpenedTab)
					)
					->Split(
						FTabManager::NewStack()
							->SetSizeCoefficient(0.15f)
							->AddTab(Constants::PropertiesTabIdentifier, ETabState::OpenedTab)
					)
			)
	);
}

void FBiomeAssetAppMode::RegisterTabFactories(const TSharedPtr<FTabManager> InTabManager)
{
	const TSharedPtr<FBiomeAssetEditorApp> Pin = App.Pin();
	Pin->PushTabFactories(Tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FBiomeAssetAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FBiomeAssetAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
