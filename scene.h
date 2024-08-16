//
// Created by 肖科 on 2024/8/15.
//

#ifndef SNAKE_SCENE_H
#define SNAKE_SCENE_H

#include <gtkmm.h>
#include "snake.h"
#include "food.h"

enum GameStatus {
    GAME_INIT,
    GAME_RUNNING,
    GAME_PAUSE,
    GAME_STOP,
    GAME_OVER
};


class GameScene : public Gtk::DrawingArea {
public:
    GameScene();

    GameScene(int w, int h);

    ~GameScene() {}

    void set_direction(int d);

    void start();

    int get_score() const;


    int get_level() const;

    void pause();

    GameStatus get_status() const;

    void set_status(GameStatus s);

private:
    int m_width;
    int m_height;
    int m_square_width;

protected:
    bool update_game();

    void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);

    sigc::connection m_timer;
    GameStatus m_status = GAME_INIT; //0: 未开始, 1: 游戏中, 2: 暂停中, 3: 结束
    Snake m_snake;
    Food m_food;
    int m_score = 0;
    int m_level = 1;
    int m_counter = 0;
    std::array<int, 10> m_level_rate = {200, 180, 160, 150, 125, 120, 100, 80, 75, 50};
};

#endif //SNAKE_SCENE_H
