#pragma once

#include "CoreMinimal.h"
#include "SGraphPanel.h"
#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Factories/CustomNodeFactory.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class UEdGraph;

class FBiomeAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
	
private:
	
	Constants::UAssetSupportType* WorkingAsset = nullptr;
	UEdGraph* WorkingGraph = nullptr;
	
	TSharedPtr<SGraphEditor> WorkingGraphNodeUi = nullptr;
	TSharedPtr<IDetailsView> SelectedNodeDetailsView = nullptr;

	FDelegateHandle GraphChangeListenerHandle;
	
public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject);

	FORCEINLINE Constants::UAssetSupportType* GetWorkingAsset() const { return WorkingAsset; }
	FORCEINLINE UEdGraph* GetWorkingGraph() const { return WorkingGraph; }

	void SetWorkingGraphNodeUi(TSharedPtr<SGraphEditor> NewWorkingGraphNodeUi) { WorkingGraphNodeUi = NewWorkingGraphNodeUi; }
	void SetSelectedNodeDetailView(TSharedPtr<IDetailsView> DetailsView);
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection);

	/// FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetDocumentationLink() const override { return TEXT(""); }

	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override {}
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override {}

	virtual void OnClose() override;

	virtual void SaveAsset_Execute() override;

	void OnNodeDetailsViewPropertiesUpdated(const FPropertyChangedEvent& Event);
	void OnGraphChanced(const FEdGraphEditAction& EditAction);

protected:
	void UpdateWorkingAssetFromGraph();
	void UpdateEditorGraphFromWorkingAsset();
};
