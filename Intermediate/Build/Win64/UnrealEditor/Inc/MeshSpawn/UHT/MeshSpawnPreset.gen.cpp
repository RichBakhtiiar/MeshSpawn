// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MeshSpawn/Public/MeshSpawnPreset.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMeshSpawnPreset() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UDataAsset();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMesh_NoRegister();
	MESHSPAWN_API UClass* Z_Construct_UClass_UMeshSpawnPreset();
	MESHSPAWN_API UClass* Z_Construct_UClass_UMeshSpawnPreset_Factory();
	MESHSPAWN_API UClass* Z_Construct_UClass_UMeshSpawnPreset_Factory_NoRegister();
	MESHSPAWN_API UClass* Z_Construct_UClass_UMeshSpawnPreset_NoRegister();
	MESHSPAWN_API UScriptStruct* Z_Construct_UScriptStruct_FPLPaintObject();
	UNREALED_API UClass* Z_Construct_UClass_UFactory();
	UPackage* Z_Construct_UPackage__Script_MeshSpawn();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_PLPaintObject;
class UScriptStruct* FPLPaintObject::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_PLPaintObject.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_PLPaintObject.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FPLPaintObject, (UObject*)Z_Construct_UPackage__Script_MeshSpawn(), TEXT("PLPaintObject"));
	}
	return Z_Registration_Info_UScriptStruct_PLPaintObject.OuterSingleton;
}
template<> MESHSPAWN_API UScriptStruct* StaticStruct<FPLPaintObject>()
{
	return FPLPaintObject::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FPLPaintObject_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_StaticNesh_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_StaticNesh;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Chance_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Chance;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FPLPaintObject_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "ModuleRelativePath", "Public/MeshSpawnPreset.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPLPaintObject>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewProp_StaticNesh_MetaData[] = {
		{ "Category", "Preset" },
		{ "ModuleRelativePath", "Public/MeshSpawnPreset.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewProp_StaticNesh = { "StaticNesh", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FPLPaintObject, StaticNesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewProp_StaticNesh_MetaData), Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewProp_StaticNesh_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewProp_Chance_MetaData[] = {
		{ "Category", "Preset" },
		{ "ModuleRelativePath", "Public/MeshSpawnPreset.h" },
		{ "UIMax", "100" },
		{ "UIMin", "0" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewProp_Chance = { "Chance", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FPLPaintObject, Chance), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewProp_Chance_MetaData), Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewProp_Chance_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FPLPaintObject_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewProp_StaticNesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewProp_Chance,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FPLPaintObject_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_MeshSpawn,
		nullptr,
		&NewStructOps,
		"PLPaintObject",
		Z_Construct_UScriptStruct_FPLPaintObject_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPLPaintObject_Statics::PropPointers),
		sizeof(FPLPaintObject),
		alignof(FPLPaintObject),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000201),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPLPaintObject_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FPLPaintObject_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPLPaintObject_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FPLPaintObject()
	{
		if (!Z_Registration_Info_UScriptStruct_PLPaintObject.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_PLPaintObject.InnerSingleton, Z_Construct_UScriptStruct_FPLPaintObject_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_PLPaintObject.InnerSingleton;
	}
	void UMeshSpawnPreset::StaticRegisterNativesUMeshSpawnPreset()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMeshSpawnPreset);
	UClass* Z_Construct_UClass_UMeshSpawnPreset_NoRegister()
	{
		return UMeshSpawnPreset::StaticClass();
	}
	struct Z_Construct_UClass_UMeshSpawnPreset_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_PaintObjects_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PaintObjects_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_PaintObjects;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UMeshSpawnPreset_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UDataAsset,
		(UObject* (*)())Z_Construct_UPackage__Script_MeshSpawn,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMeshSpawnPreset_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMeshSpawnPreset_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "MeshSpawnPreset.h" },
		{ "ModuleRelativePath", "Public/MeshSpawnPreset.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UMeshSpawnPreset_Statics::NewProp_PaintObjects_Inner = { "PaintObjects", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FPLPaintObject, METADATA_PARAMS(0, nullptr) }; // 3426203541
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMeshSpawnPreset_Statics::NewProp_PaintObjects_MetaData[] = {
		{ "Category", "Preset" },
		{ "ModuleRelativePath", "Public/MeshSpawnPreset.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UMeshSpawnPreset_Statics::NewProp_PaintObjects = { "PaintObjects", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMeshSpawnPreset, PaintObjects), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMeshSpawnPreset_Statics::NewProp_PaintObjects_MetaData), Z_Construct_UClass_UMeshSpawnPreset_Statics::NewProp_PaintObjects_MetaData) }; // 3426203541
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UMeshSpawnPreset_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMeshSpawnPreset_Statics::NewProp_PaintObjects_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMeshSpawnPreset_Statics::NewProp_PaintObjects,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UMeshSpawnPreset_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMeshSpawnPreset>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UMeshSpawnPreset_Statics::ClassParams = {
		&UMeshSpawnPreset::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UMeshSpawnPreset_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UMeshSpawnPreset_Statics::PropPointers),
		0,
		0x001000A0u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMeshSpawnPreset_Statics::Class_MetaDataParams), Z_Construct_UClass_UMeshSpawnPreset_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMeshSpawnPreset_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UMeshSpawnPreset()
	{
		if (!Z_Registration_Info_UClass_UMeshSpawnPreset.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMeshSpawnPreset.OuterSingleton, Z_Construct_UClass_UMeshSpawnPreset_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UMeshSpawnPreset.OuterSingleton;
	}
	template<> MESHSPAWN_API UClass* StaticClass<UMeshSpawnPreset>()
	{
		return UMeshSpawnPreset::StaticClass();
	}
	UMeshSpawnPreset::UMeshSpawnPreset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UMeshSpawnPreset);
	UMeshSpawnPreset::~UMeshSpawnPreset() {}
	void UMeshSpawnPreset_Factory::StaticRegisterNativesUMeshSpawnPreset_Factory()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMeshSpawnPreset_Factory);
	UClass* Z_Construct_UClass_UMeshSpawnPreset_Factory_NoRegister()
	{
		return UMeshSpawnPreset_Factory::StaticClass();
	}
	struct Z_Construct_UClass_UMeshSpawnPreset_Factory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UMeshSpawnPreset_Factory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_MeshSpawn,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMeshSpawnPreset_Factory_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMeshSpawnPreset_Factory_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MeshSpawnPreset.h" },
		{ "ModuleRelativePath", "Public/MeshSpawnPreset.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UMeshSpawnPreset_Factory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMeshSpawnPreset_Factory>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UMeshSpawnPreset_Factory_Statics::ClassParams = {
		&UMeshSpawnPreset_Factory::StaticClass,
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
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMeshSpawnPreset_Factory_Statics::Class_MetaDataParams), Z_Construct_UClass_UMeshSpawnPreset_Factory_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_UMeshSpawnPreset_Factory()
	{
		if (!Z_Registration_Info_UClass_UMeshSpawnPreset_Factory.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMeshSpawnPreset_Factory.OuterSingleton, Z_Construct_UClass_UMeshSpawnPreset_Factory_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UMeshSpawnPreset_Factory.OuterSingleton;
	}
	template<> MESHSPAWN_API UClass* StaticClass<UMeshSpawnPreset_Factory>()
	{
		return UMeshSpawnPreset_Factory::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UMeshSpawnPreset_Factory);
	UMeshSpawnPreset_Factory::~UMeshSpawnPreset_Factory() {}
	struct Z_CompiledInDeferFile_FID_Workspace_MeshSpawnTest_Plugins_MeshSpawn_Source_MeshSpawn_Public_MeshSpawnPreset_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Workspace_MeshSpawnTest_Plugins_MeshSpawn_Source_MeshSpawn_Public_MeshSpawnPreset_h_Statics::ScriptStructInfo[] = {
		{ FPLPaintObject::StaticStruct, Z_Construct_UScriptStruct_FPLPaintObject_Statics::NewStructOps, TEXT("PLPaintObject"), &Z_Registration_Info_UScriptStruct_PLPaintObject, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FPLPaintObject), 3426203541U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Workspace_MeshSpawnTest_Plugins_MeshSpawn_Source_MeshSpawn_Public_MeshSpawnPreset_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UMeshSpawnPreset, UMeshSpawnPreset::StaticClass, TEXT("UMeshSpawnPreset"), &Z_Registration_Info_UClass_UMeshSpawnPreset, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMeshSpawnPreset), 2196050830U) },
		{ Z_Construct_UClass_UMeshSpawnPreset_Factory, UMeshSpawnPreset_Factory::StaticClass, TEXT("UMeshSpawnPreset_Factory"), &Z_Registration_Info_UClass_UMeshSpawnPreset_Factory, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMeshSpawnPreset_Factory), 2222235016U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Workspace_MeshSpawnTest_Plugins_MeshSpawn_Source_MeshSpawn_Public_MeshSpawnPreset_h_2079648515(TEXT("/Script/MeshSpawn"),
		Z_CompiledInDeferFile_FID_Workspace_MeshSpawnTest_Plugins_MeshSpawn_Source_MeshSpawn_Public_MeshSpawnPreset_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Workspace_MeshSpawnTest_Plugins_MeshSpawn_Source_MeshSpawn_Public_MeshSpawnPreset_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_Workspace_MeshSpawnTest_Plugins_MeshSpawn_Source_MeshSpawn_Public_MeshSpawnPreset_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Workspace_MeshSpawnTest_Plugins_MeshSpawn_Source_MeshSpawn_Public_MeshSpawnPreset_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
