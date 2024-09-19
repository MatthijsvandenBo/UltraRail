#pragma once

#include "CoreMinimal.h"
#include "Structs/BlockIdConnection.h"
#include "Structs/CellState.h"
#include "UObject/Interface.h"
#include "FieldObserver.generated.h"

class UBiomeBlockIDs;

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
	void SetupField(UBiomeBlockIDs* Data);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetCurrentOptimalLocation(int32& X, int32& Y);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetFieldState(TArray<FCellState>& FieldState);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetCell(int32 X, int32 Y, FCellState& CellState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetCell(int32 X, int32 Y, FCellState NewCellState);

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
	void GetNeighbours(int32 X, int32 Y, TArray<FIntVector>& Neighbours);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetTopNeighbour(int32 X, int32 Y, FCellState& TopNeighbour);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetRightNeighbour(int32 X, int32 Y, FCellState& RightNeighbour);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetBottomNeighbour(int32 X, int32 Y, FCellState& BottomNeighbour);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetLeftNeighbour(int32 X, int32 Y, FCellState& LeftNeighbour);
};
