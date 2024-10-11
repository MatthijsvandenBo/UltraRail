#include "Modes/BiomeAssetAppMode.h"
#include "Apps/BiomeAssetEditorApp.h"
#include "Factories/BiomeAssetPrimaryTabFactory.h"

FBiomeAssetAppMode::FBiomeAssetAppMode(TSharedPtr<FBiomeAssetEditorApp> App)
	: FApplicationMode(TEXT("BiomeAssetAppMode"))
{
	this->App = App;
	Tabs.RegisterFactory(MakeShareable(new FBiomeAssetPrimaryTabFactory(App)));

	TabLayout = FTabManager::NewLayout("BiomeAssetAppMode_Layout_v1")
	->AddArea(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split(
			FTabManager::NewStack()
			->AddTab(FName(TEXT("BiomeAssetPrimaryTab")), ETabState::OpenedTab)
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
