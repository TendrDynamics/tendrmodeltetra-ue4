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

namespace UnrealBuildTool.Rules
{
	public class TendrModelTetra : ModuleRules
	{
        public TendrModelTetra(TargetInfo Target)
        {
            PrivateIncludePaths.Add("TendrModelTetra/Private");

            MinFilesUsingPrecompiledHeaderOverride = 1;
            bFasterWithoutUnity = true;

            // DirectX11/RHI dependencies
            {
                PrivateIncludePaths.Add("../../../../Source/Runtime/Windows/D3D11RHI/Private");
                PrivateIncludePaths.Add("../../../../Source/Runtime/Windows/D3D11RHI/Private/Windows");
            }

            PublicDependencyModuleNames.AddRange(
                new string[]
				{
					"Core",
					"CoreUObject",
                    "Engine",
                    "RenderCore",
                    "ShaderCore"
				}
                );

            // For editor-specific functionality (e.g. world settings)
            PrivateDependencyModuleNames.AddRange(
            new string[] {
				"PropertyEditor",
				"AssetTools",
				"WorkspaceMenuStructure",
				"EditorStyle",
				"UnrealEd",
                "LevelEditor",
                "Slate",

                // Slate and SlateCore are needed as of 4.2
                "SlateCore"
			}
            );
        }
	}
}
