#pragma once

#include "CoreMinimal.h"
#include "BiomeAsset/Nodes/RuntimeNode.h"

class UCustomGraphNode;
class URuntimeNode;

/**
 * 
 */
class CUSTOMASSETSEDITOR_API FCustomNodeFactory
{
public:	
	FCustomNodeFactory();

	static URuntimeNode* CreateRuntimeNode(const UCustomGraphNode* NodeName, UObject* Outer);
	static UCustomGraphNode* CreateEditorNode(URuntimeNode* NodeName, UObject* Outer);
	
};
