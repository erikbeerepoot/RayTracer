#ifndef __GEOMETRIC_TYPES_HPP
#define __GEOMETRIC_TYPES_HPP
/**
 * Filename:	GeometricTypes.hpp 
 * Purpose:		Define the geometric types used in the RayTracer, including units
 * Author:		Erik E. Beerepoot
 */
#include <stdint.h>
#include <math.h>
/*********************************
 ******    Unit Templates    *****
 *********************************/
template<int M, int R, int K,int S> struct Unit {
	enum { m = M, rad = R, kg = K, s = S };
};

template <typename Unit>
struct Value {
	double val;
	double get() { return val; };
	explicit constexpr Value(double d) : val(d){}
};

using NoUnit = Unit<0,0,0,0>;
using Second = Unit<0,0,0,1>;
using Second2 = Unit<0,0,0,2>;
using Metre = Unit<1,0,0,0>;
using Rad = Unit<0,1,0,0>;
using MeterSecond = Unit<1,0,0,-1>;
using RadSecond = Unit<0,1,0,-1>;

//second
constexpr Value<Second> operator"" _s(long double d)
{
	return Value<Second>(d);
}

//second^2
constexpr Value<Second2> operator "" _s2(long double d)
{
	return Value<Second2>(d);
}

//metre
constexpr Value<Metre> operator "" _m(long double d)
{
	return Value<Metre>(d);
}

//rad
constexpr Value<Rad> operator "" _rad(long double d)
{
	return Value<Rad>(d);
}
template <int M1, int R1, int K1, int S1,int M2, int R2, int K2, int S2>
Value<Unit<M1-M2,R1-R2,K1-K2,S1-S2>> operator/ (Value<Unit<M1,R1,K1,S1>> lhs,Value<Unit<M2,R2,K2,S2>> rhs) {
	 return Value<Unit<M1-M2,R1-R2,K1-K2,S1-S2>>(lhs.get()/rhs.get());
}

template <int M1, int R1, int K1, int S1>
Value<Unit<M1,R1,K1,S1>> operator/ (Value<Unit<M1,R1,K1,S1>> lhs,double val) {
	 return Value<Unit<M1,R1,K1,S1>>(lhs.get()/val);
}

template <int M1, int R1, int K1, int S1,int M2, int R2, int K2, int S2>
Value<Unit<M1+M2,R1+R2,K1+K2,S1+S2>> operator* (Value<Unit<M1,R1,K1,S1>> lhs,Value<Unit<M2,R2,K2,S2>> rhs) {
	 return Value<Unit<M1+M2,R1+R2,K1+K2,S1+S2>>(lhs.get()*rhs.get());
}

template <int M1, int R1, int K1, int S1>
Value<Unit<M1,R1,K1,S1>> operator* (Value<Unit<M1,R1,K1,S1>> lhs,double val) {
	 return Value<Unit<M1,R1,K1,S1>>(lhs.get()*val);
}

template <int M, int R, int K, int S>
bool operator < (Value<Unit<M,R,K,S>> lhs, Value<Unit<M,R,K,S>> rhs){
	return lhs.get()<rhs.get();
}

template <int M, int R, int K, int S>
bool operator > (Value<Unit<M,R,K,S>> lhs, Value<Unit<M,R,K,S>> rhs){
	return lhs.get()>rhs.get();
}

template <int M, int R, int K, int S>
bool operator == (Value<Unit<M,R,K,S>> lhs, Value<Unit<M,R,K,S>> rhs){
	return lhs.get()==rhs.get();
}


template <int M, int R, int K, int S>
bool operator <= (Value<Unit<M,R,K,S>> lhs, Value<Unit<M,R,K,S>> rhs){
	return lhs.get()<=rhs.get();
}

template <int M, int R, int K, int S>
bool operator >= (Value<Unit<M,R,K,S>> lhs, Value<Unit<M,R,K,S>> rhs){
	return lhs.get()>=rhs.get();
}

template <int M, int R, int K, int S>
Value<Unit<M,R,K,S>> operator- (Value<Unit<M,R,K,S>> lhs, Value<Unit<M,R,K,S>>&rhs){
	return Value<Unit<M,R,K,S>>(lhs.get()-rhs.get());
}

template <int M, int R, int K, int S>
Value<Unit<M,R,K,S>> operator+ (Value<Unit<M,R,K,S>> lhs, Value<Unit<M,R,K,S>> rhs){
	return Value<Unit<M,R,K,S>>(lhs.get()+rhs.get());
}

/*********************************
 ****** Euclidian primitives *****
 *********************************/
typedef Value<Unit<0,0,0,0>> DimensionLess;
typedef Value<Metre> Distance;
typedef Value<Rad> Angle;
typedef Value<Second> Time;
typedef Value<MeterSecond> LinearVelocity;
typedef Value<RadSecond> AngularVelocity;

struct Point {
	Distance x;
	Distance y;
	Distance z;
	Point(Distance xx,Distance yy, Distance zz) : x(xx),y(yy),z(zz) {};
	
	friend Point operator+ (Point& lhs,Point& rhs){
		return Point(lhs.x+rhs.x,lhs.y+rhs.y,lhs.z+rhs.z);
	}
	
	friend bool operator<= (Point& lhs,Point& rhs){
		return ((lhs.x<=rhs.x)&&(lhs.y<=rhs.y)&&(lhs.z<=rhs.z));
	}
	
	friend bool operator>= (Point& lhs,Point& rhs){
		return ((lhs.x>=rhs.x)&&(lhs.y>=rhs.y)&&(lhs.z>=rhs.z));
	}
	
	friend bool operator== (Point& lhs,Point& rhs){
		return ((lhs.x==rhs.x)&&(lhs.y==rhs.y)&&(lhs.z==rhs.z));
	}
};

struct Orientation {
	Angle roll;
	Angle pitch;
	Angle yaw;
	Orientation(Angle r,Angle p,Angle y) : roll(r),pitch(p),yaw(y) {};
};

struct Size {
	Distance length;
	Distance width;
	Distance height;
	Size(Distance l,Distance w,Distance h) : length(l),width(w),height(h) {};
};

struct Velocity {
	LinearVelocity V_x;
	LinearVelocity V_y;
	LinearVelocity V_z;
	AngularVelocity w_x;
	AngularVelocity w_y;
	AngularVelocity w_z;
	Velocity(LinearVelocity x,LinearVelocity y,LinearVelocity z,AngularVelocity wx, AngularVelocity wy, AngularVelocity wz) : V_x(x),V_y(y), V_z(z), w_x(wx),w_y(wy),w_z(wz) {};
};

/*********************************
 ******** OpenCL types   *********
 *********************************/

struct cl_Point {
    float x;
    float y;
    float z;
};

/*********************************
 ******** Graphics types *********
 *********************************/
typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_t;

Point AzInclRangeToXYZ(Angle az, Angle incl, Distance r);
Point RotateXYZ(Point p,Angle pitch, Angle roll, Angle yaw);

#endif