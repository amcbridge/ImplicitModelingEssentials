#include "Operations.h"

void Operations::GenerateLattice(float voxelSize, int gridDimension, const std::string& latticeType, const std::string& outPath)
{
	auto latticeGrid = openvdb::FloatGrid::create();
	latticeGrid->setGridClass(openvdb::GRID_LEVEL_SET);
	latticeGrid->setName("lattice");

	openvdb::math::Mat4d matrix(
		voxelSize, 0.f, 0.f, 0.f,
		0.f, voxelSize, 0.f, 0.f,
		0.f, 0.f, voxelSize, 0.f,
		0.f, 0.f, 0.f, 1.f);

	openvdb::math::Transform::Ptr transform = openvdb::math::Transform::createLinearTransform(matrix);
	latticeGrid->setTransform(transform);

	openvdb::Vec3i gridSize(gridDimension);
	openvdb::CoordBBox gridBbox(openvdb::Coord(0), openvdb::Coord(gridSize));
	FillGridWithLattice(latticeType, latticeGrid, gridBbox);
	IO::Write(latticeGrid, outPath);
}

void Operations::Boolean(const std::string& inputA, const std::string& inputB, const std::string& type,
	float translation[3], const std::string& outPath)
{
	auto aGrid = IO::Read(inputA);
	auto bGrid = IO::Read(inputB);
	std::string operationType(type);
	std::transform(operationType.begin(), operationType.end(), operationType.begin(), ::tolower);

	openvdb::math::Transform::Ptr transform = bGrid->transformPtr();
	transform->postTranslate(openvdb::Vec3s(translation));
	
	openvdb::Mat4R xform =
		(*transform).baseMap()->getAffineMap()->getMat4() *
		aGrid->transform().baseMap()->getAffineMap()->getMat4().inverse();

	openvdb::FloatGrid::Ptr transformedBGrid = openvdb::FloatGrid::create(bGrid->background());
	transformedBGrid->setTransform(aGrid->transformPtr());
	openvdb::tools::GridTransformer transformer(xform);
	
	transformer.transformGrid<openvdb::tools::BoxSampler>(*bGrid, *transformedBGrid);

	if (operationType == "union")
	{
		openvdb::tools::csgUnion(*aGrid, *transformedBGrid);
	}
	else if (operationType == "difference")
	{
		openvdb::tools::csgDifference(*aGrid, *transformedBGrid);
	}
	else if (operationType == "intersection")
	{
		openvdb::tools::csgIntersection(*aGrid, *transformedBGrid);
	}
	else
	{
		throw std::runtime_error("Unknown boolean operation: " + type);
	}

	IO::Write(aGrid, outPath);
}

void Operations::OffsetFill(float offset, const std::string& inputPath, const std::string& latticeType, const std::string& outPath)
{
	auto mainGrid = IO::Read(inputPath);
	openvdb::FloatGrid::Ptr innerGrid = mainGrid->deepCopy();

	openvdb::tools::LevelSetFilter<openvdb::FloatGrid> filter(*innerGrid);
	filter.offset(offset);

	openvdb::CoordBBox gridSize = innerGrid->evalActiveVoxelBoundingBox();
	auto background = innerGrid->background();

	openvdb::FloatGrid::Ptr lattice = openvdb::FloatGrid::create(background);
	lattice->setGridClass(openvdb::GRID_LEVEL_SET);
	lattice->setTransform(innerGrid->transformPtr());
	FillGridWithLattice(latticeType, lattice, gridSize);
	openvdb::tools::csgIntersection(*innerGrid, *lattice);
	openvdb::tools::csgDifference(*mainGrid, *innerGrid);
	
	IO::Write(mainGrid, outPath);
}

Lattice& Operations::GetLatticeType(const std::string & name)
{
	std::string type(name);
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	if (type == "primitive")
	{
		static PrimitiveLattice primitive;
		return primitive;
	}
	else if (type == "gyroid")
	{
		static GyroidLattice gyroid;
		return gyroid;
	}
	else if (type == "diamond")
	{
		static DiamondLattice diamond;
		return diamond;
	}

	throw std::runtime_error("Unknown input format: " + name);
}

void Operations::FillGridWithLattice(const std::string & latticeType, openvdb::FloatGrid::Ptr grid, const openvdb::CoordBBox & bbox)
{
	Lattice& lattice = Operations::GetLatticeType(latticeType);
	typename openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
	openvdb::Vec3s size(grid->voxelSize());

	openvdb::Coord ijk;
	int &i = ijk[0], &j = ijk[1], &k = ijk[2];
	for (i = bbox.min().x(); i <= bbox.max().x(); ++i)
	{
		for (j = bbox.min().y(); j < bbox.max().y(); ++j)
		{
			for (k = bbox.min().z(); k < bbox.max().z(); ++k)
			{
				auto wCoords = grid->transform().indexToWorld(ijk);
				double x = i * size.x();
				double y = j * size.y();
				double z = k * size.z();

				double latticeSDF = lattice.Calculate(x, y, z);
				if (latticeSDF <= 0)
				{
					accessor.setValue(ijk, latticeSDF);
				}
			}
		}
	}
}
