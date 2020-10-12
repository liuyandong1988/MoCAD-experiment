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

#include "RoadRunnerFbxFactory.h"
#include "RoadRunnerImporterLog.h"
#include "RoadRunnerFbxSceneImportFactory.h"

#include <Factories/Factory.h>
#include <Editor/UnrealEd/Public/Editor.h>
#include <Factories/FbxFactory.h>
#include <UnrealEd.h>
#include <Runtime/XmlParser/Public/XmlFile.h>

////////////////////////////////////////////////////////////////////////////////
// Sets a higher import priority so this factory will be used when trying to
// import fbx files.
URoadRunnerFbxFactory::URoadRunnerFbxFactory(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	SupportedClass = UBlueprint::StaticClass();
	Formats.Add(TEXT("fbx;FBX meshes and animations"));
	ImportPriority = DefaultImportPriority + 2;
}

////////////////////////////////////////////////////////////////////////////////
// Try to create the object using the RoadRunner scene import factory,
// otherwise, fall back to the default.
UObject * URoadRunnerFbxFactory::FactoryCreateFile(UClass * inClass, UObject * inParent, FName inName, EObjectFlags flags, const FString & filename, const TCHAR * parms, FFeedbackContext * warn, bool & outCancelled)
{
	FString rrMetadataFile = FPaths::ChangeExtension(filename, ".rrdata.xml");

	// Only use our scene import if metadata file exists, and is valid, otherwise use the default one
	if (!FPaths::FileExists(rrMetadataFile))
	{
		return UFbxFactory::FactoryCreateFile(inClass, inParent, inName, flags, filename, parms, warn, outCancelled);
	}

	FXmlFile* rrXml = new FXmlFile(rrMetadataFile);
	if (!rrXml->IsValid())
	{
		UE_LOG(RoadRunnerImporter, Warning, TEXT("RoadRunner metadata file not valid. Reverting to default fbx factory."));
		return UFbxFactory::FactoryCreateFile(inClass, inParent, inName, flags, filename, parms, warn, outCancelled);
	}

	auto fbxSceneFactory = NewObject<URoadRunnerFbxSceneImportFactory>();

	// Protect factory from garbage collection
	fbxSceneFactory->AddToRoot();
	UObject* newObj = fbxSceneFactory->FactoryCreateFile(inClass, inParent, inName, flags, filename, parms, warn, outCancelled);
	fbxSceneFactory->RemoveFromRoot();

	return newObj;
}

////////////////////////////////////////////////////////////////////////////////

bool URoadRunnerFbxFactory::FactoryCanImport(const FString& filename)
{
	const FString extension = FPaths::GetExtension(filename);

	if (extension == TEXT("fbx"))
	{
		return true;
	}
	return false;
}