#include "BiomeAsset/Factories/CustomNodeFactory.h"

#include "BiomeAsset/Nodes/CellConnectionNode.h"
#include "BiomeAsset/Nodes/CellConnectionRulesNode.h"
#include "BiomeAsset/Nodes/CellDefinitionNode.h"
#include "BiomeAsset/Nodes/CellGraphNode.h"

#define CLASSNAME_TEXT(CLASS_NAME) #CLASS_NAME

FCustomNodeFactory::FCustomNodeFactory()
{
}

URuntimeNode* FCustomNodeFactory::CreateRuntimeNode(const FString& NodeName, UObject* Outer)
{
	if (NodeName == CLASSNAME_TEXT(UCellGraphNode))
		return NewObject<UCellDefinitionNode>(Outer);

	if (NodeName == CLASSNAME_TEXT(UCellConnectionNode))
		return NewObject<UCellConnectionRulesNode>(Outer);

	return nullptr;
}

UCustomGraphNode* FCustomNodeFactory::CreateEditorNode(const FName& NodeName, UObject* Outer)
{
	if (NodeName == CLASSNAME_TEXT(UCellGraphNode))
		return NewObject<UCellGraphNode>(Outer);

	if (NodeName == CLASSNAME_TEXT(UCellConnectionNode))
		return NewObject<UCellConnectionNode>(Outer);

	return nullptr;
}
