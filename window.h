//
// Created by 肖科 on 2024/8/15.
//

#ifndef SNAKE_WINDOW_H
#define SNAKE_WINDOW_H

#include <gtkmm.h>
#include "scene.h"
#include "score.h"
#include "dialog.h"

class GameWindow : public Gtk::Window {
public:
    GameWindow() : GameWindow(800, 600) {};

    GameWindow(int width, int height);

    ~GameWindow() override = default;

protected:
    Gtk::Paned m_paned;
    GameScene m_gameScene;
    Gtk::Box m_box_right;
    Gtk::Button m_button_start;
    Gtk::Label m_label_score;
    sigc::connection m_timer;
    Gtk::Button m_button_save;
    Gtk::Button m_button_pause;
    Gtk::Frame m_frame_top10;
    Gtk::ScrolledWindow m_scrolled_window;
    Gtk::ColumnView m_column_view_top10;
    Glib::RefPtr<Gio::ListStore<ScoreModelColumns>> m_ListStore;
    NameDialog m_nameDialog;

    bool on_key_pressed(guint key_val, guint key_code, Gdk::ModifierType state);

    void on_start_clicked();

    void on_pause_clicked();

    void on_save_clicked();

    bool update_score();

    void on_bind_score_id(const Glib::RefPtr<Gtk::ListItem> &list_item);

    void on_setup_score_label(const Glib::RefPtr<Gtk::ListItem> &list_item, Gtk::Align halign);

    void on_bind_score_name(const Glib::RefPtr<Gtk::ListItem> &list_item);

    void on_bind_score_number(const Glib::RefPtr<Gtk::ListItem> &list_item);

    std::vector<Score> m_score_list;

    void load_data();

    void on_dialog_response(const Glib::ustring& response);


private:
    std::string m_data_file;
};


#endif //SNAKE_WINDOW_H
