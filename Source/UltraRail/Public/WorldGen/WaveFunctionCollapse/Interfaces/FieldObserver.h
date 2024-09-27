#pragma once

#include "CoreMinimal.h"
#include "UltraRail/Public/WorldGen/WaveFunctionCollapse/Structs/CellState.h"
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	void SetupField(UBiomeBlockIDs* Data, int32 Width, int32 Depth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	bool GetCurrentOptimalLocation(int32& X, int32& Y);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	void GetFieldState(TArray<FCellState>& FieldState);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	bool GetCell(int32 X, int32 Y, FCellState& CellState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	bool SetCell(int32 X, int32 Y, FCellState NewCellState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	int32 GetFieldWidth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	int32 GetFieldDepth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	void TranslateIndexToCart(int64 Index, int32& X, int32& Y);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	int64 TranslateIndexFromCart(int32 X, int32 Y);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	bool AreNeighbouring(TScriptInterface<IFieldObserver>& FieldObserver, int32 X1, int32 Y1, int32 X2, int32 Y2);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	void GetNeighbours(int32 X, int32 Y, TArray<FIntVector>& Neighbours);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	bool GetTopNeighbour(int32 X, int32 Y, FCellState& TopNeighbour);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	bool GetRightNeighbour(int32 X, int32 Y, FCellState& RightNeighbour);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	bool GetBottomNeighbour(int32 X, int32 Y, FCellState& BottomNeighbour);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldObserver")
	bool GetLeftNeighbour(int32 X, int32 Y, FCellState& LeftNeighbour);
};
