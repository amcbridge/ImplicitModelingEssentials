#pragma once

#include "nlohmann/json.hpp"
#include <openvdb/openvdb.h>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

class ArgNames
{
	public:
		static const inline std::string InputPath = "inputPath";
		static const inline std::string SecondInputPath = "secondInputPath";
		static const inline std::string OutputPath = "outputPath";
		static const inline std::string VoxelSize = "voxelSize";
		static const inline std::string LatticeType = "latticeType";
		static const inline std::string Offset = "offset";
		static const inline std::string GenerateMode = "generate";
		static const inline std::string Translation = "translation";
		static const inline std::string GridDimensions = "gridDimensions";
		static const inline std::string BooleanMode = "boolean";
		static const inline std::string OffsetMode = "offsetFill";
		static const inline std::string Operation = "operation";
};

class Parameters 
{
	public:	
		virtual void Read(const nlohmann::json& node);
		const std::filesystem::path& GetOutputPath() const { return mOutputPath; }
		const bool AreEmpty() const { return mEmpty; }
	protected:
		std::filesystem::path mOutputPath;
		bool mEmpty = true;

		template<typename T>
		static T GetRequiredParameter(const nlohmann::json& node, const std::string& paramName);

		template<typename T>
		static bool GetOptionalParameter(const nlohmann::json& node, const std::string& paramName, T& value);
};

class GenerateParameters : public Parameters
{
	public:
		const openvdb::Vec3s& GetVoxelSize() const { return mVoxelSize; }
		const openvdb::Vec3i& GetGridDimensions() const { return mGridDimensions; }
		const std::string& GetLatticeType() const { return mLatticeType; }
		virtual void Read(const nlohmann::json& node) override;
	private:
		std::string mLatticeType;
		openvdb::Vec3s mVoxelSize;
		openvdb::Vec3i mGridDimensions;
};

class BooleanParameters : public Parameters
{
	public:
		const std::filesystem::path& GetInputPath() const { return mInputPath; }
		virtual void Read(const nlohmann::json& node) override;
		const std::filesystem::path& GetSecondInputPath() const { return mSecondInputPath; }
		const std::string& GetOperation() const { return mOperation; }
		const openvdb::Vec3s& GetTranslation() const { return mTranslation; }

	private:
		std::filesystem::path mInputPath;
		std::filesystem::path mSecondInputPath;
		std::string mOperation;
		openvdb::Vec3s mTranslation;
};

class OffsetParameters : public Parameters
{
	public:
		std::filesystem::path mInputPath;
		const std::filesystem::path& GetInputPath() const { return mInputPath; }
		const std::string& GetLatticeType() const { return mLatticeType; }
		float GetOffsetValue() const { return mOffset; }
		virtual void Read(const nlohmann::json& node) override;

	private:
		std::string mLatticeType;
		float mOffset;
};

class ParametersReader
{
	public:
		ParametersReader(const std::filesystem::path& jsonFilePath);
		const GenerateParameters& GetGenerateParameters() const { return mGenParameters; }
		const BooleanParameters& GetBooleanParameters() const { return mBoolParameters; }
		const OffsetParameters& GetOffsetParameters() const { return mOffsetParameters; }

	private:
		GenerateParameters mGenParameters;
		BooleanParameters mBoolParameters;
		OffsetParameters mOffsetParameters;

		static bool GetOptionalNode(const nlohmann::json& rootNode, const std::string& propertyName, nlohmann::json& value);
};
