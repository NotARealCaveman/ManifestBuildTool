#include "FileSystem.h"

using namespace Manifest_Communication;

void ObersvableFileSystem::LoadMBD(const std::string& mbd, EventSpace& eventSpace)
{
	std::ifstream bImport{ mbd, std::ios::in | std::ios::binary };
	//import database to scratch pad
	auto binaryDatabase = ImportBinaryDatabase(bImport);
	FileSystemEvent event;
	for(auto materialIndex{0}; materialIndex < binaryDatabase.binaryMaterialTable.header.totalEntries; ++materialIndex)
	{
		event.eventToken |= UnderlyingType(MessageTypes::MBD_MATERIAL);		
		event.messages.emplace_back(Message {UnderlyingType(MessageTypes::MBD_MATERIAL), Material{}});
	}	
	for (auto texture{ 0 }; texture < binaryDatabase.binaryTextureTable.header.totalEntries; ++texture)
	{
		event.eventToken |= UnderlyingType(MessageTypes::MBD_TEXTURE);
		event.messages.emplace_back(Message {UnderlyingType(MessageTypes::MBD_TEXTURE), Texture{}});
	}	
	for (auto objectIndex{ 0 }; objectIndex < binaryDatabase.binaryGeometryObjectTable.header.totalEntries; ++objectIndex)
	{			
		event.eventToken |= UnderlyingType(MessageTypes::MBD_GEOMETRYOBJECT);
		event.messages.emplace_back(Message {UnderlyingType(MessageTypes::MBD_GEOMETRYOBJECT), Texture{}});
	}
	ScratchPad<Byte>{}.Unwind();
	eventSpace.NotifyRegisteredObservers(std::move(event));
}