#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class FBiomeAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject);

	/// FAssetEditorToolkit interface

	virtual FName GetToolkitFName() const override { return FName(TEXT("BiomeAssetEditor")); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("CustomAssetEditor")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("BiomeAssetEditor"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FColor(34, 139, 34); }
	virtual FString GetDocumentationLink() const override { return TEXT(""); }

	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override {}
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override {}
};
