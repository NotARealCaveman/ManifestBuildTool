#include "GEX_Transform.h"

using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType> GEX_Transform::PropertyList::typeProperties
{
	{ "object", PropertyList::OBJECT},
	{ "kind", PropertyList::KIND},	
};


DDL_Structure* GEX_Transform::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{			
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructureView, *result)))
		switch (PropertyList::typeProperties.find(static_cast<std::string>(property.key))->second)
		{
			case PropertyList::OBJECT:
				std::stringstream{ static_cast<std::string>(property.value) } >> std::boolalpha >> object;
				break;
			case PropertyList::KIND:
				kind = property.value;
				break;
			DEFAULT_BREAK
		}	
	result->subSutructres.emplace_back(field.Build(PartitionDDLSubStructures(partitionedStructureView)[0], referenceMap));//should only have 1	
	FormatTransform(field.data.subBufferElements, reinterpret_cast<float**>(&field.data.typeHeap));
	field.data.subBufferElements = TransformSize;
	//tbd - multiple transforms
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);
	return result; 
}

//potentially swaps ownersip of unformattedData with newly allocated memory and cleans up old memory
void Manifest_Parser::FormatTransform(const size_t& transformType, float** unformattedData)
{
	if (transformType == TransformSize)
		return;
	//engine uses column major matrices X axis -> Y axis...	
	//luckily gex also uses column major layout in the spec for transform access
	float formattedData[TransformSize]
	{
		1,0,0,0,//0,1,2,3
		0,1,0,0,//4,5,6,7
		0,0,1,0,//8,9,10,11
		0,0,0,1 //12,13,14,15
	};
	//takes the identity and supplies the given transform data
	switch (transformType)
	{	
		case 12:
			memcpy(&formattedData[0], &(*unformattedData)[0], sizeof(float) * 3);
			memcpy(&formattedData[4], &(*unformattedData)[3], sizeof(float) * 3);
			memcpy(&formattedData[8], &(*unformattedData)[6], sizeof(float) * 3);
			memcpy(&formattedData[12],&(*unformattedData)[9], sizeof(float) * 3);
			break;
		case 9:
			memcpy(&formattedData[0], &(*unformattedData)[0], sizeof(float)*3);
			memcpy(&formattedData[4], &(*unformattedData)[3], sizeof(float)*3);
			memcpy(&formattedData[8], &(*unformattedData)[6], sizeof(float)*3);
			break;
		case 6:
			memcpy(&formattedData[0], &(*unformattedData)[0], sizeof(float)*2);
			memcpy(&formattedData[4], &(*unformattedData)[2], sizeof(float)*2);
			memcpy(&formattedData[8], &(*unformattedData)[4], sizeof(float)*2);
			break;
		case 4:
			memcpy(&formattedData[0], &(*unformattedData)[0], sizeof(float)*2);
			memcpy(&formattedData[4], &(*unformattedData)[2], sizeof(float)*2);
			break;
		DEFAULT_BREAK
	}	
	auto temp = *unformattedData;//temp takes reference of old memory
	//*unformattedData = new float[TransformSize];//pointer to old memory is given allocation for formatted data
	*unformattedData = New<float,ScratchPad<float>>(TransformSize);
	memcpy(*unformattedData, formattedData, sizeof(float) * TransformSize);//copy formatted data into previously unformatted data	
	//delete[] temp;//clean up old memory from caller - scratch pad
}