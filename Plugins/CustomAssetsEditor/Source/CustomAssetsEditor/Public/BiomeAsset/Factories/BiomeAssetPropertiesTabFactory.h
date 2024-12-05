/**
* https://forums.unrealengine.com/t/tutorial-slate-slistview-example/46308
*/

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FBiomeAssetEditorApp;

/**
 * 
 */
class FBiomeAssetPropertiesTabFactory : public FWorkflowTabFactory
{
private:
	TWeakPtr<FBiomeAssetEditorApp> App;
	
public:
	FBiomeAssetPropertiesTabFactory(TSharedPtr<FBiomeAssetEditorApp> InApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
};
