#pragma once
#include <openvdb/openvdb.h>
#include <openvdb/io/Stream.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <openvdb/io/Stream.h>
#include <openvdb/tools/VolumeToMesh.h>

const std::string VDBExtension(".vdb");
const std::string OBJExtension(".obj");

class IO
{
	public:
		static openvdb::FloatGrid::Ptr Read(const std::filesystem::path& filePath);
		static void Write(openvdb::FloatGrid::Ptr grid, const std::filesystem::path& filePath);

	private:
		static void ExportOBJ(const std::string& fileName, const std::vector<openvdb::math::Vec3s>& points, const std::vector<openvdb::Vec4I>& quads);
};