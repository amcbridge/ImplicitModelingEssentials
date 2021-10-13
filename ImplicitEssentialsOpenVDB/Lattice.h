#pragma once
#include <math.h>
#include <openvdb/openvdb.h>

const openvdb::Vec3s size(0.01);

class Lattice
{
	public:
		Lattice();
		virtual double Calculate(double x, double y, double z) const = 0; 

	protected:
		double Cx(double x) const { return C(mKx, x, Lx); }
		double Cy(double y) const { return C(mKy, y, Ly); }
		double Cz(double z) const { return C(mKz, z, Lz); }
		double Sx(double x) const { return S(mKx, x, Lx); }
		double Sy(double y) const { return S(mKy, y, Ly); }
		double Sz(double z) const { return S(mKz, z, Lz); }

	private:
		double k(int n) const { return 2 * M_PI * n; }
		double S (double k, double i, double L) const { return sin(k * i / L); }
		double C (double k, double i, double L) const { return cos(k * i / L); }

		double mKx = 0.0;
		double mKy = 0.0;
		double mKz = 0.0;

		double Lx = 1;
		double Ly = 1;
		double Lz = 1;

		int nx = 1;
		int ny = 1;
		int nz = 1;
};


class PrimitiveLattice : public Lattice
{
	public:
		virtual double Calculate(double x, double y, double z) const override
		{
			return Cx(x) + Cy(y) + Cz(z);
		}
};

class GyroidLattice : public Lattice
{
	public:
		virtual double Calculate(double x, double y, double z) const override
		{
			return Cx(x) * Sy(y) + Cy(y) * Sz(z) + Cz(z) * Sx(x);
		}
};

class DiamondLattice : public Lattice
{
	public:
		virtual double Calculate(double x, double y, double z) const override
		{
			return Sx(x) * Sy(y) * Sz(z) + Sx(x) * Cy(y) * Cz(z) + Cx(x) * Sy(y) * Cz(z) + Cx(x) * Cy(y) * Sz(z);
		}
};

