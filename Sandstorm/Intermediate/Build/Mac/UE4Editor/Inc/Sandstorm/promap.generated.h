// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectBase.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef SANDSTORM_promap_generated_h
#error "promap.generated.h already included, missing '#pragma once' in promap.h"
#endif
#define SANDSTORM_promap_generated_h

#define Sandstorm_Source_Sandstorm_promap_promap_h_13_RPC_WRAPPERS
#define Sandstorm_Source_Sandstorm_promap_promap_h_13_RPC_WRAPPERS_NO_PURE_DECLS
#define Sandstorm_Source_Sandstorm_promap_promap_h_13_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesApromap(); \
	friend SANDSTORM_API class UClass* Z_Construct_UClass_Apromap(); \
	public: \
	DECLARE_CLASS(Apromap, AActor, COMPILED_IN_FLAGS(0), 0, Sandstorm, NO_API) \
	DECLARE_SERIALIZER(Apromap) \
	/** Indicates whether the class is compiled into the engine */    enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	virtual UObject* _getUObject() const override { return const_cast<Apromap*>(this); }


#define Sandstorm_Source_Sandstorm_promap_promap_h_13_INCLASS \
	private: \
	static void StaticRegisterNativesApromap(); \
	friend SANDSTORM_API class UClass* Z_Construct_UClass_Apromap(); \
	public: \
	DECLARE_CLASS(Apromap, AActor, COMPILED_IN_FLAGS(0), 0, Sandstorm, NO_API) \
	DECLARE_SERIALIZER(Apromap) \
	/** Indicates whether the class is compiled into the engine */    enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	virtual UObject* _getUObject() const override { return const_cast<Apromap*>(this); }


#define Sandstorm_Source_Sandstorm_promap_promap_h_13_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API Apromap(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(Apromap) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, Apromap); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(Apromap); \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API Apromap(const Apromap& InCopy); \
public:


#define Sandstorm_Source_Sandstorm_promap_promap_h_13_ENHANCED_CONSTRUCTORS \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API Apromap(const Apromap& InCopy); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, Apromap); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(Apromap); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(Apromap)


#define Sandstorm_Source_Sandstorm_promap_promap_h_10_PROLOG
#define Sandstorm_Source_Sandstorm_promap_promap_h_13_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Sandstorm_Source_Sandstorm_promap_promap_h_13_RPC_WRAPPERS \
	Sandstorm_Source_Sandstorm_promap_promap_h_13_INCLASS \
	Sandstorm_Source_Sandstorm_promap_promap_h_13_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Sandstorm_Source_Sandstorm_promap_promap_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Sandstorm_Source_Sandstorm_promap_promap_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	Sandstorm_Source_Sandstorm_promap_promap_h_13_INCLASS_NO_PURE_DECLS \
	Sandstorm_Source_Sandstorm_promap_promap_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Sandstorm_Source_Sandstorm_promap_promap_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
