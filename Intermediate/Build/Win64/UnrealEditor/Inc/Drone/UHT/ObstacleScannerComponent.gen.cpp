// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Drone/ObstacleScannerComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeObstacleScannerComponent() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
DRONE_API UClass* Z_Construct_UClass_UGridMapComponent_NoRegister();
DRONE_API UClass* Z_Construct_UClass_UObstacleScannerComponent();
DRONE_API UClass* Z_Construct_UClass_UObstacleScannerComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
UPackage* Z_Construct_UPackage__Script_Drone();
// End Cross Module References

// Begin Class UObstacleScannerComponent Function GetGroundHeight
struct Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics
{
	struct ObstacleScannerComponent_eventGetGroundHeight_Parms
	{
		FVector Location;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "PathPlanning|Scanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x8e\xb7\xe5\x8f\x96\xe5\x9c\xb0\xe9\x9d\xa2\xe9\xab\x98\xe5\xba\xa6\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x8e\xb7\xe5\x8f\x96\xe5\x9c\xb0\xe9\x9d\xa2\xe9\xab\x98\xe5\xba\xa6" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Location_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Location;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::NewProp_Location = { "Location", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ObstacleScannerComponent_eventGetGroundHeight_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Location_MetaData), NewProp_Location_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ObstacleScannerComponent_eventGetGroundHeight_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::NewProp_Location,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UObstacleScannerComponent, nullptr, "GetGroundHeight", nullptr, nullptr, Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::PropPointers), sizeof(Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::ObstacleScannerComponent_eventGetGroundHeight_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::Function_MetaDataParams), Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::ObstacleScannerComponent_eventGetGroundHeight_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UObstacleScannerComponent::execGetGroundHeight)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_Location);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=P_THIS->GetGroundHeight(Z_Param_Out_Location);
	P_NATIVE_END;
}
// End Class UObstacleScannerComponent Function GetGroundHeight

// Begin Class UObstacleScannerComponent Function IsLocationOccupied
struct Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics
{
	struct ObstacleScannerComponent_eventIsLocationOccupied_Parms
	{
		FVector Location;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "PathPlanning|Scanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xa3\x80\xe6\x9f\xa5\xe4\xbd\x8d\xe7\xbd\xae\xe6\x98\xaf\xe5\x90\xa6\xe8\xa2\xab\xe5\x8d\xa0\xe7\x94\xa8\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xa3\x80\xe6\x9f\xa5\xe4\xbd\x8d\xe7\xbd\xae\xe6\x98\xaf\xe5\x90\xa6\xe8\xa2\xab\xe5\x8d\xa0\xe7\x94\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Location_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Location;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::NewProp_Location = { "Location", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ObstacleScannerComponent_eventIsLocationOccupied_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Location_MetaData), NewProp_Location_MetaData) };
void Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ObstacleScannerComponent_eventIsLocationOccupied_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ObstacleScannerComponent_eventIsLocationOccupied_Parms), &Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::NewProp_Location,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UObstacleScannerComponent, nullptr, "IsLocationOccupied", nullptr, nullptr, Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::PropPointers), sizeof(Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::ObstacleScannerComponent_eventIsLocationOccupied_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::Function_MetaDataParams), Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::ObstacleScannerComponent_eventIsLocationOccupied_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UObstacleScannerComponent::execIsLocationOccupied)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_Location);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsLocationOccupied(Z_Param_Out_Location);
	P_NATIVE_END;
}
// End Class UObstacleScannerComponent Function IsLocationOccupied

// Begin Class UObstacleScannerComponent Function ScanArea
struct Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics
{
	struct ObstacleScannerComponent_eventScanArea_Parms
	{
		FVector Center;
		float Radius;
		float Height;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "PathPlanning|Scanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\xab\xe6\x8f\x8f\xe6\x8c\x87\xe5\xae\x9a\xe5\x8c\xba\xe5\x9f\x9f\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\xab\xe6\x8f\x8f\xe6\x8c\x87\xe5\xae\x9a\xe5\x8c\xba\xe5\x9f\x9f" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Center_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Center;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Radius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Height;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::NewProp_Center = { "Center", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ObstacleScannerComponent_eventScanArea_Parms, Center), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Center_MetaData), NewProp_Center_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::NewProp_Radius = { "Radius", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ObstacleScannerComponent_eventScanArea_Parms, Radius), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::NewProp_Height = { "Height", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ObstacleScannerComponent_eventScanArea_Parms, Height), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::NewProp_Center,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::NewProp_Radius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::NewProp_Height,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UObstacleScannerComponent, nullptr, "ScanArea", nullptr, nullptr, Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::PropPointers), sizeof(Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::ObstacleScannerComponent_eventScanArea_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::Function_MetaDataParams), Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::ObstacleScannerComponent_eventScanArea_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UObstacleScannerComponent_ScanArea()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UObstacleScannerComponent_ScanArea_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UObstacleScannerComponent::execScanArea)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_Center);
	P_GET_PROPERTY(FFloatProperty,Z_Param_Radius);
	P_GET_PROPERTY(FFloatProperty,Z_Param_Height);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ScanArea(Z_Param_Out_Center,Z_Param_Radius,Z_Param_Height);
	P_NATIVE_END;
}
// End Class UObstacleScannerComponent Function ScanArea

// Begin Class UObstacleScannerComponent Function SetGridMap
struct Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics
{
	struct ObstacleScannerComponent_eventSetGridMap_Parms
	{
		UGridMapComponent* InGridMap;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "PathPlanning|Scanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xbe\xe7\xbd\xae\xe7\xbd\x91\xe6\xa0\xbc\xe5\x9c\xb0\xe5\x9b\xbe\xe7\xbb\x84\xe4\xbb\xb6\xe5\xbc\x95\xe7\x94\xa8\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xbe\xe7\xbd\xae\xe7\xbd\x91\xe6\xa0\xbc\xe5\x9c\xb0\xe5\x9b\xbe\xe7\xbb\x84\xe4\xbb\xb6\xe5\xbc\x95\xe7\x94\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InGridMap_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InGridMap;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::NewProp_InGridMap = { "InGridMap", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ObstacleScannerComponent_eventSetGridMap_Parms, InGridMap), Z_Construct_UClass_UGridMapComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InGridMap_MetaData), NewProp_InGridMap_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::NewProp_InGridMap,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UObstacleScannerComponent, nullptr, "SetGridMap", nullptr, nullptr, Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::PropPointers), sizeof(Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::ObstacleScannerComponent_eventSetGridMap_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::Function_MetaDataParams), Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::ObstacleScannerComponent_eventSetGridMap_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UObstacleScannerComponent::execSetGridMap)
{
	P_GET_OBJECT(UGridMapComponent,Z_Param_InGridMap);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetGridMap(Z_Param_InGridMap);
	P_NATIVE_END;
}
// End Class UObstacleScannerComponent Function SetGridMap

// Begin Class UObstacleScannerComponent
void UObstacleScannerComponent::StaticRegisterNativesUObstacleScannerComponent()
{
	UClass* Class = UObstacleScannerComponent::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetGroundHeight", &UObstacleScannerComponent::execGetGroundHeight },
		{ "IsLocationOccupied", &UObstacleScannerComponent::execIsLocationOccupied },
		{ "ScanArea", &UObstacleScannerComponent::execScanArea },
		{ "SetGridMap", &UObstacleScannerComponent::execSetGridMap },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UObstacleScannerComponent);
UClass* Z_Construct_UClass_UObstacleScannerComponent_NoRegister()
{
	return UObstacleScannerComponent::StaticClass();
}
struct Z_Construct_UClass_UObstacleScannerComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "PathPlanning" },
		{ "IncludePath", "ObstacleScannerComponent.h" },
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScanRadius_MetaData[] = {
		{ "Category", "PathPlanning|Scanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\xab\xe6\x8f\x8f\xe5\x8f\x82\xe6\x95\xb0\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\xab\xe6\x8f\x8f\xe5\x8f\x82\xe6\x95\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScanHeight_MetaData[] = {
		{ "Category", "PathPlanning|Scanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\xab\xe6\x8f\x8f\xe5\x8d\x8a\xe5\xbe\x84\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\xab\xe6\x8f\x8f\xe5\x8d\x8a\xe5\xbe\x84" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RaycastSpacing_MetaData[] = {
		{ "Category", "PathPlanning|Scanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\xab\xe6\x8f\x8f\xe9\xab\x98\xe5\xba\xa6\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\xab\xe6\x8f\x8f\xe9\xab\x98\xe5\xba\xa6" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GroundCheckDistance_MetaData[] = {
		{ "Category", "PathPlanning|Scanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xb0\x84\xe7\xba\xbf\xe9\x97\xb4\xe8\xb7\x9d\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xb0\x84\xe7\xba\xbf\xe9\x97\xb4\xe8\xb7\x9d" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bShowDebugVisualization_MetaData[] = {
		{ "Category", "PathPlanning|Scanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xb0\x83\xe8\xaf\x95\xe5\x8f\xaf\xe8\xa7\x86\xe5\x8c\x96\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xb0\x83\xe8\xaf\x95\xe5\x8f\xaf\xe8\xa7\x86\xe5\x8c\x96" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScanInterval_MetaData[] = {
		{ "Category", "ObstacleScanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\xab\xe6\x8f\x8f\xe9\x97\xb4\xe9\x9a\x94\xef\xbc\x88\xe7\xa7\x92\xef\xbc\x89\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\xab\xe6\x8f\x8f\xe9\x97\xb4\xe9\x9a\x94\xef\xbc\x88\xe7\xa7\x92\xef\xbc\x89" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAutoScan_MetaData[] = {
		{ "Category", "ObstacleScanner" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x87\xaa\xe5\x8a\xa8\xe6\x89\xab\xe6\x8f\x8f\xe5\xbc\x80\xe5\x85\xb3\n" },
#endif
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x87\xaa\xe5\x8a\xa8\xe6\x89\xab\xe6\x8f\x8f\xe5\xbc\x80\xe5\x85\xb3" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GridMap_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbd\x91\xe6\xa0\xbc\xe5\x9c\xb0\xe5\x9b\xbe\xe7\xbb\x84\xe4\xbb\xb6\xe5\xbc\x95\xe7\x94\xa8\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ObstacleScannerComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbd\x91\xe6\xa0\xbc\xe5\x9c\xb0\xe5\x9b\xbe\xe7\xbb\x84\xe4\xbb\xb6\xe5\xbc\x95\xe7\x94\xa8" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ScanRadius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ScanHeight;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RaycastSpacing;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_GroundCheckDistance;
	static void NewProp_bShowDebugVisualization_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bShowDebugVisualization;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ScanInterval;
	static void NewProp_bAutoScan_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAutoScan;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_GridMap;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UObstacleScannerComponent_GetGroundHeight, "GetGroundHeight" }, // 27492531
		{ &Z_Construct_UFunction_UObstacleScannerComponent_IsLocationOccupied, "IsLocationOccupied" }, // 1355129274
		{ &Z_Construct_UFunction_UObstacleScannerComponent_ScanArea, "ScanArea" }, // 865696151
		{ &Z_Construct_UFunction_UObstacleScannerComponent_SetGridMap, "SetGridMap" }, // 1761521519
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UObstacleScannerComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_ScanRadius = { "ScanRadius", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UObstacleScannerComponent, ScanRadius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScanRadius_MetaData), NewProp_ScanRadius_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_ScanHeight = { "ScanHeight", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UObstacleScannerComponent, ScanHeight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScanHeight_MetaData), NewProp_ScanHeight_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_RaycastSpacing = { "RaycastSpacing", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UObstacleScannerComponent, RaycastSpacing), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RaycastSpacing_MetaData), NewProp_RaycastSpacing_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_GroundCheckDistance = { "GroundCheckDistance", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UObstacleScannerComponent, GroundCheckDistance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GroundCheckDistance_MetaData), NewProp_GroundCheckDistance_MetaData) };
void Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_bShowDebugVisualization_SetBit(void* Obj)
{
	((UObstacleScannerComponent*)Obj)->bShowDebugVisualization = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_bShowDebugVisualization = { "bShowDebugVisualization", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UObstacleScannerComponent), &Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_bShowDebugVisualization_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bShowDebugVisualization_MetaData), NewProp_bShowDebugVisualization_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_ScanInterval = { "ScanInterval", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UObstacleScannerComponent, ScanInterval), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScanInterval_MetaData), NewProp_ScanInterval_MetaData) };
void Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_bAutoScan_SetBit(void* Obj)
{
	((UObstacleScannerComponent*)Obj)->bAutoScan = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_bAutoScan = { "bAutoScan", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UObstacleScannerComponent), &Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_bAutoScan_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAutoScan_MetaData), NewProp_bAutoScan_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_GridMap = { "GridMap", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UObstacleScannerComponent, GridMap), Z_Construct_UClass_UGridMapComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GridMap_MetaData), NewProp_GridMap_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UObstacleScannerComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_ScanRadius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_ScanHeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_RaycastSpacing,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_GroundCheckDistance,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_bShowDebugVisualization,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_ScanInterval,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_bAutoScan,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UObstacleScannerComponent_Statics::NewProp_GridMap,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UObstacleScannerComponent_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UObstacleScannerComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_Drone,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UObstacleScannerComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UObstacleScannerComponent_Statics::ClassParams = {
	&UObstacleScannerComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UObstacleScannerComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UObstacleScannerComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UObstacleScannerComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UObstacleScannerComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UObstacleScannerComponent()
{
	if (!Z_Registration_Info_UClass_UObstacleScannerComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UObstacleScannerComponent.OuterSingleton, Z_Construct_UClass_UObstacleScannerComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UObstacleScannerComponent.OuterSingleton;
}
template<> DRONE_API UClass* StaticClass<UObstacleScannerComponent>()
{
	return UObstacleScannerComponent::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UObstacleScannerComponent);
UObstacleScannerComponent::~UObstacleScannerComponent() {}
// End Class UObstacleScannerComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UObstacleScannerComponent, UObstacleScannerComponent::StaticClass, TEXT("UObstacleScannerComponent"), &Z_Registration_Info_UClass_UObstacleScannerComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UObstacleScannerComponent), 1125523706U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_3166610258(TEXT("/Script/Drone"),
	Z_CompiledInDeferFile_FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
