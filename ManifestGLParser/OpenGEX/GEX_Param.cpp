#include "GEX_Param.h"

using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType> GEX_Param::PropertyList::typeProperties
{
	{ "attrib", PropertyList::ATTRIB},
};


DDL_Structure GEX_Param::Build(const ScratchPadString& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	DDL_Structure result;
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, result)))	
		switch (PropertyList::typeProperties.find(property.key)->second)
		{
			case PropertyList::ATTRIB:
				attrib = property.value;
				break;
			DEFAULT_BREAK
		}
	result.typeHeap = static_cast<void*>(this);
	MapStructureName(result, referenceMap);	

	return result;
}