// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ObstacleScannerComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UGridMapComponent;
#ifdef DRONE_ObstacleScannerComponent_generated_h
#error "ObstacleScannerComponent.generated.h already included, missing '#pragma once' in ObstacleScannerComponent.h"
#endif
#define DRONE_ObstacleScannerComponent_generated_h

#define FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execIsLocationOccupied); \
	DECLARE_FUNCTION(execGetGroundHeight); \
	DECLARE_FUNCTION(execScanArea); \
	DECLARE_FUNCTION(execSetGridMap);


#define FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUObstacleScannerComponent(); \
	friend struct Z_Construct_UClass_UObstacleScannerComponent_Statics; \
public: \
	DECLARE_CLASS(UObstacleScannerComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Drone"), NO_API) \
	DECLARE_SERIALIZER(UObstacleScannerComponent)


#define FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UObstacleScannerComponent(UObstacleScannerComponent&&); \
	UObstacleScannerComponent(const UObstacleScannerComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UObstacleScannerComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UObstacleScannerComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UObstacleScannerComponent) \
	NO_API virtual ~UObstacleScannerComponent();


#define FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_9_PROLOG
#define FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_12_INCLASS_NO_PURE_DECLS \
	FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> DRONE_API UClass* StaticClass<class UObstacleScannerComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_ue5_Drone_Source_Drone_ObstacleScannerComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
