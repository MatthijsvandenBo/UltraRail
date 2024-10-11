#include "Factories/BiomeAssetFactory.h"

#include "Assets/BiomeAsset.h"

UBiomeAssetFactory::UBiomeAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UBiomeAsset::StaticClass();
}

UObject* UBiomeAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
                                              UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UBiomeAsset>(InParent, Name, Flags);
}

bool UBiomeAssetFactory::CanCreateNew() const
{
	return true;
}
