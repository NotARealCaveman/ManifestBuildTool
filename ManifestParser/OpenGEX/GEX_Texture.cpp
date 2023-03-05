#include "GEX_Texture.h"

using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType> GEX_Spectrum::PropertyList::typeProperties
{
	{ "attrib", PropertyList::ATTRIB},
	{ "min", PropertyList::MIN},
	{ "max", PropertyList::MAX}
};

DDL_Structure* GEX_Spectrum::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructureView, *result)))
		switch (PropertyList::typeProperties.find(static_cast<std::string>(property.key))->second)
		{
			case PropertyList::ATTRIB:
				break;
			case PropertyList::MIN:
				break;
			case PropertyList::MAX:
				break;
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);	

	return result;
}


const std::map<std::string, DDL_BufferType> GEX_Texture::PropertyList::typeProperties
{
	{ "attrib", PropertyList::ATTRIB},
	{"texcoord",TEXCOORD},
	{"swizzle",SWIZZLE},
	{"xaddress",XADDRESS},
	{"yaddress",YADDRESS},
	{"zaddress",ZADDRESS},
	{"border",BORDER}
};

DDL_Structure* GEX_Texture::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructureView, *result)))
		switch (PropertyList::typeProperties.find(static_cast<std::string>(property.key))->second)
		{
			case PropertyList::ATTRIB:
				attrib = property.value;
				break;
			case PropertyList::TEXCOORD:
				texcoord = std::stoi(static_cast<std::string>(property.value));
				break;
			case PropertyList::SWIZZLE:
				swizzle = property.value;
				break;
			case PropertyList::XADDRESS:
				x_address = property.value;
				break;
			case PropertyList::YADDRESS:
				y_address = property.value;
				break;
			case PropertyList::ZADDRESS:
				z_address = property.value;
				break;
			case PropertyList::BORDER:
				border = property.value;
				break;
			DEFAULT_BREAK
		}
	//parse substructures...
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}
