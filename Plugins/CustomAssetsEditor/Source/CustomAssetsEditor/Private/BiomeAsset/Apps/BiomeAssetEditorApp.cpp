#include "BiomeAsset/Apps/BiomeAssetEditorApp.h"

#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Modes/BiomeAssetAppMode.h"

void FBiomeAssetEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManagerRef)
{
	FWorkflowCentricApplication::RegisterTabSpawners(TabManagerRef);
}

void FBiomeAssetEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);
	WorkingAsset = Cast<Constants::UAssetSupportType>(InObject);

	InitAssetEditor(Mode,
		InitToolkitHost,
		Constants::ApplicationIdentifier,
		FTabManager::FLayout::NullLayout,
		true,
		true,
		ObjectsToEdit
	);

	AddApplicationMode(Constants::MainModeName, MakeShareable(new FBiomeAssetAppMode(SharedThis(this))));
	SetCurrentMode(Constants::MainModeName);
}

FName FBiomeAssetEditorApp::GetToolkitFName() const
{
	return Constants::CategoryKey;
}

FText FBiomeAssetEditorApp::GetBaseToolkitName() const
{
	return FText::FromName(Constants::CategoryKey);
}

FString FBiomeAssetEditorApp::GetWorldCentricTabPrefix() const
{
	return Constants::CategoryKey.ToString();
}

FLinearColor FBiomeAssetEditorApp::GetWorldCentricTabColorScale() const
{
	return Constants::AssetDisplayColor;
}
