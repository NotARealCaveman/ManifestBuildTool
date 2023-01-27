#pragma once
#include <ManifestUtility/DebugLogger.h>

#include "Manifest_DatabaseBuilder.h"

using namespace Manifest_Parser;

namespace Manifest_Persistence
{	
	void BuildOfflineDatabase(const DDL_File& file, ManifestDatabaseBuilder& database);
}