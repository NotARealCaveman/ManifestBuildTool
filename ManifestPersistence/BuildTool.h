#pragma once
#include <ManifestUtility/DebugLogger.h>

#include "Manifest_DatabaseBuilder.h"

using namespace Manifest_Parser;

namespace Manifest_Persistence
{	
	void BuildResourceDatabase(const DDL_File& file, ManifestResourceDatabaseBuilder& database);

	void BuildTerrainDatabase(const DDL_File& file, ManifestTerrainDatabaseBuilder& database);
}