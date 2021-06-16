#include "ParametersReader.h"

template<typename T>
T Parameters::GetRequiredParameter(const nlohmann::json& node, const std::string& paramName)
{
	auto paramNode = node.find(paramName);
	if (paramNode == node.end())
		throw std::runtime_error("Missing json argument: " + paramName);
	return paramNode->get<T>();
}

template<typename T>
bool Parameters::GetOptionalParameter(const nlohmann::json& node, const std::string& paramName, T& value)
{
	auto paramNode = node.find(paramName);
	if (paramNode == node.end())
		return false;

	value = paramNode->get<T>();
	return true;
}

void Parameters::Read(const nlohmann::json& node)
{
	mOutputPath = GetRequiredParameter<std::string>(node, ArgNames::OutputPath);
	mEmpty = false;
}

void GenerateParameters::Read(const nlohmann::json& node)
{
	Parameters::Read(node);
	mLatticeType = GetRequiredParameter<std::string>(node, ArgNames::LatticeType);
	auto voxelSize = GetRequiredParameter<std::array<float, 3>>(node, ArgNames::VoxelSize);
	mVoxelSize.x() = voxelSize[0];
	mVoxelSize.y() = voxelSize[1];
	mVoxelSize.z() = voxelSize[2];

	auto gridSize = GetRequiredParameter<std::array<int, 3>>(node, ArgNames::GridDimensions);
	mGridDimensions.x() = gridSize[0];
	mGridDimensions.y() = gridSize[1];
	mGridDimensions.z() = gridSize[2];
}

void BooleanParameters::Read(const nlohmann::json& node)
{
	Parameters::Read(node);
	mInputPath = GetRequiredParameter<std::string>(node, ArgNames::InputPath);
	mSecondInputPath = GetRequiredParameter<std::string>(node, ArgNames::SecondInputPath);
	auto translation = GetRequiredParameter<std::array<float, 3>>(node, ArgNames::Translation);
	mTranslation.x() = translation[0];
	mTranslation.y() = translation[1];
	mTranslation.z() = translation[2];

	mOperation = GetRequiredParameter<std::string>(node, ArgNames::Operation);
}

void OffsetParameters::Read(const nlohmann::json& node)
{
	Parameters::Read(node);
	mInputPath = GetRequiredParameter<std::string>(node, ArgNames::InputPath);
	mLatticeType = GetRequiredParameter<std::string>(node, ArgNames::LatticeType);
	mOffset = GetRequiredParameter<float>(node, ArgNames::Offset);
}

ParametersReader::ParametersReader(const std::filesystem::path& jsonFilePath)
{
	std::ifstream inputFileStream(jsonFilePath);
	if (!inputFileStream.is_open())
		throw std::runtime_error("Could not oped file: " + jsonFilePath.string());

	nlohmann::json jsonRootNode = nlohmann::json::parse(inputFileStream);
	
	nlohmann::json modeNode;
	if (GetOptionalNode(jsonRootNode, ArgNames::GenerateMode, modeNode))
	{
		mGenParameters.Read(modeNode);
	}

	if (GetOptionalNode(jsonRootNode, ArgNames::BooleanMode, modeNode))
	{
		mBoolParameters.Read(modeNode);
	}

	if (GetOptionalNode(jsonRootNode, ArgNames::OffsetMode, modeNode))
	{
		mOffsetParameters.Read(modeNode);
	}
}

bool ParametersReader::GetOptionalNode(const nlohmann::json & rootNode, const std::string & propertyName, nlohmann::json & value)
{
	auto paramNode = rootNode.find(propertyName);
	if (paramNode == rootNode.end())
		return false;

	value = paramNode->flatten().unflatten();
	return true;
}
