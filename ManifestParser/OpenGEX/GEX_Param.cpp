#include "GEX_Param.h"

using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType> GEX_Param::PropertyList::typeProperties
{
	{ "attrib", PropertyList::ATTRIB},
};


DDL_Structure* GEX_Param::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructureView, *result)))
		switch (PropertyList::typeProperties.find(static_cast<std::string>(property.key))->second)
		{
			case PropertyList::ATTRIB:
				attrib = property.value;
				break;
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);	

	return result;
}