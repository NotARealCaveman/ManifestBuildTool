#pragma once
#include "Database_Flags.h"


namespace Manifest_Persistence
{
	typedef uint64_t PrimaryKey;//main identifier of structure table
	typedef uint64_t ForeignKey;//main idenfitier of relational structure table
	typedef uint64_t UniqueKey;//extra table value
	typedef uint64_t CompositeKey;//composes bool row in leading bit, interger row in following bits
}