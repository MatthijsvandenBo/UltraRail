#include "BiomeAsset/Apps/BiomeAssetEditorApp.h"

#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Modes/BiomeAssetAppMode.h"
#include "BiomeAsset/Nodes/CustomGraphNode.h"
#include "BiomeAsset/Pins/RuntimePin.h"
#include "BiomeAsset/Schemas/BiomeAssetGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"

void FBiomeAssetEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManagerRef)
{
	FWorkflowCentricApplication::RegisterTabSpawners(TabManagerRef);
}

void FBiomeAssetEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);

	// Initialize the working asset
	WorkingAsset = Cast<Constants::UAssetSupportType>(InObject);

	// Initialize the working graph
	WorkingGraph = FBlueprintEditorUtils::CreateNewGraph(
		WorkingAsset,
		NAME_None,
		UEdGraph::StaticClass(),
		UBiomeAssetGraphSchema::StaticClass()
	);

	InitAssetEditor(Mode,
		InitToolkitHost,
		Constants::ApplicationIdentifier,
		FTabManager::FLayout::NullLayout,
		true,
		true,
		ObjectsToEdit
	);

	AddApplicationMode(Constants::MainModeName, MakeShareable(new FBiomeAssetAppMode(SharedThis(this))));
	SetCurrentMode(Constants::MainModeName);
}

void FBiomeAssetEditorApp::UpdateWorkingAssetFromGraph()
{
	if (WorkingAsset == nullptr || WorkingGraph == nullptr)
		return;

	UBiomeAssetRuntimeGraph* RuntimeGraph = NewObject<UBiomeAssetRuntimeGraph>(WorkingAsset);
	WorkingAsset->Graph = RuntimeGraph;

	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, URuntimePin*> IdToPinMap;

	for (UEdGraphNode* UiNode : WorkingGraph->Nodes)
	{
		// Todo! Save depending on node-type
		// (function mapped to a node-name)
		
		URuntimeNode* RuntimeNode = FCustomNodeFactory::CreateRuntimeNode(UiNode->GetName(), RuntimeGraph);
		RuntimeNode->Position = FVector2D(UiNode->NodePosX, UiNode->NodePosY);
		RuntimeNode->NodeClassName = FName(UiNode->GetName());

		for (UEdGraphPin* UiPin : UiNode->Pins)
		{
			URuntimePin* RuntimePin = NewObject<URuntimePin>(RuntimeNode);
			RuntimePin->PinName = UiPin->PinName;
			RuntimePin->PinId = UiPin->PinId;

			if (UiPin->HasAnyConnections() && UiPin->Direction == EGPD_Output)
			{
				std::pair<FGuid, FGuid> Connection = std::make_pair(UiPin->PinId, UiPin->LinkedTo[0]->PinId);
				Connections.Add(Connection);
			}

			IdToPinMap.Add(UiPin->PinId, RuntimePin);

			if (UiPin->Direction == EGPD_Input)
			{
				RuntimeNode->InputPin = RuntimePin;
			}
			else
			{
				RuntimeNode->OutputPins.Add(RuntimePin);
			}
		}

		RuntimeGraph->Nodes.Add(RuntimeNode);
		RuntimeGraph->GuidMap.Add(RuntimeNode, UiNode->NodeGuid);
	}

	for (const auto& [Pin1Guid, Pin2Guid] : Connections)
	{
		auto* Pin1 = IdToPinMap[Pin1Guid];
		auto* Pin2 = IdToPinMap[Pin2Guid];

		Pin1->Connection = Pin2;
	}
}

void FBiomeAssetEditorApp::UpdateEditorGraphFromWorkingAsset()
{
	if (WorkingAsset->Graph == nullptr)
		return;

	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> IdToPinMap;

	for (auto* RuntimeNode : WorkingAsset->Graph->Nodes)
	{
		
		// Todo! Read depending on Node-name
		// (function map with retrieving data)
		
		// UCustomGraphNode* NewNode = NewObject<UCustomGraphNode>(WorkingGraph);
		UCustomGraphNode* NewNode = FCustomNodeFactory::CreateEditorNode(RuntimeNode->NodeClassName, WorkingGraph);
		NewNode->NodeGuid = WorkingAsset->Graph->GuidMap[RuntimeNode];

		NewNode->NodePosX = RuntimeNode->Position.Y;
		NewNode->NodePosY = RuntimeNode->Position.X;

		if (RuntimeNode->InputPin != nullptr)
		{
			URuntimePin* Pin = RuntimeNode->InputPin;
			UEdGraphPin* UiPin = NewNode->CreateCustomPin(EGPD_Input, Pin->PinName, Constants::CustomPinSubCategory);
			UiPin->PinId = Pin->PinId;
		}
	}
}

FName FBiomeAssetEditorApp::GetToolkitFName() const
{
	return Constants::CategoryKey;
}

FText FBiomeAssetEditorApp::GetBaseToolkitName() const
{
	return FText::FromName(Constants::CategoryKey);
}

FString FBiomeAssetEditorApp::GetWorldCentricTabPrefix() const
{
	return Constants::CategoryKey.ToString();
}

FLinearColor FBiomeAssetEditorApp::GetWorldCentricTabColorScale() const
{
	return Constants::AssetDisplayColor;
}
