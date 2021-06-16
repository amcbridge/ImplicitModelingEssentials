#include "Lattice.h"

Lattice::Lattice() 
{
	mKx = k(nx);
	mKy = k(ny);
	mKz = k(nz);
}
