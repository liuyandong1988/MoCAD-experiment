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

#include "RoadRunnerCarlaIntegration.h"
#include "RoadRunnerCarlaIntegrationLog.h"
#include "RoadRunnerFbxSceneImportFactory.h"
#include "RoadRunnerImporterOptions.h"
#include "RoadRunnerImportUtil.h"
#include "RoadRunnerImporter.h"

#include <AssetRegistryModule.h>
#include <Developer/AssetTools/Public/AssetToolsModule.h>
#include <Developer/AssetTools/Public/IAssetTools.h>
#include <Editor/UnrealEd/Public/EditorBuildUtils.h>
#include <Runtime/XmlParser/Public/XmlFile.h>
#include <ObjectTools.h>
#include <PackageTools.h>
#include <PlatformFilemanager.h>

#include <OpenDrive/OpenDriveActor.h>

#if ENGINE_MINOR_VERSION > 21
#include <Subsystems/ImportSubsystem.h>
#endif

// Fix for CARLA 0.9.4
#undef CopyFile

#define LOCTEXT_NAMESPACE "FRoadRunnerCarlaIntegrationModule"

const FString FRoadRunnerCarlaIntegrationModule::BaseMapPath = "/Game/Carla/Maps/BaseMap/BaseMap.BaseMap";
const FString FRoadRunnerCarlaIntegrationModule::MapDestRelPath = "Carla/ExportedMaps/";
const FString FRoadRunnerCarlaIntegrationModule::OpenDriveDestRelPath = "Carla/Maps/OpenDrive";

TArray<FString> FRoadRunnerCarlaIntegrationModule::PathList = {
	"/Game/Carla/Static/Road/",
	"/Game/Carla/Static/RoadLines/",
	"/Game/Carla/Static/Terrain/",
	"/Game/Carla/Static/Props/",
	"/Game/Carla/Static/SideWalk/",
	"/Game/Carla/Static/TrafficSigns/",
	"/Game/Carla/Static/Vegetation/",
	"/Game/Carla/Static/Fences/",
	"/Game/Carla/Static/Pole/",
	"/Game/Carla/Static/Vehicles/",
	"/Game/Carla/Static/Walls/",
	"/Game/Carla/Static/Buildings/"
};

namespace
{
	////////////////////////////////////////////////////////////////////////////////
	// Create a material using CARLA's as a base to allow weather effects. Currently
	// only replaces the Road and Foliage materials since the other materials were
	// missing vertex coloring or texture scaling.
	void CreateCarlaMaterial(FString sourceFilePath, FString packagePath, FRoadRunnerImportUtil::MaterialInfo materialInfo, int metadataVersion)
	{
		FString materialFullName = materialInfo.Name;
		FString basePackageName = FPackageName::GetLongPackagePath(packagePath) / materialFullName;
		basePackageName = PackageTools::SanitizePackageName(basePackageName);

		// Always use our materials for decals and markings
		if (materialInfo.IsDecal.Equals("true"))
			return;

		// Find our base material to instance from
		FText materialSearchError;
		UMaterialInterface* baseMaterial = nullptr;

		FString materialName;
		static FString materialPath = "/Game/Carla/Static/GenericMaterials/Masters/";

		FString segName = materialInfo.SegmentationType;
		// Road and sidewalk
		if (segName.Equals("Road") || segName.Equals("Gutter") || segName.Equals("Curb") || segName.Equals("Sidewalk"))
		{
			materialName = "MI_WetPavement";
		}
		else if (segName.Equals("Ground")) // Terrain
		{
			return; // Just use RoadRunner materials
		}
		else // Props or foliage
		{
			// Foliage
			if (materialFullName.Contains("Leaves")) // Hack to find leaves
			{
				materialName = "M_Foliage";
			}
			else
			{
				return; // Just use RoadRunner materials
			}
		}

		float alphaVal = 1.0f; // Default to opaque
		float transparency = 0.0f;
		if (!materialInfo.TransparencyFactor.IsEmpty())
		{
			transparency = FCString::Atof(*(materialInfo.TransparencyFactor));
		}

		baseMaterial = UMaterialImportHelpers::FindExistingMaterialFromSearchLocation(materialName, materialPath, EMaterialSearchLocation::UnderParent, materialSearchError);

		if (!materialSearchError.IsEmpty() || baseMaterial == nullptr)
		{
			UE_LOG(RoadRunnerCarlaIntegration, Error, TEXT("Base material not found: %s"), *(materialSearchError.ToString()));
			return;
		}

		// Get the original material package and delete it
		UMaterial* oldMat = LoadObject<UMaterial>(nullptr, *basePackageName);
		if (oldMat != nullptr)
		{
			TArray<UObject*> objectsToForceDelete;
			objectsToForceDelete.Add(oldMat);
			ObjectTools::ForceDeleteObjects(objectsToForceDelete, false);
		}

		// Re-create package for new material instance
		UPackage* package = CreatePackage(NULL, *basePackageName);

		// Create material instance from our base material
		// Modified: always create a material instance from our base material
		auto materialInstanceFactory = NewObject<UMaterialInstanceConstantFactoryNew>();
		materialInstanceFactory->InitialParent = baseMaterial;
		UMaterialInstanceConstant* unrealMaterial = (UMaterialInstanceConstant*)materialInstanceFactory->FactoryCreateNew(UMaterialInstanceConstant::StaticClass(), package, *materialFullName, RF_Standalone | RF_Public, NULL, GWarn);
		if (unrealMaterial == NULL)
		{
			UE_LOG(RoadRunnerCarlaIntegration, Error, TEXT("Material %s could not be created."), *materialInfo.Name);
			return;
		}

		// Notify the asset registry
		FAssetRegistryModule::AssetCreated(unrealMaterial);
		// Set the dirty flag so this package will get saved later
		package->SetDirtyFlag(true);

		// Set material parameters
		// Leave out marking due to vertex coloring and fences
		if (materialName.Equals("MI_WetPavement"))
		{
			FRoadRunnerImportUtil::SetTextureParameter(unrealMaterial, FName(TEXT("basecolorA")), sourceFilePath, materialInfo.DiffuseMap, packagePath);
			FRoadRunnerImportUtil::SetTextureParameter(unrealMaterial, FName(TEXT("NormalA")), sourceFilePath, materialInfo.NormalMap, packagePath);
			FRoadRunnerImportUtil::SetTextureParameter(unrealMaterial, FName(TEXT("RoughnessA")), sourceFilePath, materialInfo.SpecularMap, packagePath);

			// Disable multiple textures
			FStaticParameterSet matStaticParams;
			unrealMaterial->GetStaticParameterValues(matStaticParams);
			for (auto& switchParam : matStaticParams.StaticSwitchParameters)
			{
				if (switchParam.ParameterInfo.Name.IsEqual("MultipleTextures"))
				{
					switchParam.bOverride = true;
					switchParam.Value = true;
				}
			}

			unrealMaterial->UpdateStaticPermutation(matStaticParams);
		}
		else if (materialName.Equals("M_Foliage"))
		{
			FRoadRunnerImportUtil::SetTextureParameter(unrealMaterial, FName(TEXT("BaseColor")), sourceFilePath, materialInfo.DiffuseMap, packagePath);
			FRoadRunnerImportUtil::SetTextureParameter(unrealMaterial, FName(TEXT("None")), sourceFilePath, materialInfo.NormalMap, packagePath);

#if ENGINE_MINOR_VERSION > 18
			unrealMaterial->SetVectorParameterValueEditorOnly(FMaterialParameterInfo(FName(TEXT("SubsurfaceValue")), EMaterialParameterAssociation::GlobalParameter), FLinearColor(0.07f, 0.1f, 0.07f, 1));
#else
			material->SetVectorParameterValueEditorOnly(FName(TEXT("SubsurfaceValue")), FLinearColor(0.07f, 0.1f, 0.07f, 1));
#endif
		}
		else
		{
			return;
		}

		// let the material update itself if necessary
		unrealMaterial->PreEditChange(NULL);
		unrealMaterial->PostEditChange();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Since creating the map is done pre-import, a new map will be opened even if
// the import was cancelled
void FRoadRunnerCarlaIntegrationModule::RRCarlaPreImport(UFactory* inFactory, UClass* inClass, UObject* inParent, const FName& name, const TCHAR* type)
{
	if (!inFactory->IsA<URoadRunnerFbxSceneImportFactory>())
	{
		return;
	}

	// Ensure this is only called from our delegate calls
	URoadRunnerFbxSceneImportFactory* rrFactory = Cast<URoadRunnerFbxSceneImportFactory>(inFactory);
	if (!rrFactory->OuterCall)
	{
		return;
	}


	UE_LOG(RoadRunnerCarlaIntegration, Log, TEXT("Starting RoadRunner pre-import"));

	// Search for the base map
	FString packageName;
	if (FPackageName::DoesPackageExist(BaseMapPath, nullptr, &packageName))
	{
		GEditor->CreateNewMapForEditing(); // Note: Creates save pop-up for currently loaded level

		// Load base map as template
		if (!FEditorFileUtils::LoadMap(BaseMapPath, true, false))
			UE_LOG(RoadRunnerCarlaIntegration, Error, TEXT("BaseMap failed to load"));

		// Automatically save map with correct name
		FString importFileName = FPaths::GetBaseFilename(name.ToString());
		FString mapName = FPaths::ProjectContentDir() + MapDestRelPath + importFileName + ".umap";
		FEditorFileUtils::SaveLevel(GWorld->PersistentLevel, mapName);
	}
	else
	{
		UE_LOG(RoadRunnerCarlaIntegration, Error, TEXT("BaseMap could not be found."));
	}
}

////////////////////////////////////////////////////////////////////////////////

void FRoadRunnerCarlaIntegrationModule::RRCarlaPostImport(UFactory* inFactory, UObject* inCreateObject)
{
	if (!inFactory->IsA<URoadRunnerFbxSceneImportFactory>())
	{
		return;
	}

	// Ensure this is only called from our delegate calls
	URoadRunnerFbxSceneImportFactory* rrFactory = Cast<URoadRunnerFbxSceneImportFactory>(inFactory);
	if (!rrFactory->OuterCall)
	{
		return;
	}

	if (!inCreateObject)
		return;

	// Replaces invalid characters with '_'
	FString mapName = ObjectTools::SanitizeObjectName(FPaths::GetBaseFilename(inFactory->GetCurrentFilename()));

	FAssetToolsModule& assetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	// Parse materials
	TMap<FString, FString> materialSegmentationMap;
	FString rrMetadataFile = FPaths::ChangeExtension(inFactory->GetCurrentFilename(), ".rrdata.xml");
	if (!FPaths::FileExists(rrMetadataFile))
	{
		return;
	}

	FXmlFile* rrXml = new FXmlFile(rrMetadataFile);
	if (!rrXml->IsValid())
	{
		UE_LOG(RoadRunnerCarlaIntegration, Error, TEXT("Metadata XML is invalid in: %s"), *(rrMetadataFile));
		return;
	}

	UE_LOG(RoadRunnerCarlaIntegration, Log, TEXT("Starting RoadRunner post-import"));

	FXmlNode* xmlRoot = rrXml->GetRootNode();

	// Check version
	int currentMetadataVersion = FCString::Atoi(*(xmlRoot->GetAttribute("Version")));
	bool hasSegmentInfo = currentMetadataVersion >= 2;

	FXmlNode * xmlMatList = xmlRoot->FindChildNode("MaterialList");
	if (!xmlMatList)
	{
		UE_LOG(RoadRunnerCarlaIntegration, Error, TEXT("Material List not found in metadata: %s"), *(rrMetadataFile));
		return;
	}

	// Fill out segmentation map
	if (hasSegmentInfo)
	{
		for (const auto& mat : xmlMatList->GetChildrenNodes())
		{
			const auto& matProperties = mat->GetChildrenNodes();

			// Only need name and segmentationType;
			FString name;
			FString segmentationType;
			for (const auto& matProperty : matProperties)
			{
				const FString& tag = matProperty->GetTag();
				if (tag.Equals(TEXT("Name"), ESearchCase::CaseSensitive))
				{
					name = matProperty->GetContent();

					// Clean the name
					name = FRoadRunnerImportUtil::ConvertMaterialName(name);

				}
				else if (tag.Equals(TEXT("SegmentationType"), ESearchCase::CaseSensitive))
				{
					segmentationType = matProperty->GetContent();
				}
			}
			materialSegmentationMap.Add(name, segmentationType);
		}
	}

	// Create CARLA materials
	FString srcPath = FPaths::GetPath(*inFactory->GetCurrentFilename());
	FString packagePath = FPaths::GetPath(inCreateObject->GetPathName()) + "/";

	if (rrFactory->RoadRunnerImportOptions->OverwriteMaterials)
	{
		for (const auto& matInfo : FRoadRunnerImportUtil::ParseMaterialXml(xmlMatList))
		{
			CreateCarlaMaterial(srcPath, packagePath, matInfo, currentMetadataVersion);
		}
	}


	TArray<FAssetRenameData> assetRenameList;
	TArray<UObject*> objectsToForceDelete;
	TArray<UObjectRedirector*> redirectorsToFix;

	// Update materials and move static mesh to folder based off segmentation
	for (auto assetIter = rrFactory->ImportedMeshes.CreateIterator(); assetIter; ++assetIter)
	{
		UObject *assetObject = *assetIter;
		if (assetObject)
		{
			if (assetObject->IsA(UStaticMesh::StaticClass()))
			{
				UStaticMesh* staticMesh = Cast<UStaticMesh>(assetObject);

				if (rrFactory->RoadRunnerImportOptions->OverwriteMaterials)
				{
					// Replace materials again
					for (auto& matref : staticMesh->StaticMaterials)
					{
						// Re-create material package name from old material reference
						FString materialFullName = matref.ImportedMaterialSlotName.ToString();
						// Follow Unreal's naming scheme
						materialFullName = FRoadRunnerImportUtil::ConvertMaterialName(materialFullName);

						FString basePackageName = FPackageName::GetLongPackagePath(packagePath) / materialFullName;
						basePackageName = PackageTools::SanitizePackageName(basePackageName);

						matref.MaterialInterface = LoadObject<UMaterialInterface>(nullptr, *basePackageName);
					}
				}

				// Compare name, move to appropriate folder
				FString meshPackagePath = FPackageName::GetLongPackagePath(assetObject->GetOutermost()->GetName());
				const FString meshPackageName = assetObject->GetOutermost()->GetName();
				const FString meshName = assetObject->GetName();
				int index = 2; // Default to terrain
				if (hasSegmentInfo)
				{
					FString segName;

					// Compare material segmentation type
					for (auto& matref : staticMesh->StaticMaterials)
					{
						// Re-create material package name from old material reference
						FString materialFullName = matref.ImportedMaterialSlotName.ToString();
						// Follow Unreal's naming scheme
						materialFullName = FRoadRunnerImportUtil::ConvertMaterialName(materialFullName);

						// Look up material name in map
						FString* segNamePtr = materialSegmentationMap.Find(materialFullName);
						if (segNamePtr)
						{
							segName = *segNamePtr;
							break;
						}
						else
						{
							UE_LOG(RoadRunnerCarlaIntegration, Error, TEXT("%s could not be found in metadata"), *materialFullName);
						}
					}

					if (segName.Equals("Road") || segName.Equals("Gutter"))
					{
						index = SegmentationType::eRoad;
					}
					else if (segName.Equals("Marking"))
					{
						index = SegmentationType::eMarking;
					}
					else if (segName.Equals("Ground"))
					{
						index = SegmentationType::eTerrain;
					}
					else if (segName.Equals("Prop"))
					{
						index = SegmentationType::eProp;

						// Fallback for old props that didn't have foliage set by default
						if (meshName.Contains("Trunk") || meshName.Contains("Leaves") || meshName.StartsWith("Bush"))
							index = SegmentationType::eFoliage;
					}
					else if (segName.Equals("Curb") || segName.Equals("Sidewalk"))
					{
						index = SegmentationType::eSidewalk;
					}
					else if (segName.Equals("Sign") || segName.Equals("Signal"))
					{
						index = SegmentationType::eSign;
					}
					else if (segName.Equals("Fence"))
					{
						index = SegmentationType::eFence;
					}
					else if (segName.Equals("Pole"))
					{
						index = SegmentationType::ePole;
					}
					else if (segName.Equals("Vehicle"))
					{
						index = SegmentationType::eVehicle;
					}
					else if (segName.Equals("Wall"))
					{
						index = SegmentationType::eWall;
					}
					else if (segName.Equals("Foliage"))
					{
						index = SegmentationType::eFoliage;
					}
					else if (segName.Equals("Building"))
					{
						index = SegmentationType::eBuilding;
					}
				}
				else // For older versions, revert to sorting based off mesh name
				{
					if (meshPackageName.Contains("Road"))
					{
						index = SegmentationType::eRoad;
					}
					else if (meshPackageName.Contains("LaneMarking"))
					{
						index = SegmentationType::eMarking;
					}
					else if (meshPackageName.Contains("Terrain"))
					{
						index = SegmentationType::eTerrain;
					}
				}

				FString newPath = PathList[index] + mapName;
				FString newPackageName = newPath+ "/" + assetObject->GetName();


				// Delete previous static mesh if it exists, or fix up redirectors
				UPackage* prevPackage = LoadPackage(nullptr, *newPackageName, LOAD_NoWarn);
				if (prevPackage)
				{
					if (UStaticMesh* oldMesh = LoadObject<UStaticMesh>(prevPackage, *assetObject->GetName(), NULL, LOAD_NoRedirects, NULL))
					{
						objectsToForceDelete.Add(oldMesh);
					}
					else if (UObjectRedirector* oldRedirector = LoadObject<UObjectRedirector>(prevPackage, *assetObject->GetName(), NULL, LOAD_NoRedirects, NULL))
					{
						redirectorsToFix.Add(oldRedirector);
					}
				}
				FAssetRenameData assetRename(assetObject, newPath, assetObject->GetName());
				assetRenameList.Add(assetRename);
			}
		}
	}

	assetToolsModule.Get().FixupReferencers(redirectorsToFix);
	ObjectTools::ForceDeleteObjects(objectsToForceDelete, false);
	assetToolsModule.Get().RenameAssets(assetRenameList);

	// Copy OpenDRIVE file
	FString rrOpenDriveFile = FPaths::ChangeExtension(inFactory->GetCurrentFilename(), ".xodr");

	if (!FPaths::FileExists(rrOpenDriveFile))
	{
		UE_LOG(RoadRunnerCarlaIntegration, Error, TEXT("OpenDRIVE file not found at %s"), *rrOpenDriveFile);
		return;
	}
	
	FString projectOpenDrivePath = FPaths::ProjectContentDir() + OpenDriveDestRelPath;

	FString filesystemOpenDrivePath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*projectOpenDrivePath) + "/" + mapName + ".xodr";

	if (!FPlatformFileManager::Get().GetPlatformFile().CopyFile(*filesystemOpenDrivePath, *rrOpenDriveFile))
	{
		UE_LOG(RoadRunnerCarlaIntegration, Error, TEXT("Failed to copy OpenDrive to %s"), *filesystemOpenDrivePath);
	}

	// Generate routes in current map
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (!world)
	{
		UE_LOG(RoadRunnerCarlaIntegration, Error, TEXT("Could not get current world."));
		return;
	}

	// Find existing OpenDriveActor
	TActorIterator<AOpenDriveActor> itr(world);
	AOpenDriveActor *openDriveActor = nullptr;

	// Get actor if it exists
	if (itr)
	{
		openDriveActor = *itr;
	}
	else // Create new OpenDriveActor if it doesn't exist
	{
		UE_LOG(RoadRunnerCarlaIntegration, Log, TEXT("Spawning new OpenDriveActor"));
		openDriveActor = CastChecked<AOpenDriveActor>(world->SpawnActor(AOpenDriveActor::StaticClass(), new FVector(), NULL));
	}

	// Return if OpenDriveActor doesn't exist and could not be created
	if (!openDriveActor)
		return;

	// Build route
	openDriveActor->BuildRoutes(mapName);
	openDriveActor->AddSpawners();

	// Rebuild world to ensure billboards render
	FEditorBuildUtils::EditorBuild(world, FBuildOptions::BuildGeometry);
}

////////////////////////////////////////////////////////////////////////////////

void FRoadRunnerCarlaIntegrationModule::StartupModule()
{

#if ENGINE_MINOR_VERSION <= 21
	FEditorDelegates::OnAssetPreImport.AddStatic(&FRoadRunnerCarlaIntegrationModule::RRCarlaPreImport);
	FEditorDelegates::OnAssetPostImport.AddStatic(&FRoadRunnerCarlaIntegrationModule::RRCarlaPostImport);
#else
	GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPreImport.AddStatic(&FRoadRunnerCarlaIntegrationModule::RRCarlaPreImport);
	GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPostImport.AddStatic(&FRoadRunnerCarlaIntegrationModule::RRCarlaPostImport);

#endif

}

////////////////////////////////////////////////////////////////////////////////

void FRoadRunnerCarlaIntegrationModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRoadRunnerCarlaIntegrationModule, RoadRunnerCarlaIntegration)