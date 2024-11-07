#include "BiomeAsset/Factories/CustomNodeFactory.h"

#include "BiomeAsset/Nodes/CellConnectionNode.h"
#include "BiomeAsset/Nodes/RuntimeCellConnectionNode.h"
#include "BiomeAsset/Nodes/RuntimeCellDefinitionNode.h"
#include "BiomeAsset/Nodes/CellGraphNode.h"

#define CLASSNAME_TEXT(CLASS_NAME) #CLASS_NAME

FCustomNodeFactory::FCustomNodeFactory()
{
}

URuntimeNode* FCustomNodeFactory::CreateRuntimeNode(const FString& NodeName, UObject* Outer)
{
	if (NodeName == CLASSNAME_TEXT(CellGraphNode))
		return NewObject<URuntimeCellDefinitionNode>(Outer);

	if (NodeName == CLASSNAME_TEXT(CellConnectionNode))
		return NewObject<URuntimeCellConnectionNode>(Outer);
	
	return nullptr;
}

UCustomGraphNode* FCustomNodeFactory::CreateEditorNode(const FName& NodeName, UObject* Outer)
{
	if (NodeName == CLASSNAME_TEXT(CellGraphNode))
		return NewObject<UCellGraphNode>(Outer);

	if (NodeName == CLASSNAME_TEXT(CellConnectionNode))
		return NewObject<UCellConnectionNode>(Outer);

	return nullptr;
}
