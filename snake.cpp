//
// Created by 肖科 on 2024/8/15.
//

#include "snake.h"

Snake::Snake(int x, int y, int w) : Square(x, y, w, 0.5, 1.0, 0.5) {
    increase(2, 0, w);
    increase(1, 0, w);
    increase(0, 0, w);
};

bool Snake::move() {
    for (size_t i = body.size() - 1; i > 0; --i) {
        body[i] = body[i - 1];
    }
    if (direction == 0)
        body[0].move_by(-1, 0);
    else if (direction == 1)
        body[0].move_by(0, -1);
    else if (direction == 2)
        body[0].move_by(1, 0);
    else if (direction == 3)
        body[0].move_by(0, 1);

    // 检测蛇是否撞到墙
    for (auto &o: body) {
        if (o.m_row < 0 || o.m_col < 0 || o.m_row >= square_number - 1 || o.m_col >= square_number - 1) {
            return false;
        }
    }

    // 检测蛇是否撞到自己
    for (size_t i = 1; i < body.size(); ++i) {
        if (body[0].collide(body[i])) {
            return false;
        }
    }
    return true;
}


void Snake::draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    for (auto &o: body) {
        o.draw(cr);
    }
}

bool Snake::collision(const Square &o) {
    return body[0].collide(o);
}

void Snake::increase(int x, int y, int w) {
    body.emplace_back(x, y, w, 0.5, 1.0, 0.5);
}