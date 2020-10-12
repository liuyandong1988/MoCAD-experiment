// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RoadRunnerRuntime/Public/RoadRunnerTrafficJunction.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRoadRunnerTrafficJunction() {}
// Cross Module References
	ROADRUNNERRUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FJunction();
	UPackage* Z_Construct_UPackage__Script_RoadRunnerRuntime();
	ROADRUNNERRUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FSignalPhase();
	ROADRUNNERRUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FLightInterval();
	ROADRUNNERRUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FSignalState();
	ROADRUNNERRUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FLightInstanceState();
	ENGINE_API UClass* Z_Construct_UClass_UMeshComponent_NoRegister();
	ROADRUNNERRUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FSignalAsset();
	ROADRUNNERRUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FSignalConfiguration();
	ROADRUNNERRUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FLightBulbState();
	ROADRUNNERRUNTIME_API UClass* Z_Construct_UClass_URoadRunnerTrafficJunction_NoRegister();
	ROADRUNNERRUNTIME_API UClass* Z_Construct_UClass_URoadRunnerTrafficJunction();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
// End Cross Module References
class UScriptStruct* FJunction::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern ROADRUNNERRUNTIME_API uint32 Get_Z_Construct_UScriptStruct_FJunction_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FJunction, Z_Construct_UPackage__Script_RoadRunnerRuntime(), TEXT("Junction"), sizeof(FJunction), Get_Z_Construct_UScriptStruct_FJunction_Hash());
	}
	return Singleton;
}
template<> ROADRUNNERRUNTIME_API UScriptStruct* StaticStruct<FJunction>()
{
	return FJunction::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FJunction(FJunction::StaticStruct, TEXT("/Script/RoadRunnerRuntime"), TEXT("Junction"), false, nullptr, nullptr);
static struct FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFJunction
{
	FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFJunction()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("Junction")),new UScriptStruct::TCppStructOps<FJunction>);
	}
} ScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFJunction;
	struct Z_Construct_UScriptStruct_FJunction_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SignalPhases_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_SignalPhases;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_SignalPhases_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Id_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Id;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJunction_Statics::Struct_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Represents the \"Junction\" element in the metadata\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Represents the \"Junction\" element in the metadata" },
	};
#endif
	void* Z_Construct_UScriptStruct_FJunction_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FJunction>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJunction_Statics::NewProp_SignalPhases_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// List of signal phases for this junction\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "List of signal phases for this junction" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FJunction_Statics::NewProp_SignalPhases = { "SignalPhases", nullptr, (EPropertyFlags)0x0010008000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJunction, SignalPhases), METADATA_PARAMS(Z_Construct_UScriptStruct_FJunction_Statics::NewProp_SignalPhases_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJunction_Statics::NewProp_SignalPhases_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FJunction_Statics::NewProp_SignalPhases_Inner = { "SignalPhases", nullptr, (EPropertyFlags)0x0000008000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FSignalPhase, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJunction_Statics::NewProp_Id_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// ID of the junction\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "ID of the junction" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FJunction_Statics::NewProp_Id = { "Id", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJunction, Id), METADATA_PARAMS(Z_Construct_UScriptStruct_FJunction_Statics::NewProp_Id_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJunction_Statics::NewProp_Id_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FJunction_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJunction_Statics::NewProp_SignalPhases,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJunction_Statics::NewProp_SignalPhases_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJunction_Statics::NewProp_Id,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FJunction_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerRuntime,
		nullptr,
		&NewStructOps,
		"Junction",
		sizeof(FJunction),
		alignof(FJunction),
		Z_Construct_UScriptStruct_FJunction_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJunction_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000005),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FJunction_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJunction_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FJunction()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FJunction_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_RoadRunnerRuntime();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("Junction"), sizeof(FJunction), Get_Z_Construct_UScriptStruct_FJunction_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FJunction_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FJunction_Hash() { return 3358587621U; }
class UScriptStruct* FSignalPhase::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern ROADRUNNERRUNTIME_API uint32 Get_Z_Construct_UScriptStruct_FSignalPhase_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FSignalPhase, Z_Construct_UPackage__Script_RoadRunnerRuntime(), TEXT("SignalPhase"), sizeof(FSignalPhase), Get_Z_Construct_UScriptStruct_FSignalPhase_Hash());
	}
	return Singleton;
}
template<> ROADRUNNERRUNTIME_API UScriptStruct* StaticStruct<FSignalPhase>()
{
	return FSignalPhase::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FSignalPhase(FSignalPhase::StaticStruct, TEXT("/Script/RoadRunnerRuntime"), TEXT("SignalPhase"), false, nullptr, nullptr);
static struct FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalPhase
{
	FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalPhase()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("SignalPhase")),new UScriptStruct::TCppStructOps<FSignalPhase>);
	}
} ScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalPhase;
	struct Z_Construct_UScriptStruct_FSignalPhase_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Intervals_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Intervals;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Intervals_Inner;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalPhase_Statics::Struct_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Represents the \"SignalPhase\" element in the metadata\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Represents the \"SignalPhase\" element in the metadata" },
	};
#endif
	void* Z_Construct_UScriptStruct_FSignalPhase_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FSignalPhase>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalPhase_Statics::NewProp_Intervals_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// List of intervals (i.e. green, yellow, red)\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "List of intervals (i.e. green, yellow, red)" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FSignalPhase_Statics::NewProp_Intervals = { "Intervals", nullptr, (EPropertyFlags)0x0010008000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FSignalPhase, Intervals), METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalPhase_Statics::NewProp_Intervals_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalPhase_Statics::NewProp_Intervals_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FSignalPhase_Statics::NewProp_Intervals_Inner = { "Intervals", nullptr, (EPropertyFlags)0x0000008000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FLightInterval, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FSignalPhase_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalPhase_Statics::NewProp_Intervals,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalPhase_Statics::NewProp_Intervals_Inner,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FSignalPhase_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerRuntime,
		nullptr,
		&NewStructOps,
		"SignalPhase",
		sizeof(FSignalPhase),
		alignof(FSignalPhase),
		Z_Construct_UScriptStruct_FSignalPhase_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalPhase_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000005),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalPhase_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalPhase_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FSignalPhase()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FSignalPhase_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_RoadRunnerRuntime();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("SignalPhase"), sizeof(FSignalPhase), Get_Z_Construct_UScriptStruct_FSignalPhase_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FSignalPhase_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FSignalPhase_Hash() { return 4229361961U; }
class UScriptStruct* FLightInterval::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern ROADRUNNERRUNTIME_API uint32 Get_Z_Construct_UScriptStruct_FLightInterval_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FLightInterval, Z_Construct_UPackage__Script_RoadRunnerRuntime(), TEXT("LightInterval"), sizeof(FLightInterval), Get_Z_Construct_UScriptStruct_FLightInterval_Hash());
	}
	return Singleton;
}
template<> ROADRUNNERRUNTIME_API UScriptStruct* StaticStruct<FLightInterval>()
{
	return FLightInterval::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FLightInterval(FLightInterval::StaticStruct, TEXT("/Script/RoadRunnerRuntime"), TEXT("LightInterval"), false, nullptr, nullptr);
static struct FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFLightInterval
{
	FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFLightInterval()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("LightInterval")),new UScriptStruct::TCppStructOps<FLightInterval>);
	}
} ScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFLightInterval;
	struct Z_Construct_UScriptStruct_FLightInterval_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SignalStates_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_SignalStates;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_SignalStates_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Time_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Time;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLightInterval_Statics::Struct_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Represents the \"Interval\" element in the metadata\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Represents the \"Interval\" element in the metadata" },
	};
#endif
	void* Z_Construct_UScriptStruct_FLightInterval_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FLightInterval>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_SignalStates_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// List of signal states for this interval\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "List of signal states for this interval" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_SignalStates = { "SignalStates", nullptr, (EPropertyFlags)0x0010008000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLightInterval, SignalStates), METADATA_PARAMS(Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_SignalStates_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_SignalStates_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_SignalStates_Inner = { "SignalStates", nullptr, (EPropertyFlags)0x0000008000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FSignalState, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_Time_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Duration of the interval\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Duration of the interval" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_Time = { "Time", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLightInterval, Time), METADATA_PARAMS(Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_Time_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_Time_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FLightInterval_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_SignalStates,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_SignalStates_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLightInterval_Statics::NewProp_Time,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FLightInterval_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerRuntime,
		nullptr,
		&NewStructOps,
		"LightInterval",
		sizeof(FLightInterval),
		alignof(FLightInterval),
		Z_Construct_UScriptStruct_FLightInterval_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightInterval_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000005),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FLightInterval_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightInterval_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FLightInterval()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FLightInterval_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_RoadRunnerRuntime();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("LightInterval"), sizeof(FLightInterval), Get_Z_Construct_UScriptStruct_FLightInterval_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FLightInterval_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FLightInterval_Hash() { return 4275233081U; }
class UScriptStruct* FSignalState::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern ROADRUNNERRUNTIME_API uint32 Get_Z_Construct_UScriptStruct_FSignalState_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FSignalState, Z_Construct_UPackage__Script_RoadRunnerRuntime(), TEXT("SignalState"), sizeof(FSignalState), Get_Z_Construct_UScriptStruct_FSignalState_Hash());
	}
	return Singleton;
}
template<> ROADRUNNERRUNTIME_API UScriptStruct* StaticStruct<FSignalState>()
{
	return FSignalState::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FSignalState(FSignalState::StaticStruct, TEXT("/Script/RoadRunnerRuntime"), TEXT("SignalState"), false, nullptr, nullptr);
static struct FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalState
{
	FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalState()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("SignalState")),new UScriptStruct::TCppStructOps<FSignalState>);
	}
} ScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalState;
	struct Z_Construct_UScriptStruct_FSignalState_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LightInstanceStates_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_LightInstanceStates;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_LightInstanceStates_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Configuration_MetaData[];
#endif
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_Configuration;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SignalAssetId_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_SignalAssetId;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Id_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Id;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalState_Statics::Struct_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Represents the \"Signal\" state element under \"Interval\" in the metadata\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Represents the \"Signal\" state element under \"Interval\" in the metadata" },
	};
#endif
	void* Z_Construct_UScriptStruct_FSignalState_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FSignalState>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_LightInstanceStates_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Runtime translation of the imported data\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Runtime translation of the imported data" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_LightInstanceStates = { "LightInstanceStates", nullptr, (EPropertyFlags)0x0010008000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FSignalState, LightInstanceStates), METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_LightInstanceStates_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_LightInstanceStates_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_LightInstanceStates_Inner = { "LightInstanceStates", nullptr, (EPropertyFlags)0x0000008000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FLightInstanceState, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_Configuration_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Index into the configuration list in the associated signal asset\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Index into the configuration list in the associated signal asset" },
	};
#endif
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_Configuration = { "Configuration", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FSignalState, Configuration), METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_Configuration_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_Configuration_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_SignalAssetId_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// ID of the signal asset\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "ID of the signal asset" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_SignalAssetId = { "SignalAssetId", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FSignalState, SignalAssetId), METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_SignalAssetId_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_SignalAssetId_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_Id_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// ID of the signal object\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "ID of the signal object" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_Id = { "Id", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FSignalState, Id), METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_Id_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_Id_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FSignalState_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_LightInstanceStates,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_LightInstanceStates_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_Configuration,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_SignalAssetId,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalState_Statics::NewProp_Id,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FSignalState_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerRuntime,
		nullptr,
		&NewStructOps,
		"SignalState",
		sizeof(FSignalState),
		alignof(FSignalState),
		Z_Construct_UScriptStruct_FSignalState_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalState_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000005),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalState_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalState_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FSignalState()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FSignalState_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_RoadRunnerRuntime();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("SignalState"), sizeof(FSignalState), Get_Z_Construct_UScriptStruct_FSignalState_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FSignalState_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FSignalState_Hash() { return 2043974402U; }
class UScriptStruct* FLightInstanceState::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern ROADRUNNERRUNTIME_API uint32 Get_Z_Construct_UScriptStruct_FLightInstanceState_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FLightInstanceState, Z_Construct_UPackage__Script_RoadRunnerRuntime(), TEXT("LightInstanceState"), sizeof(FLightInstanceState), Get_Z_Construct_UScriptStruct_FLightInstanceState_Hash());
	}
	return Singleton;
}
template<> ROADRUNNERRUNTIME_API UScriptStruct* StaticStruct<FLightInstanceState>()
{
	return FLightInstanceState::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FLightInstanceState(FLightInstanceState::StaticStruct, TEXT("/Script/RoadRunnerRuntime"), TEXT("LightInstanceState"), false, nullptr, nullptr);
static struct FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFLightInstanceState
{
	FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFLightInstanceState()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("LightInstanceState")),new UScriptStruct::TCppStructOps<FLightInstanceState>);
	}
} ScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFLightInstanceState;
	struct Z_Construct_UScriptStruct_FLightInstanceState_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_State_MetaData[];
#endif
		static void NewProp_State_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_State;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ComponentName_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ComponentName;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MeshComponent_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_MeshComponent;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLightInstanceState_Statics::Struct_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Generated from translating the IDs of FSignalState for use during runtime\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Generated from translating the IDs of FSignalState for use during runtime" },
	};
#endif
	void* Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FLightInstanceState>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_State_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Whether the lightbulb should be on or not\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Whether the lightbulb should be on or not" },
	};
#endif
	void Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_State_SetBit(void* Obj)
	{
		((FLightInstanceState*)Obj)->State = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_State = { "State", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FLightInstanceState), &Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_State_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_State_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_State_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_ComponentName_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Modified lightbulb name, set during import\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Modified lightbulb name, set during import" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_ComponentName = { "ComponentName", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLightInstanceState, ComponentName), METADATA_PARAMS(Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_ComponentName_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_ComponentName_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_MeshComponent_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Set after first lookup during runtime\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Set after first lookup during runtime" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_MeshComponent = { "MeshComponent", nullptr, (EPropertyFlags)0x0010000000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLightInstanceState, MeshComponent), Z_Construct_UClass_UMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_MeshComponent_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_MeshComponent_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FLightInstanceState_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_State,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_ComponentName,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLightInstanceState_Statics::NewProp_MeshComponent,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FLightInstanceState_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerRuntime,
		nullptr,
		&NewStructOps,
		"LightInstanceState",
		sizeof(FLightInstanceState),
		alignof(FLightInstanceState),
		Z_Construct_UScriptStruct_FLightInstanceState_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightInstanceState_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000005),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FLightInstanceState_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightInstanceState_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FLightInstanceState()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FLightInstanceState_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_RoadRunnerRuntime();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("LightInstanceState"), sizeof(FLightInstanceState), Get_Z_Construct_UScriptStruct_FLightInstanceState_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FLightInstanceState_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FLightInstanceState_Hash() { return 2782839933U; }
class UScriptStruct* FSignalAsset::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern ROADRUNNERRUNTIME_API uint32 Get_Z_Construct_UScriptStruct_FSignalAsset_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FSignalAsset, Z_Construct_UPackage__Script_RoadRunnerRuntime(), TEXT("SignalAsset"), sizeof(FSignalAsset), Get_Z_Construct_UScriptStruct_FSignalAsset_Hash());
	}
	return Singleton;
}
template<> ROADRUNNERRUNTIME_API UScriptStruct* StaticStruct<FSignalAsset>()
{
	return FSignalAsset::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FSignalAsset(FSignalAsset::StaticStruct, TEXT("/Script/RoadRunnerRuntime"), TEXT("SignalAsset"), false, nullptr, nullptr);
static struct FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalAsset
{
	FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalAsset()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("SignalAsset")),new UScriptStruct::TCppStructOps<FSignalAsset>);
	}
} ScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalAsset;
	struct Z_Construct_UScriptStruct_FSignalAsset_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SignalConfigurations_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_SignalConfigurations;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_SignalConfigurations_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Id_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Id;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalAsset_Statics::Struct_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Represents the \"Signal\" element under \"SingalAssets\" in the metadata\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Represents the \"Signal\" element under \"SingalAssets\" in the metadata" },
	};
#endif
	void* Z_Construct_UScriptStruct_FSignalAsset_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FSignalAsset>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_SignalConfigurations_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Whether the lightbulb should be on or not\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Whether the lightbulb should be on or not" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_SignalConfigurations = { "SignalConfigurations", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FSignalAsset, SignalConfigurations), METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_SignalConfigurations_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_SignalConfigurations_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_SignalConfigurations_Inner = { "SignalConfigurations", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FSignalConfiguration, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_Id_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Holds a reference to a lightbulb's static mesh component in the imported scene's actor\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Holds a reference to a lightbulb's static mesh component in the imported scene's actor" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_Id = { "Id", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FSignalAsset, Id), METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_Id_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_Id_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FSignalAsset_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_SignalConfigurations,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_SignalConfigurations_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalAsset_Statics::NewProp_Id,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FSignalAsset_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerRuntime,
		nullptr,
		&NewStructOps,
		"SignalAsset",
		sizeof(FSignalAsset),
		alignof(FSignalAsset),
		Z_Construct_UScriptStruct_FSignalAsset_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalAsset_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalAsset_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalAsset_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FSignalAsset()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FSignalAsset_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_RoadRunnerRuntime();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("SignalAsset"), sizeof(FSignalAsset), Get_Z_Construct_UScriptStruct_FSignalAsset_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FSignalAsset_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FSignalAsset_Hash() { return 535333187U; }
class UScriptStruct* FSignalConfiguration::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern ROADRUNNERRUNTIME_API uint32 Get_Z_Construct_UScriptStruct_FSignalConfiguration_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FSignalConfiguration, Z_Construct_UPackage__Script_RoadRunnerRuntime(), TEXT("SignalConfiguration"), sizeof(FSignalConfiguration), Get_Z_Construct_UScriptStruct_FSignalConfiguration_Hash());
	}
	return Singleton;
}
template<> ROADRUNNERRUNTIME_API UScriptStruct* StaticStruct<FSignalConfiguration>()
{
	return FSignalConfiguration::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FSignalConfiguration(FSignalConfiguration::StaticStruct, TEXT("/Script/RoadRunnerRuntime"), TEXT("SignalConfiguration"), false, nullptr, nullptr);
static struct FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalConfiguration
{
	FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalConfiguration()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("SignalConfiguration")),new UScriptStruct::TCppStructOps<FSignalConfiguration>);
	}
} ScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFSignalConfiguration;
	struct Z_Construct_UScriptStruct_FSignalConfiguration_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LightBulbStates_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_LightBulbStates;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_LightBulbStates_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Name_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Name;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalConfiguration_Statics::Struct_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Represents the \"Configuration\" element in the metadata\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Represents the \"Configuration\" element in the metadata" },
	};
#endif
	void* Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FSignalConfiguration>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_LightBulbStates_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// List of light bulb states\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "List of light bulb states" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_LightBulbStates = { "LightBulbStates", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FSignalConfiguration, LightBulbStates), METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_LightBulbStates_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_LightBulbStates_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_LightBulbStates_Inner = { "LightBulbStates", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FLightBulbState, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_Name_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Name of the configuration.\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Name of the configuration." },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_Name = { "Name", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FSignalConfiguration, Name), METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_Name_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_Name_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FSignalConfiguration_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_LightBulbStates,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_LightBulbStates_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSignalConfiguration_Statics::NewProp_Name,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FSignalConfiguration_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerRuntime,
		nullptr,
		&NewStructOps,
		"SignalConfiguration",
		sizeof(FSignalConfiguration),
		alignof(FSignalConfiguration),
		Z_Construct_UScriptStruct_FSignalConfiguration_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalConfiguration_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FSignalConfiguration_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSignalConfiguration_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FSignalConfiguration()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FSignalConfiguration_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_RoadRunnerRuntime();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("SignalConfiguration"), sizeof(FSignalConfiguration), Get_Z_Construct_UScriptStruct_FSignalConfiguration_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FSignalConfiguration_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FSignalConfiguration_Hash() { return 2447618290U; }
class UScriptStruct* FLightBulbState::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern ROADRUNNERRUNTIME_API uint32 Get_Z_Construct_UScriptStruct_FLightBulbState_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FLightBulbState, Z_Construct_UPackage__Script_RoadRunnerRuntime(), TEXT("LightBulbState"), sizeof(FLightBulbState), Get_Z_Construct_UScriptStruct_FLightBulbState_Hash());
	}
	return Singleton;
}
template<> ROADRUNNERRUNTIME_API UScriptStruct* StaticStruct<FLightBulbState>()
{
	return FLightBulbState::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FLightBulbState(FLightBulbState::StaticStruct, TEXT("/Script/RoadRunnerRuntime"), TEXT("LightBulbState"), false, nullptr, nullptr);
static struct FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFLightBulbState
{
	FScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFLightBulbState()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("LightBulbState")),new UScriptStruct::TCppStructOps<FLightBulbState>);
	}
} ScriptStruct_RoadRunnerRuntime_StaticRegisterNativesFLightBulbState;
	struct Z_Construct_UScriptStruct_FLightBulbState_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_State_MetaData[];
#endif
		static void NewProp_State_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_State;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Name_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Name;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLightBulbState_Statics::Struct_MetaDataParams[] = {
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// Represents the \"LightState\" element in the metadata\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Represents the \"LightState\" element in the metadata" },
	};
#endif
	void* Z_Construct_UScriptStruct_FLightBulbState_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FLightBulbState>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_State_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Whether the lightbulb should be on or not\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Whether the lightbulb should be on or not" },
	};
#endif
	void Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_State_SetBit(void* Obj)
	{
		((FLightBulbState*)Obj)->State = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_State = { "State", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FLightBulbState), &Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_State_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_State_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_State_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_Name_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Original name of the lightbulb mesh\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Original name of the lightbulb mesh" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_Name = { "Name", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLightBulbState, Name), METADATA_PARAMS(Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_Name_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_Name_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FLightBulbState_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_State,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLightBulbState_Statics::NewProp_Name,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FLightBulbState_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerRuntime,
		nullptr,
		&NewStructOps,
		"LightBulbState",
		sizeof(FLightBulbState),
		alignof(FLightBulbState),
		Z_Construct_UScriptStruct_FLightBulbState_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightBulbState_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FLightBulbState_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLightBulbState_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FLightBulbState()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FLightBulbState_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_RoadRunnerRuntime();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("LightBulbState"), sizeof(FLightBulbState), Get_Z_Construct_UScriptStruct_FLightBulbState_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FLightBulbState_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FLightBulbState_Hash() { return 453924255U; }
	void URoadRunnerTrafficJunction::StaticRegisterNativesURoadRunnerTrafficJunction()
	{
	}
	UClass* Z_Construct_UClass_URoadRunnerTrafficJunction_NoRegister()
	{
		return URoadRunnerTrafficJunction::StaticClass();
	}
	struct Z_Construct_UClass_URoadRunnerTrafficJunction_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CurrentInterval_MetaData[];
#endif
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_CurrentInterval;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CurrentPhase_MetaData[];
#endif
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_CurrentPhase;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Timer_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Timer;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Phases_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Phases;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Phases_Inner;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_RoadRunnerRuntime,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "Comment", "////////////////////////////////////////////////////////////////////////////////\n// URoadRunnerTrafficJunction handles the behaviour of each junction to \n// change the traffic lights\n" },
		{ "IncludePath", "RoadRunnerTrafficJunction.h" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "URoadRunnerTrafficJunction handles the behaviour of each junction to\nchange the traffic lights" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_CurrentInterval_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Keeps track of which phase the junction is on\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Keeps track of which phase the junction is on" },
	};
#endif
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_CurrentInterval = { "CurrentInterval", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(URoadRunnerTrafficJunction, CurrentInterval), METADATA_PARAMS(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_CurrentInterval_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_CurrentInterval_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_CurrentPhase_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Keeps track of which phase the junction is on\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Keeps track of which phase the junction is on" },
	};
#endif
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_CurrentPhase = { "CurrentPhase", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(URoadRunnerTrafficJunction, CurrentPhase), METADATA_PARAMS(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_CurrentPhase_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_CurrentPhase_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Timer_MetaData[] = {
		{ "Comment", "// Timer to keep track of interval switches\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Timer to keep track of interval switches" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Timer = { "Timer", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(URoadRunnerTrafficJunction, Timer), METADATA_PARAMS(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Timer_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Timer_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Phases_MetaData[] = {
		{ "Category", "Traffic" },
		{ "Comment", "// Holds a list of phases to loop through (i.e. Traffic going east/west -> Traffic going north/south -> Left Turns)\n" },
		{ "ModuleRelativePath", "Public/RoadRunnerTrafficJunction.h" },
		{ "ToolTip", "Holds a list of phases to loop through (i.e. Traffic going east/west -> Traffic going north/south -> Left Turns)" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Phases = { "Phases", nullptr, (EPropertyFlags)0x0010008000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(URoadRunnerTrafficJunction, Phases), METADATA_PARAMS(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Phases_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Phases_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Phases_Inner = { "Phases", nullptr, (EPropertyFlags)0x0000008000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FSignalPhase, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_CurrentInterval,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_CurrentPhase,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Timer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Phases,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::NewProp_Phases_Inner,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URoadRunnerTrafficJunction>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::ClassParams = {
		&URoadRunnerTrafficJunction::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_URoadRunnerTrafficJunction()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_URoadRunnerTrafficJunction_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(URoadRunnerTrafficJunction, 427238855);
	template<> ROADRUNNERRUNTIME_API UClass* StaticClass<URoadRunnerTrafficJunction>()
	{
		return URoadRunnerTrafficJunction::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_URoadRunnerTrafficJunction(Z_Construct_UClass_URoadRunnerTrafficJunction, &URoadRunnerTrafficJunction::StaticClass, TEXT("/Script/RoadRunnerRuntime"), TEXT("URoadRunnerTrafficJunction"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URoadRunnerTrafficJunction);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
