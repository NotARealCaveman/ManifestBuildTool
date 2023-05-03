#include "MDD_Rigidbody.h"

using namespace Manifest_Parser;
using namespace RIGID_BODY_PARAMS;

const std::map<std::string, MFu8> Manifest_Parser::rigidBodyParamList
{
	{"orientation",ORIENTATION},
	{"position",POSITION},
	{"linearAccelaration",ACCELARATION},
	{"linearVelocity",LINEAR_VELOCITY},
	{"angularVelocity",ANGULAR_VELOCITY},
	{"appliedForce",APPLIED_FORCE},
	{"appliedTorque",APPLIED_TORQUE},
	{"iMass",IMASS},
	{"linearDamping",LINEAR_DAMPING},
	{"angularDamping",ANGULAR_DAMPING},
};

DDL_Structure* MDD_RigidBodyParams::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);

	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructureView))
		switch (ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_float:
			{
				const auto name{ static_cast<std::string>(ExtractStructureName(subStructure)) };
				switch (rigidBodyParamList.find(name)->second)
				{
					case ORIENTATION:
						orientation.Build(subStructure, referenceMap);
						break;
					case POSITION:
						position.Build(subStructure, referenceMap);
						break;					
					case ACCELARATION:
						linearAccelaration.Build(subStructure, referenceMap);
						break;
					case LINEAR_VELOCITY:
						linearVelocity.Build(subStructure, referenceMap);
						break;
					case ANGULAR_VELOCITY:
						angularVelocity.Build(subStructure, referenceMap);
						break;
					case APPLIED_FORCE:
						appliedForce.Build(subStructure, referenceMap);
						break;
					case APPLIED_TORQUE:
						appliedTorque.Build(subStructure, referenceMap);
						break;
					case IMASS:
						iMass.Build(subStructure, referenceMap);
						break;
					case LINEAR_DAMPING:
						linearDamping.Build(subStructure, referenceMap);
						break;
					case ANGULAR_DAMPING:
						angularVelocity.Build(subStructure, referenceMap);
						break;
					DEFAULT_BREAK
				}
				break;
			}
		DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}