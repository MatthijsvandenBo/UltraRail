#include "BiomeAsset/Apps/BiomeAssetEditorApp.h"

#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Modes/BiomeAssetAppMode.h"
#include "BiomeAsset/Nodes/CellConnectionNode.h"
#include "BiomeAsset/Nodes/CellDefinitionNode.h"
#include "BiomeAsset/Nodes/RuntimeCellDefinitionNode.h"
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

	UpdateEditorGraphFromWorkingAsset();
	GraphChangeListenerHandle = WorkingGraph->AddOnGraphChangedHandler(
		FOnGraphChanged::FDelegate::CreateSP(this, &FBiomeAssetEditorApp::OnGraphChanced)
	);
}

void FBiomeAssetEditorApp::SetSelectedNodeDetailView(TSharedPtr<IDetailsView> DetailsView)
{
	SelectedNodeDetailsView = DetailsView;
	SelectedNodeDetailsView->OnFinishedChangingProperties().AddRaw(this, &FBiomeAssetEditorApp::OnNodeDetailsViewPropertiesUpdated);
}

void FBiomeAssetEditorApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection)
{
	// TODO! Left at 19:30

	// Find the first custom cell if any
	for (const auto Node : Selection)
	{
		const UCustomGraphNode* CustomGraphNode = Cast<UCustomGraphNode>(Node);
		if (CustomGraphNode != nullptr)
		{
			SelectedNodeDetailsView->SetObject(CustomGraphNode->GetNodeInfo());
			return;
		}
	}

	// If none found, explicitly set it to a nullptr
	SelectedNodeDetailsView->SetObject(nullptr);
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

void FBiomeAssetEditorApp::OnClose()
{
	UpdateWorkingAssetFromGraph();
	WorkingGraph->RemoveOnGraphChangedHandler(GraphChangeListenerHandle);
	FWorkflowCentricApplication::OnClose();
}

void FBiomeAssetEditorApp::SaveAsset_Execute()
{
	FWorkflowCentricApplication::SaveAsset_Execute();
	UpdateWorkingAssetFromGraph();
}

void FBiomeAssetEditorApp::OnNodeDetailsViewPropertiesUpdated(const FPropertyChangedEvent& Event)
{
	if (WorkingGraphNodeUi != nullptr)
		WorkingGraphNodeUi->NotifyGraphChanged();
}

void FBiomeAssetEditorApp::OnGraphChanced(const FEdGraphEditAction& EditAction)
{

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
		const UCustomGraphNode* CustomUiNode = Cast<UCustomGraphNode>(UiNode);
		if (CustomUiNode == nullptr)
			continue;
		
		URuntimeNode* RuntimeNode = FCustomNodeFactory::CreateRuntimeNode(CustomUiNode, RuntimeGraph);
		if (RuntimeNode == nullptr)
			continue;
		
		RuntimeNode->Position = FVector2D(UiNode->NodePosX, UiNode->NodePosY);

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
		UCustomGraphNode* UiNode = FCustomNodeFactory::CreateEditorNode(RuntimeNode, WorkingGraph);
		if (UiNode == nullptr)
			continue;
		
		UiNode->NodeGuid = WorkingAsset->Graph->GuidMap[RuntimeNode];

		UiNode->NodePosX = RuntimeNode->Position.X;
		UiNode->NodePosY = RuntimeNode->Position.Y;

		if (RuntimeNode->InputPin != nullptr)
		{
			URuntimePin* Pin = RuntimeNode->InputPin;
			UEdGraphPin* UiPin = UiNode->CreateCustomPin(EGPD_Input, Pin->PinName, Constants::CustomPinSubCategory);
			UiPin->PinId = Pin->PinId;

			if (Pin->Connection != nullptr)
			{
				Connections.Add(std::make_pair(Pin->PinId, Pin->Connection->PinId));
			}

			IdToPinMap.Add(Pin->PinId, UiPin);
		}

		for (URuntimePin* RuntimePin : RuntimeNode->OutputPins)
		{
			UEdGraphPin* UiPin = UiNode->CreateCustomPin(EGPD_Output, RuntimePin->PinName, Constants::CustomPinSubCategory);
			UiPin->PinId = RuntimePin->PinId;

			if (RuntimePin->Connection != nullptr)
			{
				Connections.Add(std::make_pair(RuntimePin->PinId, RuntimePin->Connection->PinId));
			}

			IdToPinMap.Add(RuntimePin->PinId, UiPin);
		}

		WorkingGraph->AddNode(UiNode, true, true);
	}

	for (const auto& [FromId, ToId] : Connections)
	{
		UEdGraphPin* FromPin = IdToPinMap[FromId];
		UEdGraphPin* ToPin = IdToPinMap[ToId];

		FromPin->LinkedTo.Add(ToPin);
		ToPin->LinkedTo.Add(FromPin);
	}
}