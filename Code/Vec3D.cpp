#include "Vec3D.h"

using namespace std;

Vec3D::Vec3D(void) { // No-args constructor.
	x = 0;
	y = 0;
	z = 0;
}

Vec3D::Vec3D(const Vec3D & vec) { // Copy constructor.
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

Vec3D::Vec3D(double X, double Y, double Z) {
	x = X;
	y = Y;
	z = Z;
}

Vec3D::~Vec3D(void) { // Destructor. 
}

double Vec3D::Abs(void) {
	return sqrt(x*x + y*y + z*z);
}

void Vec3D::SetXYZ(double X, double Y, double Z) {
	x = X;
	y = Y;
	z = Z;
}

Vec3D & Vec3D::operator=(const Vec3D & vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

Vec3D Vec3D::operator*(const Vec3D & vec) { // Implements cross product.
	Vec3D result;
	
	result.x = (this->y)*vec.z - (this->z)*vec.y;
	result.y = (this->z)*vec.x - (this->x)*vec.z;
	result.z = (this->x)*vec.y - (this->y)*vec.x;
	
	return result;
}

Vec3D Vec3D::operator+(const Vec3D & vec) {
	Vec3D result;
	
	result.x = (this->x) + vec.x;
	result.y = (this->y) + vec.y;
	result.z = (this->z) + vec.z;
	
	return result;
}

Vec3D Vec3D::operator-(const Vec3D & vec) {
	Vec3D result;
	
	result.x = (this->x) - vec.x;
	result.y = (this->y) - vec.y;
	result.z = (this->z) - vec.z;
	
	return result;
}

Vec3D Vec3D::operator*(const double & c) {
	Vec3D result;
	
	result.x = (this->x)*c;
	result.y = (this->y)*c;
	result.z = (this->z)*c;
	
	return result;
}

Vec3D Vec3D::operator/(const double & c) {
	Vec3D result;
	
	result.x = (this->x)/c;
	result.y = (this->y)/c;
	result.z = (this->z)/c;
	
	return result;
}

double Vec3D::Dot(const Vec3D & vec) { // Implements dot product.
	return (this->x)*vec.x + (this->y)*vec.y + (this->z)*vec.z;
}

double Vec3D::GetX(void) {
	return x;
}
double Vec3D::GetY(void) {
	return y;
}
double Vec3D::GetZ(void) {
	return z;
}

std::ostream& operator<<(std::ostream & os, Vec3D & vec) {
	os << "(" << vec.GetX() << "," << vec.GetY() << "," << vec.GetZ() << ")";
	return os;
}
