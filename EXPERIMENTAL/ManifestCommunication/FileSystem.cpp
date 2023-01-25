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
		
		event.messages.emplace_back(Message {UnderlyingType(MessageTypes::MBD_MATERIAL), int{}});
	}	
	for (auto texture{ 0 }; texture < binaryDatabase.binaryTextureTable.header.totalEntries; ++texture)
	{
		event.eventToken |= UnderlyingType(MessageTypes::MBD_TEXTURE);
		event.messages.emplace_back(Message {UnderlyingType(MessageTypes::MBD_TEXTURE), int{}});
	}	
	ScratchPad<Byte>{}.Unwind();
	eventSpace.NotifyRegisteredObservers(std::move(event));
}

ProcessMessage Manifest_Communication::TEST_PROCESS_FUNC(std::vector<Message>& messages)
{		
	std::vector<Message> commits;	
	for (auto& message : messages)
	{
		switch (message.messageToken)
		{
			case UnderlyingType(FileSystemMessageType::MBD_MATERIAL):	
				break;			
			case UnderlyingType(FileSystemMessageType::MBD_TEXTURE):			break;
		}
	}
	ProcessMessage result = new Message{ 0,commits };

	return result;
}