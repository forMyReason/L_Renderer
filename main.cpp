#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "line.h"

Model* model = NULL;
const int width = 800;
const int height = 800;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int main(int argc, char** argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        //BUG:为啥这里无法通过相对路径读取模型？
        //model = new Model("obj/african_head.obj");
        model = new Model("C:/Users/DELL/Desktop/L_Renderer/obj/african_head/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);        // 创建一个face数组用于保存一个face的三个顶点坐标
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);

            // 坐标转换：模型坐标->屏幕坐标。(-1，1)->(0,0)，(1，1)->(width,height)
            int x0 = (v0.x + 1.) * width / 2.;
            int y0 = (v0.y + 1.) * height / 2.;
            int x1 = (v1.x + 1.) * width / 2.;
            int y1 = (v1.y + 1.) * height / 2.;

            line(x0, y0, x1, y1, image, white);
        }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output_1.tga");
    delete model;
    return 0;
}