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
		auto& bImportMaterial = binaryDatabase.binaryMaterialTable.entries[materialIndex];
		Material material;
		material.materialIDs[0] = bImportMaterial.header.diffuseID;
		material.materialIDs[1] = bImportMaterial.header.normalID;
		material.materialIDs[2] = bImportMaterial.header.parallaxID;
		event.messages.emplace_back(Message {UnderlyingType(MessageTypes::MBD_MATERIAL),material });
	}	
	for (auto texture{ 0 }; texture < binaryDatabase.binaryTextureTable.header.totalEntries; ++texture)
	{
		event.eventToken |= UnderlyingType(MessageTypes::MBD_TEXTURE);
		auto& bImportTexture= binaryDatabase.binaryTextureTable.entries[texture];	
		auto textureCopy = bImportTexture;
		textureCopy.payload = new float[3];
		memcpy(textureCopy.payload, bImportTexture.payload, sizeof(float) * 3);
		event.messages.emplace_back(Message {UnderlyingType(MessageTypes::MBD_TEXTURE), textureCopy });
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
				//create material table entry
				auto material = message.GetMessageContentWrapper<Material>();
				break;
			case UnderlyingType(FileSystemMessageType::MBD_TEXTURE):				
				auto binaryTexture = message.GetMessageContentWrapper<Binary_Texture>();
				//create opengl id
				//release copy memory
				delete binaryTexture->content.payload;
				//relase message
				delete binaryTexture;
				//create texture table entry
				break;
		}
	}
	ProcessMessage result = new Message{ 0,commits };

	return result;
}