#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FBiomeAssetAction : public FAssetTypeActions_Base
{
private:
	EAssetTypeCategories::Type AssetCategory;

public:
	FBiomeAssetAction(EAssetTypeCategories::Type Category);

	/// FAssetTypeActions_Base interface

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects,
	                             TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;
};
