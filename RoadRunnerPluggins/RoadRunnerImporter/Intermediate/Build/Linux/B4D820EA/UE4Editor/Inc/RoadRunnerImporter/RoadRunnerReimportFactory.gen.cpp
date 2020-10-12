// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RoadRunnerImporter/Public/RoadRunnerReimportFactory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRoadRunnerReimportFactory() {}
// Cross Module References
	ROADRUNNERIMPORTER_API UClass* Z_Construct_UClass_URoadRunnerReimportFactory_NoRegister();
	ROADRUNNERIMPORTER_API UClass* Z_Construct_UClass_URoadRunnerReimportFactory();
	ROADRUNNERIMPORTER_API UClass* Z_Construct_UClass_URoadRunnerFbxSceneImportFactory();
	UPackage* Z_Construct_UPackage__Script_RoadRunnerImporter();
// End Cross Module References
	void URoadRunnerReimportFactory::StaticRegisterNativesURoadRunnerReimportFactory()
	{
	}
	UClass* Z_Construct_UClass_URoadRunnerReimportFactory_NoRegister()
	{
		return URoadRunnerReimportFactory::StaticClass();
	}
	struct Z_Construct_UClass_URoadRunnerReimportFactory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_URoadRunnerReimportFactory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_URoadRunnerFbxSceneImportFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerImporter,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerReimportFactory_Statics::Class_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Forces the FBX scene reimport to go through RoadRunnerFbxScene import\n" },
		{ "HideCategories", "Object Object Object" },
		{ "IncludePath", "RoadRunnerReimportFactory.h" },
		{ "ModuleRelativePath", "Public/RoadRunnerReimportFactory.h" },
		{ "ToolTip", "Forces the FBX scene reimport to go through RoadRunnerFbxScene import" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_URoadRunnerReimportFactory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URoadRunnerReimportFactory>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_URoadRunnerReimportFactory_Statics::ClassParams = {
		&URoadRunnerReimportFactory::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_URoadRunnerReimportFactory_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerReimportFactory_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_URoadRunnerReimportFactory()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_URoadRunnerReimportFactory_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(URoadRunnerReimportFactory, 1030055739);
	template<> ROADRUNNERIMPORTER_API UClass* StaticClass<URoadRunnerReimportFactory>()
	{
		return URoadRunnerReimportFactory::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_URoadRunnerReimportFactory(Z_Construct_UClass_URoadRunnerReimportFactory, &URoadRunnerReimportFactory::StaticClass, TEXT("/Script/RoadRunnerImporter"), TEXT("URoadRunnerReimportFactory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URoadRunnerReimportFactory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
