// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RoadRunnerImporter/Public/RoadRunnerFbxSceneImportFactory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRoadRunnerFbxSceneImportFactory() {}
// Cross Module References
	ROADRUNNERIMPORTER_API UClass* Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_NoRegister();
	ROADRUNNERIMPORTER_API UClass* Z_Construct_UClass_URoadRunnerFbxSceneImportFactory();
	UNREALED_API UClass* Z_Construct_UClass_UFbxSceneImportFactory();
	UPackage* Z_Construct_UPackage__Script_RoadRunnerImporter();
	ROADRUNNERIMPORTER_API UClass* Z_Construct_UClass_URoadRunnerImporterOptions_NoRegister();
// End Cross Module References
	void URoadRunnerFbxSceneImportFactory::StaticRegisterNativesURoadRunnerFbxSceneImportFactory()
	{
	}
	UClass* Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_NoRegister()
	{
		return URoadRunnerFbxSceneImportFactory::StaticClass();
	}
	struct Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RoadRunnerImportOptions_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_RoadRunnerImportOptions;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UFbxSceneImportFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerImporter,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::Class_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Import with UFbxSceneImportFactory using the pre/post asset import delegates\n// defined in RoadRunnerImporter.\n" },
		{ "HideCategories", "Object Object" },
		{ "IncludePath", "RoadRunnerFbxSceneImportFactory.h" },
		{ "ModuleRelativePath", "Public/RoadRunnerFbxSceneImportFactory.h" },
		{ "ToolTip", "Import with UFbxSceneImportFactory using the pre/post asset import delegates\ndefined in RoadRunnerImporter." },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::NewProp_RoadRunnerImportOptions_MetaData[] = {
		{ "ModuleRelativePath", "Public/RoadRunnerFbxSceneImportFactory.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::NewProp_RoadRunnerImportOptions = { "RoadRunnerImportOptions", nullptr, (EPropertyFlags)0x0010000000002000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(URoadRunnerFbxSceneImportFactory, RoadRunnerImportOptions), Z_Construct_UClass_URoadRunnerImporterOptions_NoRegister, METADATA_PARAMS(Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::NewProp_RoadRunnerImportOptions_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::NewProp_RoadRunnerImportOptions_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::NewProp_RoadRunnerImportOptions,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URoadRunnerFbxSceneImportFactory>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::ClassParams = {
		&URoadRunnerFbxSceneImportFactory::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::PropPointers),
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_URoadRunnerFbxSceneImportFactory()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_URoadRunnerFbxSceneImportFactory_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(URoadRunnerFbxSceneImportFactory, 3981200823);
	template<> ROADRUNNERIMPORTER_API UClass* StaticClass<URoadRunnerFbxSceneImportFactory>()
	{
		return URoadRunnerFbxSceneImportFactory::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_URoadRunnerFbxSceneImportFactory(Z_Construct_UClass_URoadRunnerFbxSceneImportFactory, &URoadRunnerFbxSceneImportFactory::StaticClass, TEXT("/Script/RoadRunnerImporter"), TEXT("URoadRunnerFbxSceneImportFactory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URoadRunnerFbxSceneImportFactory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
