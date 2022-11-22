#pragma once

#include <ManifestGLUtility/DebugLogger.h>

#include "Manifest_Database.h"

using namespace Manifest_Parser;

namespace Manifset_Persistence
{	
	void BuildDatabase(const DDL_File& file, ManifestDatabase& database);
}