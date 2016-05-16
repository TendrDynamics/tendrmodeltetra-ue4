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

#include "TendrModelTetraGeneratorActor.generated.h"

//
// ATendrModelTetraGeneratorActor
//
// Unreal actor class wrapper for UTendrModelTetraComponent
//
UCLASS( hidecategories = ( Input, Collision, Replication ), meta = ( DisplayName = "Tendr Tetrahedral ModelData" ) )
class ATendrModelTetraGeneratorActor : public AActor
{
	GENERATED_UCLASS_BODY( )

	UPROPERTY( Category = Physics, VisibleAnywhere, BlueprintReadOnly )
	class UTendrModelTetraGeneratorComponent *TendrModelDataComponent;
};
