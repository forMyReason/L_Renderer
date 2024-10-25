#include "geometry.h"
#include "line.h"
#include "model.h"
#include "tgaimage.h"
#include <cmath>
#include <vector>
using namespace std;

Model* model = NULL;
const int width = 800;
const int height = 800;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

//����������(����1������2������3��tgaָ�룬��ɫ)
//void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color)
//{
//    if (t0.y == t1.y && t0.y == t2.y) return;
//
//    //����y�Ĵ�С�������������t0<t1<t2
//    if (t0.y > t1.y) std::swap(t0, t1);
//    if (t0.y > t2.y) std::swap(t0, t2);
//    if (t1.y > t2.y) std::swap(t1, t2);
//
//    int total_height = t2.y - t0.y;
//
//    //�Ը߶Ȳ���Ϊѭ�����Ʊ�������ʱ����Ҫ����б�ʣ���Ϊ��ɫ���ÿ�ж��ᱻ���
//    for (int i = 0; i < total_height; i++)
//    {
//        //����t1�������ηָ�Ϊ����������
//        bool second_half = i > t1.y - t0.y || t1.y == t0.y;     // �ϰ벿��
//        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
//        float alpha = (float)i / total_height;
//        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
//
//        //����A,B���������
//        Vec2i A = t0 + (t2 - t0) * alpha;
//        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
//        if (A.x > B.x) std::swap(A, B);
//
//        //����A,B�͵�ǰ�߶ȶ�tga��ɫ
//        for (int j = A.x; j <= B.x; j++)
//        {
//            image.set(j, t0.y + i, color);
//        }
//    }
//}

// ����������(�������飬zbufferָ�룬tgaָ�룬��ɫ)
void triangle(Vec3f *pts , float *zbuffer , TGAImage &image , TGAColor color)
{
    Vec2f bboxmin( - std::numeric_limits<float>::max(), - std::numeric_limits<float>::max());
    Vec2f bboxmax(   std::numeric_limits<float>::max(),   std::numeric_limits<float>::max());

    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

    //ȷ�������α߿�
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }
}

// ����������ԣ��жϵ�P�Ƿ����������ڲ����ҿɶ��������ڵ�ÿ���㶼ʹ��ABC������������ʾ�����������ֵ
// TODO:���������ǲ��Ǿ����������ꣿ
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P)
{
    Vec3f s[2];
    //����[AB,AC,PA]��x��y����
    for (int i = 2; i--; )
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    //[u,v,1]��[AB,AC,PA]��Ӧ��x��y��������ֱ�����Բ��
    Vec3f u = cross(s[0], s[1]);
    //���㹲��ʱ���ᵼ��u[2]Ϊ0����ʱ����(-1,1,1)
    if (std::abs(u[2]) > 1e-2)
        //��1-u-v��u��vȫΪ����0��������ʾ�����������ڲ�
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return Vec3f(-1, 1, 1);
}

int main(int argc, char** argv) {
    if (2 == argc)
    {
        model = new Model(argv[1]);
    }
    else
    {
        //BUG:Ϊɶ�����޷�ͨ�����·����ȡģ�ͣ�
        //model = new Model("obj/african_head.obj");
        model = new Model("C:/Users/DELL/Desktop/L_Renderer/obj/african_head/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    float* zbuffer = new float[width * height];
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    // ģ���߿�
    //for (int i = 0; i < model->nfaces(); i++)
    //{
    //    std::vector<int> face = model->face(i);        // ����һ��face�������ڱ���һ��face��������������
    //    for (int j = 0; j < 3; j++)
    //    {
    //        Vec3f v0 = model->vert(face[j]);
    //        Vec3f v1 = model->vert(face[(j + 1) % 3]);

    //        // ����ת����ģ������->��Ļ���ꡣ(-1��1)->(0,0)��(1��1)->(width,height)
    //        int x0 = (v0.x + 1.) * width / 2.;
    //        int y0 = (v0.y + 1.) * height / 2.;
    //        int x1 = (v1.x + 1.) * width / 2.;
    //        int y1 = (v1.y + 1.) * height / 2.;

    //        line(x0, y0, x1, y1, image, white);
    //    }
    //}

    // ������һ����ɫ
    //for (int i = 0; i < model->nfaces(); i++)
    //{
    //    std::vector<int> face = model->face(i);
    //    Vec2i screen_coords[3];
    //    for (int j = 0; j < 3; j++)
    //    {
    //        Vec3f v = model->vert(face[j]);
    //        screen_coords[j] = Vec2i((v.x + 1.0) * width / 2.0, (v.y + 1.0) * height / 2.0);
    //    }
    //    triangle(screen_coords[0], screen_coords[1] , screen_coords[2], image, TGAColor(255, 255, 255, 255));
    //}

    // lambert
    Vec3f light_dir = Vec3f(1, 1, 1);
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];

        for (int j = 0; j < 3; j++)
        {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x * 0.8 + 1.0) * width / 2.0, (v.y * 0.8 + 1.0) * height / 2);
            world_coords[j] = v;
        }

        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);        // ���
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0)
        {
            //��������ɫ
            //triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }

    image.flip_vertically();
    image.write_tga_file("3_output_lambert.tga");
    delete model;
    return 0;
}