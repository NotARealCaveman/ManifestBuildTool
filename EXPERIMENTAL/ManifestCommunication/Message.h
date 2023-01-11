#pragma once

struct Manifest_Communication
{
	struct Message
	{
		struct MessageBase
		{};
		template<typename Payload>
		struct PayloadWrapper
		{
			Payload* payload;
		};
		template<typename Payload>
		Message();


	};
};