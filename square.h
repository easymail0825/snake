//
// Created by 肖科 on 2024/8/15.
//

#ifndef SNAKE_SQUARE_H
#define SNAKE_SQUARE_H

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

#define square_number 30


// 矩形，用行号和列号来定位
class Square {
public:
    int m_row;
    int m_col;
    int m_width;

    double m_color_red;
    double m_color_green;
    double m_color_blue;
    int direction = 2; // 0:left, 1:up, 2:right, 3:down

    Square() = default;

    Square(int row, int col, int width, double red, double green, double blue)
            : m_row(row), m_col(col), m_width(width), m_color_red(red), m_color_green(green),
              m_color_blue(blue) {}

    ~Square() {}

    virtual void draw(const Cairo::RefPtr<Cairo::Context> &cr);

    void move_by(int x, int y) {
        m_row += x;
        m_col += y;
    }

    // 碰撞检测
    bool collide(const Square &o) {
        if (m_row == o.m_row && m_col == o.m_col)
            return true;
        else
            return false;
    }

};

#endif //SNAKE_SQUARE_H
