//
// Created by 肖科 on 2024/8/15.
//

#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include <vector>
#include <iostream>
#include "square.h"


class Snake : public Square {
public:
    Snake() = default;
    Snake(int x, int y, int w);

    ~Snake() = default;

    void draw(const Cairo::RefPtr<Cairo::Context> &cr);

    // 更新蛇的位置
    bool move() ;

    // 蛇的碰撞检测
    bool collision(const Square &other) ;

    // 增加一个蛇身
    void increase(int x, int y, int w);

private:
    std::vector<Square> body;
};


#endif //SNAKE_SNAKE_H
