#pragma once

#include "CoreMinimal.h"
#include "BiomeAsset/Constants.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class UEdGraph;

class FBiomeAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
	
private:
	Constants::UAssetSupportType* WorkingAsset = nullptr;
	UEdGraph* WorkingGraph = nullptr;
	
public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject);

	FORCEINLINE Constants::UAssetSupportType* GetWorkingAsset() const { return WorkingAsset; }
	FORCEINLINE UEdGraph* GetWorkingGraph() const { return WorkingGraph; }

	/// FAssetEditorToolkit interface

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetDocumentationLink() const override { return TEXT(""); }

	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override {}
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override {}
};
