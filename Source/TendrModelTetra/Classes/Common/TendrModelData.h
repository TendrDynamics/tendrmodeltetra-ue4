///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TendrModelTetraPlugin                                                     //
//                                                                           //
// A Tendr model generator plugin for use with Unreal Engine in combination  //
// with the Tendr Dynamics soft-body physics plugin.                         //
//                                                                           //
// Version 1.0                                                               //
// January, 2015                                                             //
//                                                                           //
// Copyright (C) 2014-2015, Tendr Dynamics B.V.                              //
//                                                                           //
// Visit http://tendrdynamics.com for more information.                      //
//                                                                           //
// This file is governed by copyrights as described in the LICENSE file.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Common/TendrCommon.h>

//
// Public interface for TendrDynamics plugin
//
// This interface is intentionally decoupled from UE's interface and class system,
// as we need cross-plugin functionality without any form of binary or object linkage.
//
class ITendrModelGenerator
{
public:
	// Constant representing an invalid index value
	static const uint32 InvalidIndex = ~0;

	// Builds data for this model
	virtual FTendrModelData Build( const FTendrVertexArray& InputVertices, const FTendrIndexArray& InputIndices, const FTendrTexCoordArray( &InputTexCoords )[ MAX_TEXCOORDS ], bool bSilent ) = 0;

	// Returns the last error that occurred during building
	virtual FString GetLastError() = 0;
};

//
// Dummy class that mirrors UTendrModelTetraDataComponent's C++ vtable layout for hard cross-plugin static casting
//
class ITendrModelTetraGeneratorComponent : public USceneComponent, public ITendrModelGenerator
{
	ITendrModelTetraGeneratorComponent();

	//
	// This class requires the TendrModelGeneratorVersion UPROPERTY for proper detection.
	//
};
