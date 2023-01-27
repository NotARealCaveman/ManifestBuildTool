#pragma once

#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

#include "Runtime_Table.h"

using namespace Manifest_Experimental;

namespace Manifest_Persistence
{
	using MaterialTable = Table< Material, std::default_delete<Material>>;
	using TextureTable = Table<Texture, std::default_delete<Texture>>;
}