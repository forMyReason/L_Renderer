#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

// �ṩ�˶���άģ�͵�һЩ����

class Model {
private:
	std::vector<Vec3f> verts_;				// ���㼯��ÿ�����㶼����ά����
	std::vector<std::vector<int> > faces_;	// ��Ƭ��
public:
	Model(const char* filename);
	~Model();
	int nverts();							// ���� ��������
	int nfaces();							// ���� ��Ƭ����
	Vec3f vert(int i);						// ���� ��i������
	std::vector<int> face(int idx);			// ���� ��idx����Ƭ
};

#endif //__MODEL_H__