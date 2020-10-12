// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#include "RoadRunnerImporter.h"
#include "RoadRunnerImporterLog.h"
#include "RoadRunnerImporterOptions.h"
#include "RoadRunnerStyle.h"
#include "RoadRunnerTrafficJunction.h"
#include "RoadRunnerFbxSceneImportFactory.h"
#include "RoadRunnerImportUtil.h"

#include <AssetRegistryModule.h>
#include <Developer/AssetTools/Public/AssetToolsModule.h>
#include <Developer/AssetTools/Public/IAssetTools.h>
#include <Editor/MaterialEditor/Public/IMaterialEditor.h>
#include <Editor/UnrealEd/Public/Editor.h>
#include <Editor/UnrealEd/Public/Kismet2/KismetEditorUtilities.h>
#include <Editor/UnrealEd/Public/Layers/ILayers.h>
#include <Factories/Factory.h>
#include <Factories/FbxFactory.h>
#include <unordered_map>
#include <ObjectTools.h>
#include <PackageTools.h>
#include <Runtime/AssetRegistry/Public/ARFilter.h>
#include <Runtime/Core/Public/Internationalization/Regex.h>
#include <Runtime/Launch/Resources/Version.h>
#include <Runtime/XmlParser/Public/XmlFile.h>
#include <UnrealEd.h>
#include <UObject/GCObjectScopeGuard.h>

#include "RoadRunnerReimportFactory.h"

#if ENGINE_MINOR_VERSION > 21
#include <Subsystems/ImportSubsystem.h>
#endif


#define LOCTEXT_NAMESPACE "FRoadRunnerImporterModule"

namespace
{
	static const FString RoadRunnerExtension = ".rrdata.xml";

	struct FStringHash
	{
		std::size_t operator()(const FString& k) const
		{
			return GetTypeHash(k);
		}
	};

	////////////////////////////////////////////////////////////////////////////////
	// Get the modifed light bulb name by its original name under a given scene
	// component node.
	FString FindByNamePrefix(USCS_Node* parent, const FString& prefix)
	{
		for (const auto& child : parent->GetChildNodes())
		{
			FString componentName = child->GetVariableName().ToString();

			FString regex = FString(TEXT("^"));
			regex += prefix;
			regex += FString(TEXT("(_[0-9]+)?Node.*"));
			FRegexPattern pattern(regex);

			FRegexMatcher matcher(pattern, componentName);

			// Check if has match
			if (matcher.FindNext())
			{
				return componentName;
			}

			FString childCheck = FindByNamePrefix(child, prefix);

			if (!childCheck.IsEmpty())
			{
				return childCheck;
			}
		}
		return TEXT("");
	}

	////////////////////////////////////////////////////////////////////////////////
	// Replaces the created Unreal material with a material instance from the
	// RoadRunnerMaterials plugin
	void CreateRoadRunnerMaterial(FString sourceFilePath, FString packagePath, FRoadRunnerImportUtil::MaterialInfo materialInfo, int metadataVersion, TMap<FString, int32>& materialToLayerMap)
	{
		FString materialFullName = materialInfo.Name;
		FString basePackageName = FPackageName::GetLongPackagePath(packagePath) / materialFullName;
		basePackageName = PackageTools::SanitizePackageName(basePackageName);

		// Create new package
		const FString Suffix(TEXT(""));

		// Re-create package for new material instance
		UPackage* package = CreatePackage(NULL, *basePackageName);

		// Find our base material to instance from
		FText materialSearchError;
		UMaterialInterface* baseMaterial = nullptr;
		float alphaVal = 1.0f; // Default to opaque
		float transparency = 0.0f;
		if (!materialInfo.TransparencyFactor.IsEmpty())
		{
			transparency = FCString::Atof(*(materialInfo.TransparencyFactor));
		}
		bool isTransparent = !materialInfo.TransparencyMap.IsEmpty() || transparency > 0;


		FString materialName = "BaseMaterial";
		alphaVal = 1.0f - transparency;
		// Version 1 adds DrawQueue and ShadowCaster fields
		if (metadataVersion >= 1)
		{
			// Decals and markings always use decal material
			if (materialInfo.IsDecal.Equals("true") || !materialInfo.DrawQueue.Equals("0"))
			{
				materialName = "BaseDecalMaterial";
			}
			else if (isTransparent) // Choose base material based off transparency
			{
				if (materialInfo.TwoSided.Equals("true"))
				{
					if (materialInfo.ShadowCaster.Equals("false"))
					{
						// Translucent blend mode
						materialName = "BaseTransparentMaterialTwoSided";
					}
					else
					{
						// Masked blend mode
						materialName = "BaseCutoutMaterialTwoSided";
					}
				}
				else
				{
					if (materialInfo.ShadowCaster.Equals("false"))
					{
						// Translucent blend mode
						materialName = "BaseTransparentMaterial";
					}
					else
					{
						// Masked blend mode
						materialName = "BaseCutoutMaterial";
					}
				}
			}
		}
		else // Backwards compatibility
		{
			// Choose base material based off transparency
			if (isTransparent)
			{
				if (materialInfo.TwoSided.Equals("true"))
				{
					if (transparency > 0.0f)
					{
						// Translucent blend mode
						materialName = "BaseTransparentMaterialTwoSided";
					}
					else
					{
						// Masked blend mode
						materialName = "BaseCutoutMaterialTwoSided";
					}
				}
				else
				{
					if (transparency > 0.0f)
					{
						// Translucent blend mode
						materialName = "BaseTransparentMaterial";
					}
					else
					{
						// Masked blend mode
						materialName = "BaseCutoutMaterial";
					}
				}
			}
		}

		baseMaterial = UMaterialImportHelpers::FindExistingMaterialFromSearchLocation(materialName, "/RoadRunnerMaterials/", EMaterialSearchLocation::UnderParent, materialSearchError);

		if (!materialSearchError.IsEmpty() || baseMaterial == nullptr)
		{
			UE_LOG(RoadRunnerImporter, Error, TEXT("Base material not found: %s"), *(materialSearchError.ToString()));
			return;
		}

		// Create material instance from our base material
		// Modified: always create a material instance from our base material
		auto materialInstanceFactory = NewObject<UMaterialInstanceConstantFactoryNew>();
		materialInstanceFactory->InitialParent = baseMaterial;
		UMaterialInstanceConstant* unrealMaterial = (UMaterialInstanceConstant*)materialInstanceFactory->FactoryCreateNew(UMaterialInstanceConstant::StaticClass(), package, *materialFullName, RF_Standalone | RF_Public, NULL, GWarn);
		if (unrealMaterial == NULL)
		{
			UE_LOG(RoadRunnerImporter, Error, TEXT("Material %s could not be created."), *materialInfo.Name);
			return;
		}

		// Notify the asset registry
		FAssetRegistryModule::AssetCreated(unrealMaterial);
		// Set the dirty flag so this package will get saved later
		package->SetDirtyFlag(true);

		// Set parameters
		// Modified: set parameters based off imported material info
		// Note: RoadRunner is in the middle of converting to a roughness-metallic setup, so some material may not look right
		FRoadRunnerImportUtil::SetTextureParameter(unrealMaterial, FName(TEXT("DiffuseMap")), sourceFilePath, materialInfo.DiffuseMap, packagePath);
		FRoadRunnerImportUtil::SetTextureParameter(unrealMaterial, FName(TEXT("NormalMap")), sourceFilePath, materialInfo.NormalMap, packagePath);
		FRoadRunnerImportUtil::SetTextureParameter(unrealMaterial, FName(TEXT("RoughnessMap")), sourceFilePath, materialInfo.SpecularMap, packagePath);

		FRoadRunnerImportUtil::SetColorParameter(unrealMaterial, FName(TEXT("DiffuseColor")), materialInfo.DiffuseColor, alphaVal);

		// Note: The default RoadRunner materials have not been tuned to the new setup yet
		// FRoadRunnerImportUtil::SetScalarParameter(unrealMaterial, FName(TEXT("RoughnessFactor")), materialInfo.Roughness); 
		FRoadRunnerImportUtil::SetScalarParameter(unrealMaterial, FName(TEXT("Emission")), materialInfo.Emission);
		FRoadRunnerImportUtil::SetScalarParameter(unrealMaterial, FName(TEXT("ScalingU")), materialInfo.TextureScaleU);
		FRoadRunnerImportUtil::SetScalarParameter(unrealMaterial, FName(TEXT("ScalingV")), materialInfo.TextureScaleV);

		// let the material update itself if necessary
		unrealMaterial->PreEditChange(nullptr);
		unrealMaterial->PostEditChange();

		int32 adjustedDrawQueue = FCString::Atoi(*materialInfo.DrawQueue);
		if (isTransparent && materialInfo.IsDecal.Equals("false"))
		{
			// Render other transparent objects after decals
			adjustedDrawQueue = FRoadRunnerImporterModule::TransparentRenderQueue;
		}
		materialToLayerMap.Add(materialFullName, adjustedDrawQueue);
	}

	////////////////////////////////////////////////////////////////////////////////
	// Parses "LightState" element. Contains "Name" and "State".
	FLightBulbState LoadLightBulbState(FXmlNode* lightStateNode)
	{
		FLightBulbState lightBulbState;
		for (const auto& lightStateProperty : lightStateNode->GetChildrenNodes())
		{
			const FString& tag = lightStateProperty->GetTag();
			if (tag.Equals(TEXT("Name"), ESearchCase::CaseSensitive))
			{
				lightBulbState.Name = lightStateProperty->GetContent();
			}
			else if (tag.Equals(TEXT("State"), ESearchCase::CaseSensitive))
			{
				FString stateString = lightStateProperty->GetContent();
				lightBulbState.State = stateString.Equals(TEXT("true"), ESearchCase::CaseSensitive);
			}

		}
		return lightBulbState;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Parses "Configuration" element. Contains "Name" and multiple
	// "LightState" elements.
	FSignalConfiguration LoadSignalConfiguration(FXmlNode* configurationNode)
	{
		FSignalConfiguration signalConfiguration;
		for (const auto& signalConfigurationProperty : configurationNode->GetChildrenNodes())
		{
			const FString& tag = signalConfigurationProperty->GetTag();
			if (tag.Equals(TEXT("Name"), ESearchCase::CaseSensitive))
			{
				signalConfiguration.Name = signalConfigurationProperty->GetContent();
			}
			else if (tag.Equals(TEXT("LightState"), ESearchCase::CaseSensitive))
			{
				FLightBulbState lightState = LoadLightBulbState(signalConfigurationProperty);
				signalConfiguration.LightBulbStates.Add(lightState);
			}
		}

		return signalConfiguration;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Parses "Signal" asset element. Contains "ID" and multiple
	// "Configuration" elements.
	FSignalAsset LoadSignalAsset(FXmlNode* signalNode)
	{
		FSignalAsset signalAsset;
		for (const auto& signalAssetProperty : signalNode->GetChildrenNodes())
		{
			const FString& tag = signalAssetProperty->GetTag();
			if (tag.Equals(TEXT("ID"), ESearchCase::CaseSensitive))
			{
				signalAsset.Id = signalAssetProperty->GetContent();
			}
			else if (tag.Equals(TEXT("Configuration"), ESearchCase::CaseSensitive))
			{
				FSignalConfiguration signalConfiguration = LoadSignalConfiguration(signalAssetProperty);
				signalAsset.SignalConfigurations.Add(signalConfiguration);
			}
		}
		return signalAsset;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Load the signal assets into the array.
	void LoadSignalAssets(FXmlNode* signalConfigNode, std::unordered_map<FString, FSignalAsset, FStringHash>& outUuidToSignalAssetMap)
	{
		for (const auto& signalAssetsProperty : signalConfigNode->GetChildrenNodes())
		{
			const FString& tag = signalAssetsProperty->GetTag();
			if (tag.Equals(TEXT("Signal"), ESearchCase::CaseSensitive))
			{
				FSignalAsset signalAsset = LoadSignalAsset(signalAssetsProperty);
				outUuidToSignalAssetMap[signalAsset.Id] = signalAsset;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// Parses "Signal" state element. Contains the "ID" of the signal, the
	// "SignalAsset" ID defined in SignalAssets, and the "State" for which
	// configuration it is currently in.
	FSignalState LoadSignalState(FXmlNode* signalStateNode, const std::unordered_map<FString, USCS_Node*, FStringHash>& uuidToComponentMap, const std::unordered_map<FString, FSignalAsset, FStringHash>& uuidToSignalAssetMap)
	{
		FSignalState signalState;
		for (const auto& signalStateProperty : signalStateNode->GetChildrenNodes())
		{
			const FString& tag = signalStateProperty->GetTag();
			if (tag.Equals(TEXT("ID"), ESearchCase::CaseSensitive))
			{
				signalState.Id = signalStateProperty->GetContent();
			}
			else if (tag.Equals(TEXT("SignalAsset"), ESearchCase::CaseSensitive))
			{
				signalState.SignalAssetId = signalStateProperty->GetContent();
			}
			else if (tag.Equals(TEXT("ConfigurationIndex"), ESearchCase::CaseSensitive))
			{
				signalState.Configuration = FCString::Atoi(*signalStateProperty->GetContent());
			}
		}

		// Find the signal configuration by the id
		if (uuidToSignalAssetMap.count(signalState.SignalAssetId) == 0)
		{
			UE_LOG(RoadRunnerImporter, Warning, TEXT("Signal Asset %s could not be found."), *signalState.SignalAssetId);
			return signalState;
		}
		const FSignalAsset& signalAsset = uuidToSignalAssetMap.at(signalState.SignalAssetId);

		if (signalState.Configuration >= signalAsset.SignalConfigurations.Num() || signalState.Configuration < 0)
		{
			UE_LOG(RoadRunnerImporter, Warning, TEXT("Signal Configuration for %s out of range."), *signalState.Id);
			return signalState;
		}
		const FSignalConfiguration& signalConfiguration = signalAsset.SignalConfigurations[signalState.Configuration];

		// Loop over light bulb states and set up the light instance states
		for (const auto& lightBulbState : signalConfiguration.LightBulbStates)
		{
			if (uuidToComponentMap.count(signalState.Id) == 0)
			{
				UE_LOG(RoadRunnerImporter, Warning, TEXT("Signal %s not found inside this blueprint."), *signalState.Id);
				continue;
			}
			FLightInstanceState lightInstanceState;
			lightInstanceState.ComponentName = FindByNamePrefix(uuidToComponentMap.at(signalState.Id), lightBulbState.Name);
			lightInstanceState.State = lightBulbState.State;
			signalState.LightInstanceStates.Add(lightInstanceState);
		}

		return signalState;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Parses "Interval" element. Contains the "Time" of its duration, and multiple
	// "Signal" states.
	FLightInterval LoadInterval(FXmlNode* intervalNode, const std::unordered_map<FString, USCS_Node*, FStringHash>& uuidToComponentMap, const std::unordered_map<FString, FSignalAsset, FStringHash>& uuidToSignalAssetMap)
	{
		FLightInterval interval;
		for (const auto& intervalProperty : intervalNode->GetChildrenNodes())
		{
			const FString& tag = intervalProperty->GetTag();
			if (tag.Equals(TEXT("Time"), ESearchCase::CaseSensitive))
			{
				interval.Time = FCString::Atof(*intervalProperty->GetContent());
			}
			else if (tag.Equals(TEXT("Signal"), ESearchCase::CaseSensitive))
			{
				FSignalState signalState = LoadSignalState(intervalProperty, uuidToComponentMap, uuidToSignalAssetMap);
				interval.SignalStates.Add(signalState);
			}
		}
		return interval;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Parses "SignalPhase" element. Contains multiple "Interval" elements.
	FSignalPhase LoadSignalPhase(FXmlNode* signalPhaseNode, const std::unordered_map<FString, USCS_Node*, FStringHash>& uuidToComponentMap, const std::unordered_map<FString, FSignalAsset, FStringHash>& uuidToSignalAssetMap)
	{
		FSignalPhase signalPhase;
		for (const auto& signalPhaseProperty : signalPhaseNode->GetChildrenNodes())
		{
			const FString& tag = signalPhaseProperty->GetTag();
			if (tag.Equals(TEXT("Interval"), ESearchCase::CaseSensitive))
			{
				FLightInterval interval = LoadInterval(signalPhaseProperty, uuidToComponentMap, uuidToSignalAssetMap);
				signalPhase.Intervals.Add(interval);
			}
		}
		return signalPhase;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Parses "Junction" element. Contains its "ID" and multiple "SignalPhase" elements
	FJunction LoadJunction(FXmlNode* junctionNode, const std::unordered_map<FString, USCS_Node*, FStringHash>& uuidToComponentMap, const std::unordered_map<FString, FSignalAsset, FStringHash>& uuidToSignalAssetMap)
	{
		FJunction junction;
		for (const auto& junctionProperty : junctionNode->GetChildrenNodes())
		{
			const FString& tag = junctionProperty->GetTag();
			if (tag.Equals(TEXT("ID"), ESearchCase::CaseSensitive))
			{
				junction.Id = junctionProperty->GetContent();
			}
			else if (tag.Equals(TEXT("SignalPhase"), ESearchCase::CaseSensitive))
			{
				FSignalPhase signalPhase = LoadSignalPhase(junctionProperty, uuidToComponentMap, uuidToSignalAssetMap);
				junction.SignalPhases.Add(signalPhase);
			}

		}
		return junction;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Load the Junctions into the array.
	TArray<FJunction> LoadSignalJunctions(FXmlNode* xmlSignalData, const std::unordered_map<FString, USCS_Node*, FStringHash>& uuidToComponentMap, const std::unordered_map<FString, FSignalAsset, FStringHash>& uuidToSignalAssetMap)
	{
		TArray<FJunction> ret = TArray<FJunction>();

		if (!xmlSignalData)
			return ret;

		// "SignalData" has multiple Junction elements under it
		for (const auto& signalDataProperty : xmlSignalData->GetChildrenNodes())
		{
			const FString& tag = signalDataProperty->GetTag();
			if (tag.Equals(TEXT("Junction"), ESearchCase::CaseSensitive))
			{
				FJunction junction = LoadJunction(signalDataProperty, uuidToComponentMap, uuidToSignalAssetMap);
				ret.Add(junction);
			}
		}

		return ret;
	}
}

////////////////////////////////////////////////////////////////////////////////
// If fbx was imported through our scene importer, re-import the materials using
// the metadata file, then parse the signal metadata and attach traffic
// junctions components to the newly created blueprint.
void FRoadRunnerImporterModule::RoadRunnerPostProcessing(UFactory* inFactory, UObject* inCreateObject)
{
	if (!inCreateObject)
	{
		return;
	}
	if (!inFactory->IsA<URoadRunnerFbxSceneImportFactory>())
	{
		return;
	}
	auto roadrunnerFactory = Cast<URoadRunnerFbxSceneImportFactory>(inFactory);
	
	FString srcPath = FPaths::GetPath(*inFactory->GetCurrentFilename());
	FString packagePath = FPaths::GetPath(inCreateObject->GetPathName()) + "/";

	FString rrMetadataFile = FPaths::ChangeExtension(inFactory->GetCurrentFilename(), RoadRunnerExtension);
	if (!FPaths::FileExists(rrMetadataFile))
	{
		return;
	}

	FXmlFile* rrXml = new FXmlFile(rrMetadataFile);
	if (!rrXml->IsValid())
	{
		UE_LOG(RoadRunnerImporter, Error, TEXT("Metadata XML is invalid in: %s"), *(rrMetadataFile));
		return;
	}
	FXmlNode* xmlRoot = rrXml->GetRootNode();

	int metadataVersion = FCString::Atoi(*(xmlRoot->GetAttribute("Version")));

	if (roadrunnerFactory->RoadRunnerImportOptions->OverwriteMaterials)
	{
		FXmlNode * xmlMatList = xmlRoot->FindChildNode("MaterialList");
		if (!xmlMatList)
		{
			UE_LOG(RoadRunnerImporter, Error, TEXT("Material List not found in metadata: %s"), *(rrMetadataFile));
			return;
		}

		TMap<FString, int32> materialToLayerMap;

		auto matList = FRoadRunnerImportUtil::ParseMaterialXml(xmlMatList);

		// Delete old materials first
		TArray<UObject*> objectsToForceDelete;
		for (const auto& matInfo : matList)
		{
			FString materialFullName = matInfo.Name;
			FString basePackageName = FPackageName::GetLongPackagePath(packagePath) / materialFullName;
			basePackageName = PackageTools::SanitizePackageName(basePackageName);
			// Get the original material package and delete it
			UMaterial* oldMat = LoadObject<UMaterial>(nullptr, *basePackageName);
			if (oldMat != nullptr)
			{
				objectsToForceDelete.Add(oldMat);
			}
		}
		ObjectTools::ForceDeleteObjects(objectsToForceDelete, false);

		// Create RoadRunner materials
		for (const auto& matInfo : matList)
		{
			CreateRoadRunnerMaterial(srcPath, packagePath, matInfo, metadataVersion, materialToLayerMap);
		}

		// Blueprint specific setup
		UBlueprint* blueprint = Cast<UBlueprint>(inCreateObject);
		if (!blueprint)
		{
			return;
		}
		blueprint->AddToRoot(); // Protect from garbage collection

		// Set sort priority in blueprint
		for (const auto& uscsnode : blueprint->SimpleConstructionScript->GetAllNodes())
		{
			if (!uscsnode->ComponentTemplate->IsA<UStaticMeshComponent>())
			{
				continue;
			}
			UStaticMeshComponent* staticMeshComponent = Cast<UStaticMeshComponent>(uscsnode->ComponentTemplate);
			auto slotNames = staticMeshComponent->GetMaterialSlotNames();
			// check mesh has materials
			if (slotNames.Num() <= 0)
			{
				continue;
			}

			// just get the first material to find the layer
			FName matName = slotNames[0];
			FString materialFullName = matName.ToString();
			// Follow Unreal's naming scheme
			materialFullName = FRoadRunnerImportUtil::ConvertMaterialName(materialFullName);

			int32* drawQueue = materialToLayerMap.Find(materialFullName);
			if (drawQueue != nullptr)
			{
				staticMeshComponent->SetTranslucentSortPriority(*drawQueue);
			}
		}
		blueprint->RemoveFromRoot();
	}

	if (roadrunnerFactory->RoadRunnerImportOptions->ImportSignals)
	{
		FXmlNode * xmlSignalConfigs = nullptr;
		FXmlNode * xmlJunctionData = nullptr;

		// Prior to version 3, both signal configurations and junction data were stored in the same element
		if (metadataVersion < 3)
		{
			if (FXmlNode * xmlSignalData = xmlRoot->FindChildNode("SignalData"))
			{
				xmlSignalConfigs = xmlSignalData->FindChildNode(TEXT("SignalAssets"));
				xmlJunctionData = xmlSignalData;
			}
		}
		else
		{
			xmlSignalConfigs = xmlRoot->FindChildNode("SignalConfigurations");
			xmlJunctionData = xmlRoot->FindChildNode("Signalization");
		}
		
		if (xmlSignalConfigs && xmlJunctionData)
		{
			std::unordered_map<FString, USCS_Node*, FStringHash> uuidToComponentMap;
			const FRegexPattern uuidPattern(TEXT("^[{(]?[0-9A-Fa-f]{8}[-]?([0-9A-Fa-f]{4}[-]?){3}[0-9A-Fa-f]{12}[)}]?"));

			// Blueprint specific setup
			UBlueprint* blueprint = Cast<UBlueprint>(inCreateObject);
			if (!blueprint)
			{
				return;
			}
			blueprint->AddToRoot(); // Protect from garbage collection

			// Create uuid to component map
			for (const auto& uscsnode : blueprint->SimpleConstructionScript->GetAllNodes())
			{
				FString nodeName = uscsnode->GetVariableName().ToString();

				FRegexMatcher matcher(uuidPattern, nodeName);

				// Need to check name for uuid, then add uscs_node to a map
				if (matcher.FindNext())
				{
					// Only check first match
					FString match = matcher.GetCaptureGroup(0);

					if (uscsnode->GetChildNodes().Num() != 0)
					{
						uuidToComponentMap[match] = (uscsnode->GetChildNodes())[0];
					}
				}
			}

			std::unordered_map<FString, FSignalAsset, FStringHash> uuidToSignalAssetMap;

			// Create map of signal assets from metadata
			LoadSignalAssets(xmlSignalConfigs, uuidToSignalAssetMap);

			// Parse junction data from xml
			TArray<FJunction> junctions = LoadSignalJunctions(xmlJunctionData, uuidToComponentMap, uuidToSignalAssetMap);

			// Add to blueprint
			AActor* dummyActor = NewObject<AActor>();
			dummyActor->AddToRoot();

			TArray<UActorComponent*> newComponents;
			for (const auto& junction : junctions)
			{
				URoadRunnerTrafficJunction* component = NewObject<URoadRunnerTrafficJunction>(dummyActor);
				component->AddToRoot();
				component->SetPhases(junction);
				newComponents.Add(component);
			}

			FKismetEditorUtilities::AddComponentsToBlueprint(blueprint, newComponents, false, (USCS_Node*)nullptr, true);

			FKismetEditorUtilities::CompileBlueprint(blueprint);
			blueprint->MarkPackageDirty();
			blueprint->PreEditChange(nullptr);
			blueprint->PostEditChange();
			dummyActor->RemoveFromRoot();
			for (const auto& comp : newComponents)
			{
				comp->RemoveFromRoot();
			}

			blueprint->RemoveFromRoot();
		}
	}

	// If any modifications were made
	if (roadrunnerFactory->RoadRunnerImportOptions->OverwriteMaterials
		|| roadrunnerFactory->RoadRunnerImportOptions->ImportSignals)
	{
		// Blueprint specific setup
		UBlueprint* blueprint = Cast<UBlueprint>(inCreateObject);
		if (!blueprint)
		{
			return;
		}

		// Replace the original actor in the world with the updated blueprint
		AActor* origActor = GEditor->GetSelectedActors()->GetTop<AActor>();
		if (!origActor)
			return;

		UWorld* world = origActor->GetWorld();
		if (!world)
			return;

		// Deselect the original actor and Destroy it
		GEditor->SelectActor(origActor, false, false);
		GEditor->Layers->DisassociateActorFromLayers(origActor);
		world->EditorDestroyActor(origActor, false);

		// Replace the actor
		AActor* newActor = world->SpawnActor(blueprint->GeneratedClass);

		// Update selection to new actor
		GEditor->SelectActor(newActor, /*bSelected=*/ true, /*bNotify=*/ true);
	}
}

////////////////////////////////////////////////////////////////////////////////

void FRoadRunnerImporterModule::StartupModule()
{
#if ENGINE_MINOR_VERSION <= 21
	FEditorDelegates::OnAssetReimport.AddStatic(&URoadRunnerReimportFactory::PostReimport);
#else
	GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetReimport.AddStatic(&URoadRunnerReimportFactory::PostReimport);
#endif


	FRoadRunnerStyle::Initialize();
	FRoadRunnerStyle::ReloadTextures();

	if (FRoadRunnerImportUtil::TempFolderExists())
		UE_LOG(RoadRunnerImporter, Warning, TEXT("RoadRunnerImporter temporary folder was not cleaned up last time, check the RoadRunnerImporter content folder if you need to recover assets."));
}

////////////////////////////////////////////////////////////////////////////////

void FRoadRunnerImporterModule::ShutdownModule()
{
	FRoadRunnerImportUtil::CleanUpTempFolder();

	FRoadRunnerStyle::Shutdown();
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRoadRunnerImporterModule, RoadRunnerImporter)