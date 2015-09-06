#include "GeometricTypes.hpp"

Point AzInclRangeToXYZ(Angle az, Angle incl, Distance r){
	Distance x = r*sin(incl.get())*cos(az.get());
	Distance y = r*sin(incl.get())*sin(az.get());
	Distance z = r*cos(incl.get());
    
    
	return Point(x,y,z);
}

Point RotateXYZ(Point p,Angle pitch, Angle roll, Angle yaw){
    
    return p;
}