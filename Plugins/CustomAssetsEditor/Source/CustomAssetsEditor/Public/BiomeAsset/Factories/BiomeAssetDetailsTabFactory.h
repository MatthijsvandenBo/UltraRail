#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FBiomeAssetEditorApp;

/**
 * 
 */
class FBiomeAssetDetailsTabFactory : public FWorkflowTabFactory
{
private:
	TWeakPtr<FBiomeAssetEditorApp> App;
	
public:
	FBiomeAssetDetailsTabFactory(TSharedPtr<FBiomeAssetEditorApp> InApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
};
