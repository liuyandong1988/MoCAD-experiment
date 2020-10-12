// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RoadRunnerImporter/Public/RoadRunnerFbxFactory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRoadRunnerFbxFactory() {}
// Cross Module References
	ROADRUNNERIMPORTER_API UClass* Z_Construct_UClass_URoadRunnerFbxFactory_NoRegister();
	ROADRUNNERIMPORTER_API UClass* Z_Construct_UClass_URoadRunnerFbxFactory();
	UNREALED_API UClass* Z_Construct_UClass_UFbxFactory();
	UPackage* Z_Construct_UPackage__Script_RoadRunnerImporter();
// End Cross Module References
	void URoadRunnerFbxFactory::StaticRegisterNativesURoadRunnerFbxFactory()
	{
	}
	UClass* Z_Construct_UClass_URoadRunnerFbxFactory_NoRegister()
	{
		return URoadRunnerFbxFactory::StaticClass();
	}
	struct Z_Construct_UClass_URoadRunnerFbxFactory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_URoadRunnerFbxFactory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UFbxFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerImporter,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerFbxFactory_Statics::Class_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Attempts to import using the custom RoadRunner scene import factory.\n// If it fails, go back to the default fbx importer.\n// Note: This factory is needed to overwrite the normal fbx import with\n// our scene importer since Factory priority doesn't work with FbxSceneImportFactory\n" },
		{ "HideCategories", "Object Object" },
		{ "IncludePath", "RoadRunnerFbxFactory.h" },
		{ "ModuleRelativePath", "Public/RoadRunnerFbxFactory.h" },
		{ "ToolTip", "Attempts to import using the custom RoadRunner scene import factory.\nIf it fails, go back to the default fbx importer.\nNote: This factory is needed to overwrite the normal fbx import with\nour scene importer since Factory priority doesn't work with FbxSceneImportFactory" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_URoadRunnerFbxFactory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URoadRunnerFbxFactory>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_URoadRunnerFbxFactory_Statics::ClassParams = {
		&URoadRunnerFbxFactory::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_URoadRunnerFbxFactory_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerFbxFactory_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_URoadRunnerFbxFactory()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_URoadRunnerFbxFactory_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(URoadRunnerFbxFactory, 869644689);
	template<> ROADRUNNERIMPORTER_API UClass* StaticClass<URoadRunnerFbxFactory>()
	{
		return URoadRunnerFbxFactory::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_URoadRunnerFbxFactory(Z_Construct_UClass_URoadRunnerFbxFactory, &URoadRunnerFbxFactory::StaticClass, TEXT("/Script/RoadRunnerImporter"), TEXT("URoadRunnerFbxFactory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URoadRunnerFbxFactory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
