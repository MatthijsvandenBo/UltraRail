#include "BiomeAsset/Modes/BiomeAssetAppMode.h"

#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Apps/BiomeAssetEditorApp.h"
#include "BiomeAsset/Factories/BiomeAssetDetailsTabFactory.h"

FBiomeAssetAppMode::FBiomeAssetAppMode(TSharedPtr<FBiomeAssetEditorApp> InApp)
	: FApplicationMode(Constants::MainModeName)
{
	App = InApp;
	Tabs.RegisterFactory(MakeShareable(new FBiomeAssetDetailsTabFactory(InApp)));

	TabLayout = FTabManager::NewLayout("BiomeAssetAppMode_Layout_v1.0.1")
	->AddArea(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split(
			FTabManager::NewStack()
			->AddTab(Constants::DetailsTabIdentifier, ETabState::OpenedTab)
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
