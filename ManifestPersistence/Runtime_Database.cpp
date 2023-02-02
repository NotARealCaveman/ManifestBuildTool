#include "Runtime_Database.h"

using namespace Manifest_Persistence;

ManifestRuntimeDatabase::ManifestRuntimeDatabase(NodeStateTable&& _nodeStateTable)
	:nodeStateTable{ std::move(_nodeStateTable) }
{

}