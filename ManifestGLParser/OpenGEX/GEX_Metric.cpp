#include "GEX_Metric.h"

using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType> GEX_Metric::PropertyList::typeProperties
{
	{ "key", PropertyList::KEY},
};

DDL_Structure* GEX_Metric::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{	
	return nullptr;
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);

	DISABLE
	{
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, *result)))
		switch (PropertyList::typeProperties.find(property.key)->second)
		{
			case PropertyList::KEY:
				key = property.value;
				break;
			DEFAULT_BREAK;
		}

	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructure))
		switch (metricType = ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_float:
			{
				metric.emplace<DDL_Float>();
				result->subSutructres.emplace_back(std::get<DDL_Float>(metric).Build(subStructure, referenceMap));
				break;
			}
			case DDL_BufferTypes::DDL_string:
			{
				metric.emplace<DDL_String>();
				result->subSutructres.emplace_back(std::get<DDL_String>(metric).Build(subStructure, referenceMap));
				break;
			}
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);
	}

	return result;
}