#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CellStateObserver.generated.h"

class IFieldObserver;

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UCellStateObserver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ULTRARAIL_API ICellStateObserver
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CellStateObserver")
	void ObserveCell(UObject* FieldObserver, int32 X, int32 Y);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CellStateObserver")
	void GetLastObserved(int32& X, int32& Y);
};
