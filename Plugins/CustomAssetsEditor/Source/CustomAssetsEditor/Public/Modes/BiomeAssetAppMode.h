#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class FBiomeAssetEditorApp;

/**
 * 
 */
class FBiomeAssetAppMode : public FApplicationMode
{
private:
	TWeakPtr<FBiomeAssetEditorApp> App;
	FWorkflowAllowedTabSet Tabs;
	
public:
	FBiomeAssetAppMode(TSharedPtr<FBiomeAssetEditorApp> App);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;
};
