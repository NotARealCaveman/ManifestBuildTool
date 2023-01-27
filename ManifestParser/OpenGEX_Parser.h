#pragma once

#include "OpenDDL_Parser.h"
#include "OpenGEX/GEX_Metric.h"
#include "OpenGEX/GEX_GeometryObject.h"

//OpenGEX is a ddl derivative file format that describes the data transimssion for common game engine assets and programs.

namespace Manifest_Parser
{	
	void InitializeExtendedTypes();
	void Initialize_GEXTypes();
	void Initialize_GEXGenerators();		
}
