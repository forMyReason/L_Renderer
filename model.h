#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

// 提供了对三维模型的一些操作

class Model {
private:
	std::vector<Vec3f> verts_;				// 顶点集，每个顶点都是三维向量
	std::vector<std::vector<int> > faces_;	// 面片集
public:
	Model(const char* filename);
	~Model();
	int nverts();							// 返回 顶点数量
	int nfaces();							// 返回 面片数量
	Vec3f vert(int i);						// 返回 第i个顶点
	std::vector<int> face(int idx);			// 返回 第idx个面片
};

#endif //__MODEL_H__