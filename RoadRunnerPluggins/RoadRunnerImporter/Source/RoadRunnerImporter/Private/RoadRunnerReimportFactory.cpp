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

#include "RoadRunnerReimportFactory.h"
#include "RoadRunnerImporterLog.h"
#include "RoadRunnerImportUtil.h"

#include <Factories/Factory.h>
#include <Editor/UnrealEd/Public/Editor.h>
#include <Factories/FbxFactory.h>
#include <ObjectTools.h>
#include <PackageTools.h>
#include <UnrealEd.h>
#include <Runtime/XmlParser/Public/XmlFile.h>

#include <AssetRegistryModule.h>
#include <ContentBrowserModule.h>
#include <Editor/ContentBrowser/Public/IContentBrowserSingleton.h>
#include <Developer/AssetTools/Public/AssetToolsModule.h>
#include <Developer/AssetTools/Public/IAssetTools.h>

////////////////////////////////////////////////////////////////////////////////
// Sets a higher import priority so this factory will be used when trying to
// import fbx files.
URoadRunnerReimportFactory::URoadRunnerReimportFactory(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	SupportedClass = UFbxSceneImportData::StaticClass();
	Formats.Add(TEXT("fbx;FBX scene"));

	bCreateNew = false;
	bText = false;

	ImportPriority = DefaultImportPriority + 1;
}

////////////////////////////////////////////////////////////////////////////////

bool URoadRunnerReimportFactory::CanReimport(UObject * obj, TArray<FString>& outFilenames)
{
	UFbxSceneImportData* sceneImportData = nullptr;
	if (obj->IsA(UFbxSceneImportData::StaticClass()))
	{
		//Reimport from the scene data
		sceneImportData = Cast<UFbxSceneImportData>(obj);
	}
	if (!sceneImportData)
		return false;

	FString rrMetadataFile = FPaths::ChangeExtension(sceneImportData->SourceFbxFile, ".rrdata.xml");
	// Only use our reimporter if metadata file exists
	if (!FPaths::FileExists(rrMetadataFile))
	{
		return false;
	}

	outFilenames.Add(sceneImportData->SourceFbxFile);
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void URoadRunnerReimportFactory::SetReimportPaths(UObject * obj, const TArray<FString>& newReimportPaths)
{
	UFbxSceneImportData* sceneImportData = Cast<UFbxSceneImportData>(obj);
	if (sceneImportData && ensure(newReimportPaths.Num() == 1))
	{
		sceneImportData->SourceFbxFile = newReimportPaths[0];
	}
}

////////////////////////////////////////////////////////////////////////////////

EReimportResult::Type URoadRunnerReimportFactory::Reimport(UObject * obj)
{
	ReimportData = nullptr;
	if (obj->IsA(UFbxSceneImportData::StaticClass()))
	{
		//Reimport from the scene data
		ReimportData = Cast<UFbxSceneImportData>(obj);
	}
	if (!ReimportData)
	{
		return EReimportResult::Failed;
	}

	// Clean up temp folder
	FRoadRunnerImportUtil::CleanUpTempFolder();

	this->AddToRoot();

	obj->GetOutermost()->FullyLoad();
	bool cancelled = false;
	CurrentFilename = ReimportData->SourceFbxFile;

	FString oldPackageFolder = FPaths::GetPath(ReimportData->GetPathName(nullptr));
	FName oldName = FName(*obj->GetName());


	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	// Get both in-memory and on-disk references
	TSet<UStaticMesh*> oldMeshes;

	// Check on-disk references
	FString packageName = FPackageName::ObjectPathToPackageName(ReimportData->GetPathName());
	auto assetId = FAssetIdentifier::FromString(packageName);
	TArray<FAssetIdentifier> refNames;
	assetRegistryModule.Get().GetReferencers(assetId, refNames, EAssetRegistryDependencyType::Hard);
	for (const auto& asset : refNames)
	{
		UPackage* staticMeshPackage = LoadPackage(nullptr, *asset.PackageName.ToString(), LOAD_NoWarn);

		TArray<UObject*> objects;
		TArray<UPackage*> packages;
		packages.Add(staticMeshPackage);
		PackageTools::GetObjectsInPackages(&packages, objects);

		// If able to get static mesh asset
		if (objects.Num() > 0)
		{
			// confirm asset is static mesh
			UStaticMesh* meshAsset = Cast<UStaticMesh>(objects[0]);
			if (meshAsset)
			{
				oldMeshes.Add(meshAsset);
			}
		}
	}

	// Get in-memory referencers
	TArray<FReferencerInformation> internalRefs;
	TArray<FReferencerInformation> externalRefs;
	ReimportData->RetrieveReferencers(&internalRefs, &externalRefs);

	for (const auto& refInfo : externalRefs)
	{
		UStaticMesh* meshAsset = Cast<UStaticMesh>(refInfo.Referencer->GetOuter());
		if (meshAsset)
		{
			oldMeshes.Add(meshAsset);
		}
	}

	// Move old files to temp folder
	// Restore if import is cancelled, or fails
	// Delete if import succeeds
	TArray<FAssetRenameData> assetRenameList;
	TArray<FAssetRenameData> revertList;
	TArray<UObject*> objectsToForceDelete;

	for (auto& meshAsset : oldMeshes)
	{
		FAssetRenameData assetRename(meshAsset, "/RoadRunnerImporter/TEMP" / obj->GetName(), meshAsset->GetName());
		assetRenameList.Add(assetRename);
		FString folderName = FPaths::GetPath(meshAsset->GetPathName(nullptr));
		FAssetRenameData revertRename(meshAsset, folderName, meshAsset->GetName());
		revertList.Add(revertRename);
		objectsToForceDelete.Add(meshAsset);
	}

	FAssetToolsModule& assetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	assetToolsModule.Get().RenameAssets(assetRenameList);

	ReimportData = nullptr;

	FRoadRunnerImportUtil::FixUpRedirectors();

	// Returns reimport data or blueprint
	UObject* retVal = URoadRunnerFbxSceneImportFactory::FactoryCreateFile(
		UStaticMesh::StaticClass(),
		obj->GetOutermost(),
		oldName,
		EObjectFlags::RF_Public | EObjectFlags::RF_Standalone | EObjectFlags::RF_Transactional,
		CurrentFilename,
		nullptr,
		GWarn,
		cancelled
	);

	this->RemoveFromRoot();

	// Setup to refresh content browser
	TArray<FString> folder = { oldPackageFolder };
	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	if (cancelled)
	{
		assetToolsModule.Get().RenameAssets(revertList);
		TArray<UObject*> objs = { obj };
		ContentBrowserModule.Get().SyncBrowserToAssets(objs);
		return EReimportResult::Cancelled;
	}

	if (!retVal)
	{
		assetToolsModule.Get().RenameAssets(revertList); 
		TArray<UObject*> objs = { obj };
		ContentBrowserModule.Get().SyncBrowserToAssets(objs);
		return EReimportResult::Failed;
	}

	// Ensure the objects were not deleted elsewhere
	TArray<UObject*> validObjToDelete;
	for (UObject* oldObj : objectsToForceDelete)
	{
		if (oldObj->IsValidLowLevel())
		{
			validObjToDelete.Add(oldObj);
		}
	}

	ObjectTools::ForceDeleteObjects(validObjToDelete, false);

	// Fix up all referencers
	FRoadRunnerImportUtil::FixUpRedirectors();

	TArray<UObject*> objs = { obj };
	ContentBrowserModule.Get().SyncBrowserToAssets(objs);
	return EReimportResult::Succeeded;
}

////////////////////////////////////////////////////////////////////////////////

int32 URoadRunnerReimportFactory::GetPriority() const
{
	return ImportPriority;
}

////////////////////////////////////////////////////////////////////////////////

bool URoadRunnerReimportFactory::FactoryCanImport(const FString& filename)
{
	// Return false, we are a reimport only factory
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// Since we are not using the ReimportFbxSceneFactory, an extra FbxSceneImportData
// will be created in the same package. This function will try to merge the extra
// object and fixup the redirectors.
void URoadRunnerReimportFactory::PostReimport(UObject* inCreateObject)
{
	if (!inCreateObject)
		return;

	if (!inCreateObject->IsA<UFbxSceneImportData>())
		return;

	UPackage* package = inCreateObject->GetOutermost();
	TArray<UObject*> objects;
	TArray<UPackage*> packages;
	packages.Add(package);
	PackageTools::GetObjectsInPackages(&packages, objects);

	// Merge references
	if (objects.Num() > 1)
	{
		// Consolidate all extra objects in the reimport data package
		FCommentNodeSet extras;
		for (int i = 1; i < objects.Num(); i++)
		{
			extras.Add(objects[i]);
		}

		ObjectTools::FConsolidationResults ConsolidationResults = ObjectTools::ConsolidateObjects(objects[0], extras, false);
	}

	TArray<UObject*> newObjects;
	// Reload package and fixup referencers
	PackageTools::GetObjectsInPackages(&packages, newObjects);
	if (newObjects.Num() > 1)
	{
		TArray<UObjectRedirector*> redirectors;

		for (int i = 0; i < newObjects.Num(); i++)
		{
			auto& obj = newObjects[i];
			if (!obj->IsA(UObjectRedirector::StaticClass()))
				continue;
			auto objRedirector = Cast<UObjectRedirector>(obj);
			if (!objRedirector)
				continue;

			redirectors.Add(objRedirector);
		}

		FAssetToolsModule& assetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		assetToolsModule.Get().FixupReferencers(redirectors);
	}

	// Ensure referencer was deleted
	TArray<UObject*> deleteCheck;
	PackageTools::GetObjectsInPackages(&packages, deleteCheck);
	if (deleteCheck.Num() > 1)
	{
		TArray<UObject*> objectsToForceDelete;
		objectsToForceDelete.Add(deleteCheck[1]);
		ObjectTools::ForceDeleteObjects(objectsToForceDelete, false);
	}
	inCreateObject->GetOutermost()->MarkPackageDirty();
}

////////////////////////////////////////////////////////////////////////////////
