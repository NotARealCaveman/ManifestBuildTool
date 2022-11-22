#include "DDL_Structure.h"

using namespace Manifest_Parser;

void Manifest_Parser::MapStructureName(DDL_Structure& structure, DDL_ReferenceMap& referenceMap)
{
	referenceMap.referenceMap.insert({ structure.name.empty() ? structure.name= "unamed" + std::to_string(referenceMap.unnamedStructureCount++) : structure.name,structure });
}