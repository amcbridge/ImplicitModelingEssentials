#include "Operations.h"

void Operations::GenerateLattice(const GenerateParameters& params)
{
	auto latticeGrid = openvdb::FloatGrid::create();
	latticeGrid->setGridClass(openvdb::GRID_LEVEL_SET);
	latticeGrid->setName("lattice");

	openvdb::math::Mat4d matrix(
		size.x(), 0.f, 0.f, 0.f,
		0.f, size.y(), 0.f, 0.f,
		0.f, 0.f, size.z(), 0.f,
		0.f, 0.f, 0.f, 1.f);

	openvdb::math::Transform::Ptr transform = openvdb::math::Transform::createLinearTransform(matrix);
	latticeGrid->setTransform(transform);

	openvdb::Vec3i gridSize(params.GetGridDimensions());
	openvdb::CoordBBox gridBbox(openvdb::Coord(0), openvdb::Coord(gridSize));
	FillGridWithLattice(params.GetLatticeType(), latticeGrid, gridBbox);
	IO::Write(latticeGrid, params.GetOutputPath());
}

void Operations::Boolean(const BooleanParameters & params)
{
	auto aGrid = IO::Read(params.GetInputPath());
	auto bGrid = IO::Read(params.GetSecondInputPath());

	openvdb::math::Transform::Ptr transform = bGrid->transformPtr();
	transform->postTranslate(params.GetTranslation());
	
	openvdb::Mat4R xform =
		(*transform).baseMap()->getAffineMap()->getMat4() *
		aGrid->transform().baseMap()->getAffineMap()->getMat4().inverse();

	std::cout << "Transforming grid" << std::endl;

	openvdb::FloatGrid::Ptr transformedBGrid = openvdb::FloatGrid::create(bGrid->background());
	transformedBGrid->setTransform(aGrid->transformPtr());
	openvdb::tools::GridTransformer transformer(xform);
	
	transformer.transformGrid<openvdb::tools::BoxSampler>(*bGrid, *transformedBGrid);
	std::cout << "Transform done" << std::endl;

	if (params.GetOperation() == "union")
	{
		openvdb::tools::csgUnion(*aGrid, *transformedBGrid);
	}
	else if (params.GetOperation() == "difference")
	{
		openvdb::tools::csgDifference(*aGrid, *transformedBGrid);
	}
	else if (params.GetOperation() == "intersection")
	{
		openvdb::tools::csgIntersection(*aGrid, *transformedBGrid);
	}
	else
	{
		throw std::runtime_error("Unknown boolean operation: " + params.GetOperation());
	}

	IO::Write(aGrid, params.GetOutputPath());
}

void Operations::OffsetFill(const OffsetParameters & params)
{
	auto mainGrid = IO::Read(params.GetInputPath());
	openvdb::FloatGrid::Ptr innerGrid = mainGrid->deepCopy();

	std::cout << "Offsetting grid" << std::endl;

	openvdb::tools::LevelSetFilter<openvdb::FloatGrid> filter(*innerGrid);
	filter.offset(params.GetOffsetValue());

	std::cout << "Offset finished" << std::endl;

	openvdb::CoordBBox gridSize = innerGrid->evalActiveVoxelBoundingBox();
	auto background = innerGrid->background();

	openvdb::FloatGrid::Ptr lattice = openvdb::FloatGrid::create(background);
	lattice->setGridClass(openvdb::GRID_LEVEL_SET);
	lattice->setTransform(innerGrid->transformPtr());
	FillGridWithLattice(params.GetLatticeType(), lattice, gridSize);
	openvdb::tools::csgIntersection(*innerGrid, *lattice);
	openvdb::tools::csgDifference(*mainGrid, *innerGrid);
	
	IO::Write(mainGrid, params.GetOutputPath());
}

Lattice& Operations::GetLatticeType(const std::string & name)
{
	if (name == "primitive")
	{
		static PrimitiveLattice primitive;
		return primitive;
	}
	else if (name == "gyroid")
	{
		static GyroidLattice gyroid;
		return gyroid;
	}
	else if (name == "diamond")
	{
		static DiamondLattice diamond;
		return diamond;
	}

	throw std::runtime_error("Unknown input format: " + name);
}

void Operations::FillGridWithLattice(const std::string & latticeType, openvdb::FloatGrid::Ptr grid, const openvdb::CoordBBox & bbox)
{
	std::cout << "Filling grid with lattice" << std::endl;
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

	std::cout << "Grid filled" << std::endl;
}
