#include "GEX_Color.h"
using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType> GEX_Color::PropertyList::typeProperties
{
	{ "attrib", PropertyList::ATTRIB},	
};

DDL_Structure GEX_Color::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
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
	result.subSutructres.emplace_back(channel.Build(PartitionDDLSubStructures(partitionedStructure)[0], referenceMap));
	result.typeHeap = static_cast<void*>(this);
	MapStructureName(result, referenceMap);
	colorType = channel.data.subBufferElements == 4 ? GEX_ColorType::RGBA : GEX_ColorType::RGB;

	return result;
}