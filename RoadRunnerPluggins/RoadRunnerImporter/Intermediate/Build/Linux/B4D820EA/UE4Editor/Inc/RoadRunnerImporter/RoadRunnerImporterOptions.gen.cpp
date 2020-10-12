// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RoadRunnerImporter/Public/RoadRunnerImporterOptions.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRoadRunnerImporterOptions() {}
// Cross Module References
	ROADRUNNERIMPORTER_API UClass* Z_Construct_UClass_URoadRunnerImporterOptions_NoRegister();
	ROADRUNNERIMPORTER_API UClass* Z_Construct_UClass_URoadRunnerImporterOptions();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_RoadRunnerImporter();
// End Cross Module References
	void URoadRunnerImporterOptions::StaticRegisterNativesURoadRunnerImporterOptions()
	{
	}
	UClass* Z_Construct_UClass_URoadRunnerImporterOptions_NoRegister()
	{
		return URoadRunnerImporterOptions::StaticClass();
	}
	struct Z_Construct_UClass_URoadRunnerImporterOptions_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ImportSignals_MetaData[];
#endif
		static void NewProp_ImportSignals_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ImportSignals;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OverwriteMaterials_MetaData[];
#endif
		static void NewProp_OverwriteMaterials_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_OverwriteMaterials;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_URoadRunnerImporterOptions_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerImporter,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerImporterOptions_Statics::Class_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Options for the RoadRunner importer plugin.\n" },
		{ "HideCategories", "Object" },
		{ "IncludePath", "RoadRunnerImporterOptions.h" },
		{ "ModuleRelativePath", "Public/RoadRunnerImporterOptions.h" },
		{ "ToolTip", "Options for the RoadRunner importer plugin." },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_ImportSignals_MetaData[] = {
		{ "Category", "ImportOptions" },
		{ "Comment", "// Determines if the plugin should import traffic signal visuals.\n// Currently only compatible with the import as Blueprint option.\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerImporterOptions.h" },
		{ "ToolTip", "Determines if the plugin should import traffic signal visuals.\nCurrently only compatible with the import as Blueprint option." },
	};
#endif
	void Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_ImportSignals_SetBit(void* Obj)
	{
		((URoadRunnerImporterOptions*)Obj)->ImportSignals = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_ImportSignals = { "ImportSignals", nullptr, (EPropertyFlags)0x0010000000004001, UE4CodeGen_Private::EPropertyGenFlags::Bool , RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(uint8), sizeof(URoadRunnerImporterOptions), &Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_ImportSignals_SetBit, METADATA_PARAMS(Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_ImportSignals_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_ImportSignals_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_OverwriteMaterials_MetaData[] = {
		{ "Category", "ImportOptions" },
		{ "Comment", "// Determines if the plugin will overwrite the previous materials attached\n// with new materials instanced from the base materials.\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerImporterOptions.h" },
		{ "ToolTip", "Determines if the plugin will overwrite the previous materials attached\nwith new materials instanced from the base materials." },
	};
#endif
	void Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_OverwriteMaterials_SetBit(void* Obj)
	{
		((URoadRunnerImporterOptions*)Obj)->OverwriteMaterials = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_OverwriteMaterials = { "OverwriteMaterials", nullptr, (EPropertyFlags)0x0010000000004001, UE4CodeGen_Private::EPropertyGenFlags::Bool , RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(uint8), sizeof(URoadRunnerImporterOptions), &Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_OverwriteMaterials_SetBit, METADATA_PARAMS(Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_OverwriteMaterials_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_OverwriteMaterials_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_URoadRunnerImporterOptions_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_ImportSignals,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URoadRunnerImporterOptions_Statics::NewProp_OverwriteMaterials,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_URoadRunnerImporterOptions_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URoadRunnerImporterOptions>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_URoadRunnerImporterOptions_Statics::ClassParams = {
		&URoadRunnerImporterOptions::StaticClass,
		"EditorPerProjectUserSettings",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_URoadRunnerImporterOptions_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerImporterOptions_Statics::PropPointers),
		0,
		0x000800A4u,
		METADATA_PARAMS(Z_Construct_UClass_URoadRunnerImporterOptions_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerImporterOptions_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_URoadRunnerImporterOptions()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_URoadRunnerImporterOptions_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(URoadRunnerImporterOptions, 961296264);
	template<> ROADRUNNERIMPORTER_API UClass* StaticClass<URoadRunnerImporterOptions>()
	{
		return URoadRunnerImporterOptions::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_URoadRunnerImporterOptions(Z_Construct_UClass_URoadRunnerImporterOptions, &URoadRunnerImporterOptions::StaticClass, TEXT("/Script/RoadRunnerImporter"), TEXT("URoadRunnerImporterOptions"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URoadRunnerImporterOptions);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
