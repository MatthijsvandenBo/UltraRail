#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BlockStateObserver.generated.h"

class IFieldObserver;

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UBlockStateObserver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ULTRARAIL_API IBlockStateObserver
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ObserveBlock(TScriptInterface<IFieldObserver>& FieldObserver, int32 X, int32 Y);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetLastObserved(int32& X, int32& Y);
};
