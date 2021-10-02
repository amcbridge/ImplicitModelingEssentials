#pragma once
#include "ParametersReader.h"
#include "IO.h"
#include <openvdb/tools/LevelSetFilter.h>
#include "Lattice.h"
#include <openvdb/tools/Composite.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/GridTransformer.h>

class Operations
{
	public:	
		static void GenerateLattice(float voxelSize, int gridDimension, const std::string& latticeType, const std::string& outPath);
		static void Boolean(const BooleanParameters& params);
		static void OffsetFill(float offset, const std::string& inputPath, const std::string& latticeType, const std::string& outPath);
	private:
		static Lattice& GetLatticeType(const std::string& name);
		static void FillGridWithLattice(const std::string& latticeType, openvdb::FloatGrid::Ptr grid, 
			const openvdb::CoordBBox& bbox);
};

