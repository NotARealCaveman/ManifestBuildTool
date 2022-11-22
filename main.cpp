#pragma once

#include <ManifestGLParser/OpenGEX_Parser.h>
#include <ManifestGLPersistence/BuildTool.h>
#include <ManifestGLUtility/Console_Color.h>

using namespace Manifest_Parser;

int main()
{
	WINDOWS_COLOR_CONSOLE
	//ddl start up
	Initialize_GEXTypes();
	Initialize_GEXGenerators();
	//parse

	return 0;
}