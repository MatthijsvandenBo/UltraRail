#include "BiomeAsset/Factories/BiomeAssetFactory.h"

#include "BiomeAsset/Constants.h"

UBiomeAssetFactory::UBiomeAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = Constants::UAssetSupportType::StaticClass();
}

UObject* UBiomeAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
                                              UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<Constants::UAssetSupportType>(InParent, Name, Flags);
}

bool UBiomeAssetFactory::CanCreateNew() const
{
	return true;
}
