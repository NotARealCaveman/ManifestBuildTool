#pragma once
#include <ManifestGLUtility/DebugLogger.h>

#include "Manifest_Database.h"

using namespace Manifest_Parser;

namespace Manifest_Persistence
{	
	void BuildOfflineDatabase(const DDL_File& file, ManifestDatabaseBuild& database);
}