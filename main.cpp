#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "line.h"

//const TGAColor white = TGAColor(255, 255, 255, 255);
//const TGAColor red = TGAColor(255, 0, 0, 255);
//Model* model = NULL;
//const int width = 800;
//const int height = 800;


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int main(int argc, char** argv)
{
	TGAImage image(100 , 100 , TGAImage::RGB);
	line3(13, 20, 80, 40, image, white);
    line3(20, 13, 40, 80, image, red);
    line3(80, 40, 13, 20, image, red);
	//这条红色的线跟第一条白色的线只是交换了起点和终点像素坐标，即以不同的方向绘制同一根直线
	image.set(52 , 41 , white);
	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

//void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
//    bool steep = false;
//    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
//        std::swap(x0, y0);
//        std::swap(x1, y1);
//        steep = true;
//    }
//    if (x0 > x1) {
//        std::swap(x0, x1);
//        std::swap(y0, y1);
//    }
//
//    for (int x = x0; x <= x1; x++) {
//        float t = (x - x0) / (float)(x1 - x0);
//        int y = y0 * (1. - t) + y1 * t;
//        if (steep) {
//            image.set(y, x, color);
//        }
//        else {
//            image.set(x, y, color);
//        }
//    }
//}
//
//int main(int argc, char** argv) {
//    if (2 == argc) {
//        model = new Model(argv[1]);
//    }
//    else {
//        model = new Model("obj/african_head.obj");
//    }
//
//    TGAImage image(width, height, TGAImage::RGB);
//    for (int i = 0; i < model->nfaces(); i++) {
//        std::vector<int> face = model->face(i);
//        for (int j = 0; j < 3; j++) {
//            Vec3f v0 = model->vert(face[j]);
//            Vec3f v1 = model->vert(face[(j + 1) % 3]);
//            int x0 = (v0.x + 1.) * width / 2.;
//            int y0 = (v0.y + 1.) * height / 2.;
//            int x1 = (v1.x + 1.) * width / 2.;
//            int y1 = (v1.y + 1.) * height / 2.;
//            line(x0, y0, x1, y1, image, white);
//        }
//    }
//
//    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
//    image.write_tga_file("output.tga");
//    delete model;
//    return 0;
//}