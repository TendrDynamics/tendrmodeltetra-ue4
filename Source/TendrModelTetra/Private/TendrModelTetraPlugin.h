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

//
// Global Tendr model generator version
//
const FTendrModelGeneratorVersion PluginModelGeneratorVersion = 0x00000001;

//
// Global logging declarations
//
DECLARE_LOG_CATEGORY_EXTERN(TendrModelTetraLog, All, All);

//
// Global logging macros
//
#define TENDR_ERROR(Message, ...) UE_LOG(TendrModelTetraLog, Error, TEXT("[%s] ") TEXT(Message), TEXT(__FUNCTION__), ##__VA_ARGS__)
