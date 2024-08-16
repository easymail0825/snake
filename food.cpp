//
// Created by 肖科 on 2024/8/15.
//

#include <random>
#include "food.h"


std::random_device rd;  // 不可预测的随机数生成器
std::mt19937 gen(rd()); // 以 rd() 作为种子的 Mersenne Twister 生成器
std::uniform_int_distribution<> dis(5, square_number - 5); // 定义均匀分布的范围


Food::Food(int w) : Square(0, 0, w, 1.0, 0.0, 0.0) {
    m_row = dis(gen);
    m_col = dis(gen);
}