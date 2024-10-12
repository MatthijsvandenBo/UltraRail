#pragma once

#include "CoreMinimal.h"
#include "BiomeAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class UBiomeAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UBiomeAssetFactory(const FObjectInitializer& ObjectInitializer);

	/// UFactory interface

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
};
