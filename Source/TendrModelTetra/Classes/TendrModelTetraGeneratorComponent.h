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

#include "Common/TendrModelData.h"
#include "TendrModelTetraGeneratorComponent.generated.h"

//
// UTendrModelTetraGeneratorComponent
//
// Unreal component class for Tendr-compatible container objects with tetrahedral model data.
// Implements the base ITendrModelData interface for use with the TendrDynamics plugin.
//
UCLASS( CollapseCategories, HideCategories = Object )
class UTendrModelTetraGeneratorComponent : public USceneComponent, public ITendrModelGenerator
{
	GENERATED_UCLASS_BODY( )

public:

	// Begin UObject interface.
#if WITH_EDITOR
	virtual void PreEditChange( UProperty* PropertyAboutToChange ) override;
	virtual void PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent ) override;
#endif // WITH_EDITOR
	virtual void BeginDestroy() override;
	virtual void GetAssetRegistryTags( TArray<FAssetRegistryTag>& OutTags ) const override;
	virtual FString GetDesc() override;
	// End UObject interface.

	// Begin UActorComponent interface.
	virtual void OnRegister() override;
	// End UActorComponent interface.

	// Begin ITendrModelData interface.
	virtual FTendrModelData Build( const FTendrVertexArray& InputVertices, const FTendrIndexArray& InputIndices, const FTendrTexCoordArray( &InputTexCoords )[ MAX_TEXCOORDS ], bool bSilent ) override;
	virtual FString GetLastError() override;
	// End ITendrModelData interface.

	// Exposes the TendrModelGenerator version so the TendrDynamics plugin can detect this as a compatible class.
	UPROPERTY()
	uint32 TendrModelGeneratorVersion;

	// Maximum volume constraint for tetrahedra in the generated model
	UPROPERTY( EditAnywhere, NoClear, BlueprintReadWrite, Category = "Tendr Model", meta = ( UIMin = 0, ClampMin = 0 ) )
	float MaximumTetraVolume;

private:
	/** Sets the last error **/
	void SetError( FString Text );

	/** Adds a neighbour to the connectivity structure **/
	bool ConnectivityAddNeighbour( FTendrModelData& ModelData, uint32 Src, uint32 Dst, uint32& MaxNeighbours );

	/** Last error **/
	FString LastError;
};
