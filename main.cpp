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

//绘制三角形(坐标1，坐标2，坐标3，tga指针，颜色)
//void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color)
//{
//    if (t0.y == t1.y && t0.y == t2.y) return;
//
//    //根据y的大小对坐标进行排序：t0<t1<t2
//    if (t0.y > t1.y) std::swap(t0, t1);
//    if (t0.y > t2.y) std::swap(t0, t2);
//    if (t1.y > t2.y) std::swap(t1, t2);
//
//    int total_height = t2.y - t0.y;
//
//    //以高度差作为循环控制变量，此时不需要考虑斜率，因为着色完后每行都会被填充
//    for (int i = 0; i < total_height; i++)
//    {
//        //根据t1将三角形分割为上下两部分
//        bool second_half = i > t1.y - t0.y || t1.y == t0.y;     // 上半部分
//        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
//        float alpha = (float)i / total_height;
//        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
//
//        //计算A,B两点的坐标
//        Vec2i A = t0 + (t2 - t0) * alpha;
//        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
//        if (A.x > B.x) std::swap(A, B);
//
//        //根据A,B和当前高度对tga着色
//        for (int j = A.x; j <= B.x; j++)
//        {
//            image.set(j, t0.y + i, color);
//        }
//    }
//}

// 质心坐标可以：判断点P是否在三角形内部，且可对三角形内的每个点都使用ABC三个点的坐标表示，方便后续插值
// TODO:质心坐标是不是就是重心坐标？
// TODO:线性代数都不会了
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];    //存储[AB,AC,PA]的x和y分量
    s[0] = Vec3f(B[0] - A[0], C[0] - A[0], A[0] - P[0]);
    s[1] = Vec3f(B[1] - A[1], C[1] - A[1], A[1] - P[1]);
    //[u,v,1]和[AB,AC,PA]对应的x和y向量都垂直，所以叉乘
    Vec3f u = cross(s[0], s[1]);
    //三点共线时，会导致u[2]为0，此时返回(-1,1,1)
    if (std::abs(u[2]) > 1e-2)
        //若1-u-v，u，v全为大于0的数，表示点在三角形内部
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return Vec3f(-1, 1, 1);
}

//绘制三角形(坐标数组，zbuffer指针，tga指针，颜色)
void triangle(Vec3f* pts, float* zbuffer, TGAImage& image, TGAColor color) {
    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
    //确定三角形的边框
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }
    Vec3f P;
    //遍历边框中的每一个点
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
            //计算质心
            if (P.x > 600 && P.y > 500)
            {
                P.x += 0.01;
            }
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            //质心坐标有一个负值，说明点在三角形外
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            P.z = 0;
            //计算zbuffer
            for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];
            if (zbuffer[int(P.x + P.y * width)] < P.z) {
                zbuffer[int(P.x + P.y * width)] = P.z;
                image.set(P.x, P.y, color);
            }
        }
    }
}

int main(int argc, char** argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else
    {
        //BUG:为啥这里无法通过相对路径读取模型？
        //model = new Model("obj/african_head.obj");
        model = new Model("C:/Users/79160/Desktop/L_Renderer/obj/african_head/african_head.obj");
        //model = new Model("C:/Users/DELL/Desktop/L_Renderer/obj/african_head/african_head.obj");
        
    }

    TGAImage image(width, height, TGAImage::RGB);
    float* zbuffer = new float[width * height];
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

     //模型线框
    //for (int i = 0; i < model->nfaces(); i++)
    //{
    //    std::vector<int> face = model->face(i);        // 创建一个face数组用于保存一个face的三个顶点坐标
    //    for (int j = 0; j < 3; j++)
    //    {
    //        Vec3f v0 = model->vert(face[j]);
    //        Vec3f v1 = model->vert(face[(j + 1) % 3]);

    //        // 坐标转换：模型坐标->屏幕坐标。(-1，1)->(0,0)，(1，1)->(width,height)
    //        int x0 = (v0.x + 1.) * width / 2.;
    //        int y0 = (v0.y + 1.) * height / 2.;
    //        int x1 = (v1.x + 1.) * width / 2.;
    //        int y1 = (v1.y + 1.) * height / 2.;

    //        line(x0, y0, x1, y1, image, white);
    //    }
    //}

    // 三角形一般着色
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
    Vec3f light_dir = Vec3f(0, 0, -1);
    for (int i = 0; i < model->nfaces(); i++)
    {
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        std::vector<int> face = model->face(i);

        for (int j = 0; j < 3; j++)
        {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
            world_coords[j] = v;
        }

        //世界坐标用于计算法向量
        Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0]));
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0)
        {
            //三角形着色
            //triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
            triangle(screen_coords, zbuffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }

    image.flip_vertically();
    image.write_tga_file("3_output_lambert.tga");
    delete model;
    return 0;
}