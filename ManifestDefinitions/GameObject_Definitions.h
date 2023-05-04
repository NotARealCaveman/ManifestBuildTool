#pragma once

#include <ManifestCommunication/MessageOperators.h>

using namespace Manifest_Communication;

namespace Manifest_Definitions
{
	enum class GAMEOBJECT_TYPES : int
	{
		PHYSICS = 1,
		GRAPHICS = 2
	};

	static constexpr GAMEOBJECT_TYPES RIGIDBODY_OBJECT = GAMEOBJECT_TYPES::PHYSICS | GAMEOBJECT_TYPES::GRAPHICS;
}