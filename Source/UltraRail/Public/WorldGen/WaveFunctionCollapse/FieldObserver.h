#pragma once

#include "CoreMinimal.h"
#include "Structs/BlockIdConnection.h"
#include "Structs/BlockState.h"
#include "UObject/Interface.h"
#include "FieldObserver.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFieldObserver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ULTRARAIL_API IFieldObserver
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetCurrentOptimalLocation(int32& X, int32& Y);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetFieldState(TArray<FBlockState>& FieldState);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetItem(int32 X, int32 Y, FBlockState& BlockState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetFieldWidth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetFieldDepth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TranslateIndexToCart(int64 Index, int32& X, int32& Y);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int64 TranslateIndexFromCart(int32 X, int32 Y);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool AreNeighbouring(TScriptInterface<IFieldObserver>& FieldObserver, int32 X1, int32 Y1, int32 X2, int32 Y2);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetNeighbours(int32 X, int32 Y, TArray<FBlockState>& Neighbours);
};
