///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TendrModelTetraPlugin                                                     //
//                                                                           //
// A Tendr model generator plugin for use with Unreal Engine in combination  //
// with the Tendr Dynamics soft-body physics plugin.                         //
//                                                                           //
// Version 1.1                                                               //
// April, 2016                                                               //
//                                                                           //
// Copyright (C) 2014-2015, Tendr Dynamics B.V.                              //
//                                                                           //
// Visit http://tendrdynamics.com for more information.                      //
//                                                                           //
// This file is governed by copyrights as described in the LICENSE file.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

//
// FTendrVertexConnectivityData
//
// Structure for simple vertex connectivity bookkeeping.
//
class FTendrVertexConnectivityData
{
public:
	// Constant representing the maximum amount of neighbour indices
	static const uint32 MaxNeighbours = 52;

	// Array of neighbour indices
	uint32 NeighbourIndices[ MaxNeighbours ];
};

//
// FTendrTangent
//
// Structure for X and Z tangent storage.
//
struct FTendrTangent
{
	FPackedNormal TangentX;
	FPackedNormal TangentZ;
};

//
// Common Tendr type definitions
//
typedef TArray<uint32> FTendrIndexArray;
typedef TArray<uint32> FTendrCoarseSparseMapping;
typedef TArray<FVector4> FTendrVertexArray;
typedef TArray<class FTendrVertexConnectivityData> FTendrConnectivityArray;
typedef TArray<uint32> FTendrTetrahedronFaceIndexArray;
typedef TArray<uint32> FTendrTetrahedronVertexIndexArray;
typedef TArray<FVector2D> FTendrTexCoordArray;
typedef TArray<FTendrTangent> FTendrTangentArray;
typedef TArray<bool> FTendrVertexSurfaceIndicatorArray;

typedef uint32 FTendrModelGeneratorVersion;

//
// FTendrModelData
//
// Structure that contains all necessary Tendr model data
//
struct FTendrModelData
{
public:
	FTendrModelData()
		: Valid( false )
	{
	}

public:
	/** Vertices for physics use (sparse) **/
	FTendrVertexArray VerticesPhysics;

	/** Vertices for UE4 use (coarse) corresponding to the entire (internal) model **/
	FTendrVertexArray Vertices;

	/** Indices for UE4 use (coarse) corresponding to the entire (internal) model **/
	FTendrIndexArray Indices;
	
	/** Texcoords for UE4 use (coarse) corresponding to the entire (internal) model **/
	FTendrTexCoordArray TexCoords[ MAX_TEXCOORDS ];

	/** Tangents for UE4 use (coarse) corresponding to the entire (internal) model **/
	FTendrTangentArray Tangents;

	/** Boolean surface indicators for UE4 use (coarse) corresponding to the entire (internal) model **/
	FTendrVertexSurfaceIndicatorArray VerticesSurfaceIndicators;

	/** Tetrahedron face indices (4 face indices per tetrahedron) for UE4 use (coarse) corresponding to the entire (internal) model **/
	FTendrTetrahedronFaceIndexArray TetrahedronFaceIndices;

	/** Tetrahedron vertex indices (4 vertex indices per tetrahedron) for UE4 use (coarse) corresponding to the entire (internal) model **/
	FTendrTetrahedronVertexIndexArray TetrahedronVertexIndices;

	/** Vertex connectivity data **/
	FTendrConnectivityArray Connectivity;

	/** Linear mapping from coarse to sparse vertices **/
	FTendrCoarseSparseMapping MappingCoarseToSparse;

	/** Indicator for valid model data **/
	bool Valid;

public:
	bool IsValid() const
	{
		return Valid;
	};
};
