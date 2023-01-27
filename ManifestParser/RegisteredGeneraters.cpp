#include "RegisteredGenerators.h" 

using namespace Manifest_Parser;

std::map<std::string, RegisteredGenerator*> RegisteredGenerator::registeredGenerators{};