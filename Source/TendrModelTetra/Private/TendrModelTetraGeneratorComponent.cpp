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

#include "TendrModelTetraPluginPrivatePCH.h"

///////////////////////////////////////////////////////////////////////////////
//
// Configuration
//

//
// Static log buffer size
//
#define LOG_BUFFER_MAX 2048

///////////////////////////////////////////////////////////////////////////////
//
// Tetgen dependency, compiled inline for ease of use
//
#pragma warning( disable: 4701 )
#include "tetgen/predicates.cxx"
#include "tetgen/tetgen.cxx"

//
// Enables verbose output logging (decreases performance)
//
//#define TETRA_DEBUG

//
// Enables verbose tetgen output logging (decreases performance)
//#define TETGEN_DEBUG

//
// Tetgen print callback
//
#include <string>
#include <sstream>
#include <iostream>
#include <ostream>
#include <vector>

void myprintf( const char* format, ... )
{
#ifdef TETGEN_DEBUG
	va_list args;
	va_start( args, format );
	{
		TCHAR pBufferWide[ LOG_BUFFER_MAX ] = { 0 };
		char pBuffer[ LOG_BUFFER_MAX ] = { 0 };

		_vsnprintf( pBuffer, LOG_BUFFER_MAX - 1, format, args );

		MultiByteToWideChar( CP_ACP, 0, pBuffer, -1, pBufferWide, LOG_BUFFER_MAX - 1 );
		pBufferWide[ LOG_BUFFER_MAX - 1 ] = 0;

		UE_LOG( TendrModelTetraLog, Log, pBufferWide );
	}
	va_end( args );
#endif
}
///////////////////////////////////////////////////////////////////////////////

UTendrModelTetraGeneratorComponent::UTendrModelTetraGeneratorComponent( const FObjectInitializer &ObjectInitializer )
: USceneComponent( ObjectInitializer )
{
	TendrModelGeneratorVersion = PluginModelGeneratorVersion;
	MaximumTetraVolume = 7500;
}

void UTendrModelTetraGeneratorComponent::OnRegister()
{
	Super::OnRegister();
}

#if WITH_EDITOR
void UTendrModelTetraGeneratorComponent::PreEditChange( UProperty* PropertyAboutToChange )
{
	Super::PreEditChange( PropertyAboutToChange );

	// Release generated data here
}

void UTendrModelTetraGeneratorComponent::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent )
{
	Super::PostEditChangeProperty( PropertyChangedEvent );
}
#endif

void UTendrModelTetraGeneratorComponent::BeginDestroy()
{
	Super::BeginDestroy();
	
	// Release generated data here
}

void UTendrModelTetraGeneratorComponent::GetAssetRegistryTags( TArray<FAssetRegistryTag>& OutTags ) const
{
	Super::GetAssetRegistryTags( OutTags );
}

FString UTendrModelTetraGeneratorComponent::GetDesc()
{
	return FString::Printf(
		TEXT( "Tendr Tetrahedral model generator" )
		);
}

FTendrModelData UTendrModelTetraGeneratorComponent::Build( const FTendrVertexArray& InputVertices, const FTendrIndexArray& InputIndices, bool bSilent )
{
	FTendrModelData OutputModelData;

	if(InputVertices.Num() > 0 && InputIndices.Num() > 0)
	{
		if(!bSilent)
		{
			GWarn->BeginSlowTask( FText::FromString( TEXT( "Building Tendr model" ) ), true );
		}

		// Generate model
		{
			// Debug statistics
			const uint32 NumSurfaceVerts = InputVertices.Num();

			// 1. Tetrahedralization
			bool ValidModel = false;
			tetgenio out;	// deinitialize (deallocator) is automatically called when these go out of scope
			{
				// Tetrahedral model generation
				{
					tetgenio in;	// deinitialize (deallocator) is automatically called when these go out of scope

					in.firstnumber = 0;
					in.numberofpoints = NumSurfaceVerts;

					in.pointlist = new REAL[ in.numberofpoints * 3 ];
					in.pointparamlist = new tetgenio::pointparam[ in.numberofpoints ];
					for(int i = 0; i < in.numberofpoints; ++i)
					{
						// Store vertices
						in.pointlist[ i * 3 + 0 ] = InputVertices[ i ].X;
						in.pointlist[ i * 3 + 1 ] = InputVertices[ i ].Y;
						in.pointlist[ i * 3 + 2 ] = InputVertices[ i ].Z;

						// Store UVs (currently not used)
						tetgenio::pointparam param;
						param.tag = 0;
						param.type = 0;
						for(uint32 t = 0; t < MAX_TEXCOORDS; ++t)
						{
							param.uv[ t * 2 + 0 ] = 1;
							param.uv[ t * 2 + 1 ] = 1;
						}

						// Store other per-vertex data (currently not used)
						param.uv[ 8 + 0 ] = 0;
						param.uv[ 8 + 1 ] = 0;
						param.uv[ 8 + 2 ] = 0;
						param.uv[ 8 + 3 ] = 0;
						param.uv[ 8 + 4 ] = 0;
						param.uv[ 8 + 5 ] = 0;

						in.pointparamlist[ i ] = param;

#ifdef TETRA_DEBUG
						UE_LOG( TendrModelTetraLog, Log, TEXT( "Vertex %u: %.3f, %.3f, %.3f" ), i, InputVertices[ i ].X, InputVertices[ i ].Y, InputVertices[ i ].Z );
#endif
					}

					in.numberoffacets = InputIndices.Num() / 3;
					in.facetlist = new tetgenio::facet[ in.numberoffacets ];
					in.facetmarkerlist = new int[ in.numberoffacets ];
					for(int i = 0; i < in.numberoffacets; ++i)
					{
						in.facetmarkerlist[ i ] = 1;

						tetgenio::facet& f = in.facetlist[ i ];
						f.holelist = NULL;
						f.numberofholes = 0;
						f.numberofpolygons = 1;
						f.polygonlist = new tetgenio::polygon[ f.numberofpolygons ];

						tetgenio::polygon& p = f.polygonlist[ 0 ];
						p.numberofvertices = 3;
						p.vertexlist = new int[ p.numberofvertices ];
						p.vertexlist[ 0 ] = InputIndices[ i * 3 + 0 ];
						p.vertexlist[ 1 ] = InputIndices[ i * 3 + 1 ];
						p.vertexlist[ 2 ] = InputIndices[ i * 3 + 2 ];
					}

					in.facetmarkerlist = NULL;

					UE_LOG( TendrModelTetraLog, Log, TEXT( "Tetrahedralizing mesh (input vertices: %u, triangles: %u)" ), in.numberofpoints, in.numberoffacets );

					tetgenbehavior b;
					{
						b.zeroindex = 1;
						b.docheck = 1;
						b.verbose = 1;
						b.diagnose = 0;
						b.facesout = 1;
						b.edgesout = 1;
						b.neighout = 2;
						b.object = tetgenbehavior::POLY;
						b.refine = 0;
						b.plc = 1;
						b.quality = 1;
						b.steinerleft = 4;

						//b.weighted = 1;

						//b.no_sort = 1;
						b.optlevel = 9;
						b.optscheme = 7;

						// Disable removal of duplicate vertices and faces
						b.nomergefacet = 1;
						b.nomergevertex = 1;
						b.nojettison = 1;

						// Prevent insertion of vertices on boundary, only interior
						b.nobisect = 1;
						b.nobisect_nomerge = 1;
						b.supsteiner_level = 4;
						b.addsteiner_algo = 1;

						// First order tetrahedra (with 4 vertices) are necessary for all operations below that deal with tetrahedra
						b.order = 1;

						// UV support
						b.psc = 1;

						// Constrain maximum volume of tetras
						//b.metric = 1;
						b.minratio = 1.5;
						b.mindihedral = 10.0;
						if(MaximumTetraVolume > 0)
						{
							//b.varvolume = 0;
							b.fixedvolume = 1;
							b.maxvolume = MaximumTetraVolume;
							
						}
					}

					try
					{
						// Invoke tetgen
						{
							tetrahedralize( &b, &in, &out );

							UE_LOG( TendrModelTetraLog, Log, TEXT( "Output mesh done, points: %u, tetras: %u, triangles: %u, edges: %u" ),
									out.numberofpoints,
									out.numberoftetrahedra,
									out.numberoftrifaces,
									out.numberofedges
									);

							if( out.numberofpoints == 0 )
							{
								throw 6;
							}
							if( out.numberoftetrahedra == 0 )
							{
								throw 7;
							}
							if(	out.numberoftrifaces == 0 )
							{
								throw 8;
							}
							if( out.numberofedges == 0 )
							{
								throw 9;
							}
						}

						// Conversion to compatible structures
						{
							// Iterate over generated triangles of model
							for(int i = 0; i < out.numberoftrifaces; ++i)
							{
								int A = out.trifacelist[ i * 3 + 0 ];
								int B = out.trifacelist[ i * 3 + 1 ];
								int C = out.trifacelist[ i * 3 + 2 ];

								// Add indices to output data
								OutputModelData.Indices.Add( A );
								OutputModelData.Indices.Add( B );
								OutputModelData.Indices.Add( C );

	#ifdef TETRA_DEBUG
								UE_LOG( TendrModelTetraLog, Log, TEXT( "Index %u: %u %u %u" ), i, A, B, C );
	#endif
							}

							//
							// Face uniqueness
							//
							// This map is used to identify faces that are used multiple times, by counting their references.
							// Since we only allow unique faces for tetrahedra, we use this map to identify and duplicate any non-unique faces.
							//
							// This array is initialized with N elements set to 0, where N is the current amount of faces in the tetgen output.
							//
							TArray< uint8 > TempFaceUniqueness;
							TempFaceUniqueness.Init( 0, out.numberoftrifaces );
							uint32 NonUniqueFaces = 0;

							// Iterate over generated tetrahedra of model
							{
								// Lambda function to make faces unique
								auto FnFaceMakeUnique = [ &TempFaceUniqueness, &NonUniqueFaces, &OutputModelData ]( int FaceIndex )
								{
									if(TempFaceUniqueness[ FaceIndex ] > 0)
									{
										// Face is not unique, so duplicate by reinserting at the end
										int A = OutputModelData.Indices[ FaceIndex * 3 + 0 ];
										int B = OutputModelData.Indices[ FaceIndex * 3 + 1 ];
										int C = OutputModelData.Indices[ FaceIndex * 3 + 2 ];

										int NewIndex = OutputModelData.Indices.Add( A );
										OutputModelData.Indices.Add( B );
										OutputModelData.Indices.Add( C );
										TempFaceUniqueness.Add( 0 );
										++NonUniqueFaces;

										// Update face index
										FaceIndex = NewIndex / 3;
									}

									// Increment uniqueness counter
									TempFaceUniqueness[ FaceIndex ] = TempFaceUniqueness[ FaceIndex ] + 1;

									return FaceIndex;
								};

								for(int i = 0; i < out.numberoftetrahedra; ++i)
								{
									// We assume first-order tetrahedra with 4 vertices

									// Store tetrahedron vertices
									OutputModelData.TetrahedronVertexIndices.Add( out.tetrahedronlist[ i * 4 + 0 ] );
									OutputModelData.TetrahedronVertexIndices.Add( out.tetrahedronlist[ i * 4 + 1 ] );
									OutputModelData.TetrahedronVertexIndices.Add( out.tetrahedronlist[ i * 4 + 2 ] );
									OutputModelData.TetrahedronVertexIndices.Add( out.tetrahedronlist[ i * 4 + 3 ] );

									// For each face, ensure the uniqueness
									int A = FnFaceMakeUnique( out.tet2facelist[ i * 4 + 0 ] );
									int B = FnFaceMakeUnique( out.tet2facelist[ i * 4 + 1 ] );
									int C = FnFaceMakeUnique( out.tet2facelist[ i * 4 + 2 ] );
									int D = FnFaceMakeUnique( out.tet2facelist[ i * 4 + 3 ] );

									// Store tetrahedron to face mapping
									OutputModelData.TetrahedronFaceIndices.Add( A );
									OutputModelData.TetrahedronFaceIndices.Add( B );
									OutputModelData.TetrahedronFaceIndices.Add( C );
									OutputModelData.TetrahedronFaceIndices.Add( D );
								}
								UE_LOG( TendrModelTetraLog, Log, TEXT( "Face uniqueness pass: %u non unique faces have been found and duplicated" ), NonUniqueFaces );
							}

							//
							// Coarse-to-sparse mapping
							//
							// Coarse: duplicated vertices, for use in UE4 graphics rendering
							// Sparse: non-duplicated vertices, for use in physics engine
							//
							// This distinction is necessary because UE4 graphics rendering requires the use of duplicated vertices
							// for vertex data blending, while our physics engine needs to be as sparse as possible for performance reasons.
							//
							TMap<FVector, uint32> TempMapCoarseToSparse;

							// Iterate over generated points of model
							for(int i = 0; i < out.numberofpoints; ++i)
							{
								// Retrieve vertex
								FVector Vertex( out.pointlist[ i * 3 + 0 ], out.pointlist[ i * 3 + 1 ], out.pointlist[ i * 3 + 2 ] );

								// Store vertex
								OutputModelData.Vertices.Add( FVector4( Vertex, 0 ) );

								// Coarse to sparse mapping algorithm
								{
									uint32 IndexSparse = OutputModelData.VerticesPhysics.Num();

									// Check if duplicate vertex exists and insert if it doesn't
									uint32 * pKey = TempMapCoarseToSparse.Find( Vertex );
									if(pKey == NULL)
									{
										// No duplicate vertex exists

										// Add to map
										TempMapCoarseToSparse.Add( Vertex, IndexSparse );

										// Initialize dummy connectivity to InvalidIndex
										FTendrVertexConnectivityData Dummy;
										memset( Dummy.NeighbourIndices, 0xFF, sizeof( Dummy.NeighbourIndices ) );
										OutputModelData.Connectivity.Add( Dummy );

#ifdef TETRA_DEBUG
										UE_LOG( TendrModelTetraLog, Log, TEXT( "Vertex %u (%.3f, %.3f, %.3f) ++ physics vertex %u" ),
												i,
												Vertex.X,
												Vertex.Y,
												Vertex.Z,
												IndexSparse
												);
#endif
										// Add vertex to sparse data structure
										OutputModelData.VerticesPhysics.Add( FVector4( Vertex, 0 ) );

									}
									else
									{
#ifdef TETRA_DEBUG
										UE_LOG( TendrModelTetraLog, Log, TEXT( "Vertex %u (%.3f, %.3f, %.3f) == physics vertex %u" ),
												i,
												Vertex.X,
												Vertex.Y,
												Vertex.Z,
												*pKey
												);
#endif
									}

									// In any case, add mapping
									OutputModelData.MappingCoarseToSparse.Add( ( pKey == NULL ) ? IndexSparse : (*pKey) );
								}
							}

							// Output some statistics
							UE_LOG( TendrModelTetraLog, Log, TEXT( "Input  = Indices [%u], Vertices [%u]" ), InputIndices.Num(), NumSurfaceVerts );
							UE_LOG( TendrModelTetraLog, Log, TEXT( "Output = Indices [%u], Triangles [%u], Tetrahedra [%u], Vertices [%u surface, %u internal, %u physics]" ),
									OutputModelData.Indices.Num(),
									OutputModelData.Indices.Num() / 3,
									OutputModelData.TetrahedronFaceIndices.Num() / 4,
									NumSurfaceVerts,
									OutputModelData.Vertices.Num(),
									OutputModelData.VerticesPhysics.Num()
									);

							// Add connectivity based on edges
							uint32 MaxNeighbours = 0;
							for(int i = 0; i < out.numberofedges; ++i)
							{
								int a, b;
								{
									// Retrieve edge indices (coarse)
									int coarseA = out.edgelist[ i * 2 + 0 ];
									int coarseB = out.edgelist[ i * 2 + 1 ];

									// Get corresponding sparse index
									a = OutputModelData.MappingCoarseToSparse[ coarseA ];
									b = OutputModelData.MappingCoarseToSparse[ coarseB ];

									// Check for consistency
									check( a != InvalidIndex );
									check( b != InvalidIndex );

									// Add to sparse connectivity data structure
									ConnectivityAddNeighbour( OutputModelData, a, b, MaxNeighbours );
									ConnectivityAddNeighbour( OutputModelData, b, a, MaxNeighbours );
								}
							}

							UE_LOG( TendrModelTetraLog, Log, TEXT( "Maximum neighbours in model: %u" ), MaxNeighbours );

							// Mark as valid
							OutputModelData.Valid = true;
						}
					}
					catch(int error)
					{
						switch(error)
						{
						case 1:
							SetError( TEXT( "Not enough memory available or model excessively large" ) );
							break;
						case 3:
							SetError( TEXT( "Input contains self-intersecting triangles" ) );
							break;
						case 4:
						case 5:
							SetError( TEXT( "Input contains triangles that are too small" ) );
							break;
						case 6:
							SetError( TEXT( "Model generator output did not output any points" ) );
							break;
						case 7:
							SetError( TEXT( "Model generator output did not output any tetrahedra" ) );
							break;
						case 8:
							SetError( TEXT( "Model generator output did not output any triangles" ) );
							break;
						case 9:
							SetError( TEXT( "Model generator output did not output any edges" ) );
							break;
						case 9000:
							SetError( TEXT( "Second-order tetrahedrons are not supported" ) );
							break;
						default:
							SetError( FString::Printf( TEXT( "Input could not be processed (tetgen error %u)" ), error ) );
							break;
						}
					}
				}
			}
		}

		if(!bSilent)
		{
			GWarn->EndSlowTask();
		}
	}

	return OutputModelData;
}

void UTendrModelTetraGeneratorComponent::SetError( FString Text )
{
	// Write to log
	UE_LOG( TendrModelTetraLog, Error, TEXT( "%s" ), *Text );

	// Set as last error
	LastError = Text;
}

FString UTendrModelTetraGeneratorComponent::GetLastError()
{
	return LastError;
}

bool UTendrModelTetraGeneratorComponent::ConnectivityAddNeighbour( FTendrModelData& ModelData, uint32 Src, uint32 Dst, uint32& MaxNeighbours )
{
	uint32 * SrcIndices = ModelData.Connectivity[ Src ].NeighbourIndices;

	// Ensure index is not already added
	for(uint8 i = 0; i < FTendrVertexConnectivityData::MaxNeighbours; ++i)
	{
		if(SrcIndices[ i ] == Dst)
		{
			// Already added, return
			return true;
		}
		else if(SrcIndices[ i ] == InvalidIndex)
		{
			// Reached the end, insert here
			MaxNeighbours = MaxNeighbours > i ? MaxNeighbours : i;
			SrcIndices[ i ] = Dst;
			return true;
		}
	}

	// Could not add, neighbour array full
	UE_LOG( TendrModelTetraLog, Log, TEXT( "FTendrVertexConnectivity: Neighbour overflow" ) );
	return false;
}
