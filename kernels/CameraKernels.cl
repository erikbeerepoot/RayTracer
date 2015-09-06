
struct cl_Point {
    float x;
    float y;
    float z;
};


__constant float kSpatialSamplingDistance = (float)0.05;
__constant float kPi = (float)(3.14159265358979);

__kernel void traceray(float distance,
                       const int row,
                       const int u_c,
                       const int v_c,
                       const float diff_u,
                       const float diff_v,
                       const float pitch_h,
                       const float pitch_v,
                       const struct cl_Point centre,
                       __global struct cl_Point *out_points){
    
    
    //determine location of this thread
    int u = get_global_id(0);
    int v = get_global_id(1);
    v = row;
    
    //Calculate current angle relative to image plane
	float theta_u = diff_u * (u_c - u); //+ _orientation.yaw;;
	float theta_v = diff_v * (v - v_c) + (kPi/2); // + _orientation.pitch;
	
	//Calculate origin of this point relative to image plane
	float y = pitch_h * (u - u_c);
	float z = pitch_v * (v - v_c);
    
    //Calculate step parameters
    struct cl_Point pixelLoc,pixStep;
    
    //calculate starting pixel
    pixelLoc.x = centre.x;
    pixelLoc.y = centre.y + y;
    pixelLoc.z = centre.z + z;
    
    //calculate pixel step
    pixStep.x = kSpatialSamplingDistance*sin(theta_v)*cos(theta_u);
	pixStep.y = kSpatialSamplingDistance*sin(theta_v)*sin(theta_u);
	pixStep.z = kSpatialSamplingDistance*cos(theta_v);
    
    int index = u*(distance/kSpatialSamplingDistance);
    for(float d=0.0;d<=distance;d+=kSpatialSamplingDistance){
        out_points[index].x = centre.x;
        out_points[index].y = centre.y + y;
        out_points[index].z = centre.z + z;
        index++;
    }
}