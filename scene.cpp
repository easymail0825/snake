//
// Created by 肖科 on 2024/8/15.
//

#include "scene.h"

GameScene::GameScene(int w, int h)
        : m_width(w),
          m_height(h),
          m_square_width(ceil((w / square_number))),
          m_snake(0, 0, m_square_width),
          m_food(m_square_width) {
    set_draw_func(sigc::mem_fun(*this, &GameScene::on_draw));
}


void GameScene::set_direction(int d) {
    if (std::abs(m_snake.direction - d) != 2) {
        m_snake.direction = d;
    }
}

void GameScene::start() {
    if (!(m_status == GAME_INIT || m_status == GAME_OVER)) {
        return;
    }
    m_status = GAME_RUNNING;
    m_score = 0;
    m_level = 1;
    m_snake = Snake(0, 0, m_square_width);
    m_food = Food(m_square_width);
    m_timer.disconnect();
    auto m_slot = sigc::mem_fun(*this, &GameScene::update_game);
    m_timer = Glib::signal_timeout().connect(m_slot, m_level_rate.at(m_level - 1));
    queue_draw();
}

int GameScene::get_score() const {
    return m_score;
}

int GameScene::get_level() const {
    return m_level;
}

bool GameScene::update_game() {
    if (m_status == GAME_PAUSE) {
        return true;
    }

    if (!m_snake.move()) {
        m_status = GAME_STOP;
        queue_draw();
        return false;
    }

    if (m_snake.collision(m_food)) {
        m_snake.increase(0, 0, m_square_width);
        Food m_food_tmp = Food(m_square_width);
        // 要求新的食物不能与蛇身重叠
        while (m_snake.collision(m_food_tmp)) {
            m_food_tmp = Food(m_square_width);
        }
        m_food = m_food_tmp;
        m_score++;
        m_counter++;
        int old_level = m_level;
        m_level = 1 + m_score / 10;
        if (m_level > 10) {
            m_level = 10;
        }
        if (m_level > old_level) {
            m_timer.disconnect();
            auto m_slot = sigc::mem_fun(*this, &GameScene::update_game);
            m_timer = Glib::signal_timeout().connect(m_slot, m_level_rate.at(m_level - 1));
        }
    }
    queue_draw();
    return true;
}

void GameScene::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height) {
    if (width > height) {
        cr->translate(width - height, 0);
    } else if (height > width) {
        cr->translate(0, height - width);
    }

    // 绘制50*50的网线，线宽为1，颜色为灰色
    cr->set_line_width(0.1);
    cr->set_source_rgb(0.05, 0.05, 0.05);
    m_square_width = ceil(width / square_number);
    for (int i = 0; i < square_number; ++i) {
        cr->move_to(i * m_square_width, 0);
        cr->line_to(i * m_square_width, square_number * m_square_width);
        cr->stroke();
    }
    for (int i = 0; i < square_number; ++i) {
        cr->move_to(0, i * m_square_width);
        cr->line_to(square_number * m_square_width, i * m_square_width);
        cr->stroke();
    }

    if (m_status == GAME_RUNNING) {
        // 绘制蛇
        m_snake.draw(cr);
        // 绘制食物
        m_food.draw(cr);
    }

    if (m_status == GAME_STOP) {
        m_status = GAME_OVER;

        Pango::FontDescription font;

        font.set_family("Monospace");
        font.set_weight(Pango::Weight::BOLD);
        font.set_size(Pango::SCALE * 50);

        auto layout = create_pango_layout("Game Over");

        layout->set_font_description(font);

        int text_width = 100;
        int text_height = 100;

        //get the text dimensions (it updates the variables -- by reference)
        layout->get_pixel_size(text_width, text_height);

        // Position the text in the middle
        cr->move_to(ceil((width - text_width) / 2), ceil((height - text_height) / 2));
        layout->show_in_cairo_context(cr);
    }
}

void GameScene::pause() {
    if (m_status == GAME_RUNNING) {
        m_status = GAME_PAUSE;
    } else if (m_status == GAME_PAUSE) {
        m_status = GAME_RUNNING;
    }
}

GameStatus GameScene::get_status() const {
    return m_status;
}

void GameScene::set_status(GameStatus s) {
    m_status = s;
}
