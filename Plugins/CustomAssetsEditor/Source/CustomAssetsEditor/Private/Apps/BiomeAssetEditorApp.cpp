#include "Apps/BiomeAssetEditorApp.h"

#include "Modes/BiomeAssetAppMode.h"

void FBiomeAssetEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManagerRef)
{
	FWorkflowCentricApplication::RegisterTabSpawners(TabManagerRef);
}

void FBiomeAssetEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);

	InitAssetEditor(Mode,
		InitToolkitHost,
		TEXT("BiomeAssetEditor"),
		FTabManager::FLayout::NullLayout,
		true,
		true,
		ObjectsToEdit
	);

	AddApplicationMode(TEXT("BiomeAssetAppMode"), MakeShareable(new FBiomeAssetAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("BiomeAssetAppMode"));
}
