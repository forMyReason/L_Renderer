#ifndef _LINE_H_
#define _LINE_H_

#include <cmath>
#include "tgaimage.h"

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
    for (float t = 0.0f; t < 1.0f; t += 0.01f)
    {
        int x = static_cast<int>(x0 + (x1 - x0) * t);
        int y = static_cast<int>(y0 + (y1 - y0) * t);
        image.set(x, y, color);
    }
}

// 遍历x，插值y坐标
void line2(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
    float divisor = 1.0 / static_cast<float>(x1 - x0);
    for (int x = x0; x < x1; x++)
    {
        float t = (x - x0) * divisor;
        int y = static_cast<int>(y0 * (1.0f - t) + y1 * t);
        image.set(x, y, color);
    }
}

// 遍历y，插值x坐标
void line2_transpose(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
    for (int y = y0; y < y1; y++)
    {
        float t = (y - y0) / static_cast<float> (y1 - y0);
        int x = static_cast<int>(x0 * (1.0f - t) + x1 * t);
        image.set(x, y, color);
    }
}

void line3(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    if (dx >= dy)
    {
        if (x0 < x1)
        {
            line2(x0, y0, x1, y1, image, color);
        }
        else
        {
            line2(x1, y1, x0, y0, image, color);
        }
    }
    else
    {
        if (y0 < y1)
        {
            line2_transpose(x0, y0, x1, y1, image, color);
        }
        else
        {
            line2_transpose(x1, y1, x0, y0, image, color);
        }
    }
}

// 原作者的改动方式是这样的（与原作者代码在形式上有区别，但思路一致）
void line_x_loop(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    float derror = dy / static_cast<float>(dx);
    float error = 0.0f;

    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        error += derror;
        if (error > .5) {
            y += (y1 > y0 ? 1 : -1);
            error -= 1.;
        }
        image.set(x, y, color);
    }
}

void line_y_loop(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    float derror = dx / static_cast<float>(dy);
    float error = 0.0f;

    int x = x0;
    for (int y = y0; y <= y1; y++)
    {
        error += derror;
        if (error > .5) {
            x += (x1 > x0 ? 1 : -1);
            error -= 1.;
        }
        image.set(x, y, color);
    }
}

void line4(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    if (dx >= dy)
    {
        if (x0 < x1)
        {
            line2(x0, y0, x1, y1, image, color);
        }
        else
        {
            line2(x1, y1, x0, y0, image, color);
        }
    }
    else
    {
        if (y0 < y1)
        {
            line2_transpose(x0, y0, x1, y1, image, color);
        }
        else
        {
            line2_transpose(x1, y1, x0, y0, image, color);
        }
    }
}

// Bresenham画线算法的完整代码：
void Bresenham(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;//如果dy大于dx，那么就翻转x、y坐标
    }
    if (x0 > x1) {
        std::swap(x0, x1);//如果逆序，那么就交换起点终点坐标
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = std::abs(y1 - y0);

    int d = 2 * dy - dx;
    int incrE = 2 * dy;
    int incrNE = 2 * dy - 2 * dx;

    int x = x0, y = y0;

    if (steep) {
        image.set(y, x, color);
    }
    else {
        image.set(x, y, color);
    }

    for (int x = x0 + 1; x <= x1; x++)
    {
        if (d < 0)
        {
            d += incrE;
        }
        else
        {
            if (y1 > y0)
                y++;
            else
                y--;
            d += incrNE;
        }

        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
    }
}

// 将上述Breseham算法进行修改，（即原作者的版本），这也是我们画线算法的最终版本：
void line5(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

// TODO:需要整理划线算法迭代过程，具体看文章 + 理解代码

#endif // !_LINE_H_

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