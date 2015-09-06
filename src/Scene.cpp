
/**
 *      ___           ___           ___           ___
 *     /\  \         /\  \         /\  \         /\__\
 *    /::\  \       /::\  \       /::\  \       /:/  /
 *   /:/\:\  \     /:/\ \  \     /:/\:\  \     /:/  /
 *  /::\~\:\  \   _\:\~\ \  \   /::\~\:\  \   /:/  /
 * /:/\:\ \:\__\ /\ \:\ \ \__\ /:/\:\ \:\__\ /:/__/
 * \/__\:\/:/  / \:\ \:\ \/__/ \/_|::\/:/  / \:\  \
 *      \::/  /   \:\ \:\__\      |:|::/  /   \:\  \
 *      /:/  /     \:\/:/  /      |:|\/__/     \:\  \
 *     /:/  /       \::/  /       |:|  |        \:\__\
 *     \/__/         \/__/         \|__|         \/__/
 * -----------------------------------------------------
 * ----------> Autonomous Space Robotics Lab <----------
 * -----------------------------------------------------
 * /name    Scene.hpp
 * /brief   Implementation for Scene class. Allows the construction of a fixed size scene and the 
 * addition of primitives.
 * /author  Erik E. Beerepoot
 */

 #include "Scene.hpp"
 #include <string>
 #include <iostream>

 /** 
  * /name Scene
  * /brief Constructs scene with default size 
  */
Scene::Scene() : _sceneSize(Size(5.0_m,5.0_m,2.0_m)), _gridDim(Size(0.01_m,0.01_m,0.01_m)) {
	_sceneData = NULL;
	
	//need try catch
	AllocScene();
	
	
}

 /** 
  * /name Scene
  * /brief Constructs scene with custom size 
  */
Scene::Scene(Size size) : _sceneSize(size),_gridDim(0.01_m,0.01_m,0.01_m){
	_sceneData = NULL;
	
	//need try catch
	AllocScene();
}

/**
 * /name AllocScene
 * /brief Allocate memory for scene pixels
 */
void Scene::AllocScene(){
	long long len = static_cast<long long>((_sceneSize.length/_gridDim.length).get());
	long long wid = static_cast<long long>((_sceneSize.width/_gridDim.width).get());
	long long hei = static_cast<long long>((_sceneSize.height/_gridDim.height).get());
	
	_sceneData = new pixel_t**[len];
	for(long long x = 0; x < len; ++x){
		_sceneData[x] = new pixel_t*[wid];
		 for(long long y = 0; y <wid; ++y){
			 _sceneData[x][y] = new pixel_t[hei];
			 for(long long z = 0; z < hei; ++z){
				 _sceneData[x][y][z].red = 0;
				 _sceneData[x][y][z].green = 0;
				 _sceneData[x][y][z].blue = 0;
			 }
		 }
	 }
}

/**
 * /name DeallocScene
 * /brief Deallocate scene memory
 */
void Scene::DeallocScene(){
	long long len = static_cast<long>((_sceneSize.length/_gridDim.length).get());
	long long wid = static_cast<long>((_sceneSize.width/_gridDim.width).get());

	for(long long x = 0; x <  len; ++x)
	{
		for(long long y = 0; y <  wid; ++y)
		{
			delete[] _sceneData[x][y];
		}

		delete[] _sceneData[x];
	}

	delete[] _sceneData;
}
		
/**
 * /name ~Scene
 * /brief Destructor for Scene class
 */
Scene::~Scene(){
	DeallocScene();
	_sceneData = NULL;
}

/**
 * /name CheckPoints 
 * /brief Checks the points in the points vector for intersection with scene objects. 
 */
pixel_t Scene::CheckPoints(std::vector<Point>& points) const{
	static pixel_t emptyPix;
	
	for(auto it=points.begin();it!=points.end();++it){
			Point tmp = *it;
			if(ClipPoint(tmp)) return emptyPix;
			pixel_t pix = *((Scene*)this)->At(tmp);
			if(pix.red!=0 || pix.blue!=0 || pix.green!=0) return pix;
	}
	return emptyPix;
}
 
/**
 * /name AddPlane
 * /brief Adds a plane to the current scene, clipping it if necessary.
 */
void Scene::AddPlane(Point p1,Point p2,pixel_t color){
	ClipPoint(p1);
	ClipPoint(p2);
	
	//ERROR: need proper directory checking
	for(auto x = p1.x; x <= p2.x; x = x + _gridDim.length){
		for(auto y = p1.y; y <= p2.y; y = y + _gridDim.width){
			for(auto z = p1.z; z <= p2.z; z = z + _gridDim.height){
				At(Point(x,y,z))->red = color.red;
				At(Point(x,y,z))->blue = color.blue;
				At(Point(x,y,z))->green = color.green;
			}
		}	
	}
}

		
/**
 * /name AddRightCuboid
 * /brief Adds a AddRightCuboid to the current scene, clipping it if necessary.
 */
void Scene::AddRightCuboid(Point centroid, Size size){
}

/**
 * /name ClipPoint
 * /brief Performs clipping on the point, modifies the point if required. Returns true if clipped.
 */
bool Scene::ClipPoint(Point& point) const{
	Point oldPoint = point;
	if(point.x < 0.0_m) point.x = 0.0_m;
	if(point.x > _sceneSize.length) point.x = _sceneSize.length;
	if(point.y < 0.0_m) point.y = 0.0_m;
	if(point.y > _sceneSize.width) point.y = _sceneSize.width;
	if(point.z < 0.0_m) point.z = 0.0_m;
	if(point.z > _sceneSize.height) point.z = _sceneSize.height;
	return !(oldPoint==point);
}

/**
 * /name At
 * /brief Returns pixel at the Point "pix"
 */
pixel_t* Scene::At(Point pix){
	int idx,idy,idz;
	idx = (pix.x / _gridDim.length).get();
	idy = (pix.y / _gridDim.width).get();
	idz = (pix.z / _gridDim.height).get();
	return _sceneData[idx][idy]  + idz;
}

/**
 * /name ClipRightCuboid
 * /brief Performs clipping on the Right Cuboid, modifies the points defining the plane if required.
 */
void Scene::ClipRightCuboid(Point& centroid, Size& size){
}