#include <gtkmm.h>
#include "window.h"


int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create("com.game.snake");
    return app->make_window_and_run<GameWindow>(argc, argv);
}
