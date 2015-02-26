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

DEFINE_LOG_CATEGORY(TendrModelTetraLog);

class FTendrModelTetraPlugin : public IModuleInterface
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	class UTendrSoftManager* TendrSoftManager;
};

IMPLEMENT_MODULE(FTendrModelTetraPlugin, TendrModelTetra)

void FTendrModelTetraPlugin::StartupModule()
{
}

void FTendrModelTetraPlugin::ShutdownModule()
{
}
