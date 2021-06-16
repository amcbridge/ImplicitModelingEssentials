#pragma once
#include <tbb/tbb.h>
#include <openvdb/openvdb.h>

using namespace tbb;

class Generator
{
	public:
		void operator()(const blocked_range<size_t>& r) const 
		{
			//openvdb::Coord ijk;
			//int &i = ijk[0], &j = ijk[1], &k = ijk[2];
			//typename openvdb::FloatGrid::Accessor accessor = mGrid->getAccessor();
			//for (size_t i = r.begin(); i != r.end(); ++i)
			//{
			//	for (j = MIN; j < MAX; ++j)
			//	{
			//		for (k = MIN; k < MAX; ++k)
			//		{
			//			double x = i * size.x();
			//			double y = j * size.y();
			//			double z = k * size.z();

			//			double latticeSDF = latticeType.Calculate(x, y, z);
			//			if (latticeSDF <= 0)
			//			{
			//				accessor.setValue(ijk, latticeSDF);
			//			}
			//		}
			//	}
			//}
		}

		Generator(openvdb::FloatGrid::Ptr grid) 
		{
			mGrid = grid;
		}

	private:
		openvdb::FloatGrid::Ptr mGrid;

};

