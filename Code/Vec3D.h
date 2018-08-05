#ifndef _VEC3D_H_
#define _VEC3D_H_

#include <iostream>
#include <math.h> 

class Vec3D {
private:
	double x, y, z;

public:
	// Constructors.
	Vec3D(void);
	Vec3D(const Vec3D &);
	Vec3D(double X, double Y, double Z);
	~Vec3D(void);
	// Methods.
	double Abs(void);
	void SetXYZ(double, double, double);
	double GetX(void);
	double GetY(void);
	double GetZ(void);
	double Dot(const Vec3D & ); // Implements dot product.
	// Operators.
	Vec3D & operator=(const Vec3D &);
	Vec3D operator*(const Vec3D &); // Implements cross product.
	Vec3D operator+(const Vec3D &);
	Vec3D operator-(const Vec3D &);
	Vec3D operator*(const double &);
	Vec3D operator/(const double &);
};

std::ostream& operator<<(std::ostream& os, Vec3D& vec);

#endif
