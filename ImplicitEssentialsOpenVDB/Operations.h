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
		static void GenerateLattice(const GenerateParameters& params);
		static void Boolean(const BooleanParameters& params);
		static void OffsetFill(const OffsetParameters& params);
	private:
		static Lattice& GetLatticeType(const std::string& name);
		static void FillGridWithLattice(const std::string& latticeType, openvdb::FloatGrid::Ptr grid, 
			const openvdb::CoordBBox& bbox);
};

