#include "IO.h"

openvdb::FloatGrid::Ptr IO::Read(const std::filesystem::path & filePath)
{
	std::cout << "Reading file: " << filePath.filename() << std::endl;

	std::string extension(filePath.extension().string());
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	if (extension == VDBExtension)
	{
		openvdb::io::File file(filePath.string());
		file.open();
		openvdb::GridPtrVecPtr gridsf;
		gridsf = file.getGrids();
		file.close();

		openvdb::FloatGrid::Ptr grid = openvdb::gridPtrCast<openvdb::FloatGrid>(gridsf->at(0));
		grid->setGridClass(openvdb::GRID_LEVEL_SET);
		return grid;
	}
	else
	{
		throw std::runtime_error("Unknown input format: " + extension);
	}
}

void IO::Write(openvdb::FloatGrid::Ptr grid, const std::filesystem::path& filePath)
{
	std::cout << "Writing output" << std::endl;
	std::string extension(filePath.extension().string());
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	if (extension == VDBExtension)
	{
		openvdb::GridPtrVec grids;
		openvdb::io::File file(filePath.string());
		grids.push_back(grid);
		file.write(grids);
		file.close();
	}
	else if (filePath.extension() == OBJExtension)
	{
		std::vector<openvdb::math::Vec3s> points;
		std::vector<openvdb::Vec4I> quads;
		openvdb::tools::volumeToMesh(*grid, points, quads, 0);
		IO::ExportOBJ(filePath.string(), points, quads);
	}
	else
	{
		throw std::runtime_error("Unknown output format" + extension);
	}
}

void IO::ExportOBJ(const std::string& fileName, const std::vector<openvdb::math::Vec3s>& points, const std::vector<openvdb::Vec4I>& quads)
{
	std::ofstream out(fileName);

	for (const auto& point : points)
	{
		out << "v " << point.x() << " " << point.y() << " " << point.z() << "\n";
	}

	for (const auto& quad : quads)
	{
		out << "f " << quad.x() + 1 << " " << quad.y() + 1 << " " << quad.z() + 1 << " " << quad.w() + 1 << "\n";
	}
}