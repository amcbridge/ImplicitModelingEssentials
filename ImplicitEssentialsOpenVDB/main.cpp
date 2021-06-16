#if (_MSC_VER >= 1915)
#define no_init_all deprecated
#endif

#include <openvdb/openvdb.h>

#include "ParametersReader.h"
#include "Operations.h"

const std::string PARAMETERS_FILE_NAME("parameters.json");

int main()
{
	try
	{
		ParametersReader parameters(PARAMETERS_FILE_NAME);
		openvdb::initialize();

		auto generateParams = parameters.GetGenerateParameters();
		auto booleanParams = parameters.GetBooleanParameters();
		auto offsetParams = parameters.GetOffsetParameters();
	
		if (!generateParams.AreEmpty()) Operations::GenerateLattice(generateParams);
		if (!booleanParams.AreEmpty()) Operations::Boolean(booleanParams);
		if (!offsetParams.AreEmpty()) Operations::OffsetFill(offsetParams);

		std::cout << "Finished" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cin.get();
}
