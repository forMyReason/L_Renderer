#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;		// 顶点集
	std::vector<std::vector<int> > faces_;	// 面片集
public:
	Model(const char* filename);
	~Model();
	int nverts();				// 返回顶点数量
	int nfaces();				// 返回面篇数量
	Vec3f vert(int i);			// 返回第i个顶点
	std::vector<int> face(int idx);	// 返回第i个面片
};

#endif //__MODEL_H__