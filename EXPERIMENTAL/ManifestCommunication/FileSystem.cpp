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
		material.materialIDs[2] = bImportMaterial.header.diffuseID;
		event.messages.emplace_back(Message {UnderlyingType(MessageTypes::MBD_MATERIAL),material });
	}	
	for (auto texture{ 0 }; texture < binaryDatabase.binaryTextureTable.header.totalEntries; ++texture)
	{
		event.eventToken |= UnderlyingType(MessageTypes::MBD_TEXTURE);
		auto& bImportTexture= binaryDatabase.binaryTextureTable.entries[texture];		
		auto textureDataCopy = bImportTexture;
		constexpr auto iFloatSize = 1.0f / sizeof(MFfloat);
		//copy texture and payload for deffered processing - this is done so the scratchpad may be attempted to be unwound now
		textureDataCopy.payload = new MFfloat[bImportTexture.header.payloadSize * iFloatSize];
		memcpy(textureDataCopy.payload, bImportTexture.payload, textureDataCopy.header.payloadSize);		
		event.messages.emplace_back(Message {UnderlyingType(MessageTypes::MBD_TEXTURE), textureDataCopy});
	}
	ScratchPad<Byte>{}.Unwind();//attempt to unwind
	eventSpace.NotifyRegisteredObservers(std::move(event));
}

Message* Manifest_Communication::TEST_PROCESS_FUNC(std::vector<Message>& messages, void* addy)
{	

	for (auto& message : messages)
	{
		switch (message.messageToken)
		{
			case UnderlyingType(FileSystemMessageType::MBD_MATERIAL):
				break;
			case UnderlyingType(FileSystemMessageType::MBD_TEXTURE):
				break;
		}
	}

	return nullptr;
}