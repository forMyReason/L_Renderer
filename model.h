#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;		// ���㼯
	std::vector<std::vector<int> > faces_;	// ��Ƭ��
public:
	Model(const char* filename);
	~Model();
	int nverts();				// ���ض�������
	int nfaces();				// ������ƪ����
	Vec3f vert(int i);			// ���ص�i������
	std::vector<int> face(int idx);	// ���ص�i����Ƭ
};

#endif //__MODEL_H__