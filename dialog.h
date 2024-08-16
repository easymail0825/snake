//
// Created by 肖科 on 2024/8/16.
//

#ifndef SNAKE_DIALOG_H
#define SNAKE_DIALOG_H

#include <gtkmm.h>

class NameDialog : public Gtk::Window {
public:
    NameDialog();

    ~NameDialog() override;

    void buttons_clicked_connect(const sigc::slot<void(const Glib::ustring &)> &slot);

    Glib::ustring get_entry_name() const;

    void on_entry_changed();


protected:
    Gtk::Box m_box_h1;
    Gtk::Box m_box_h2;
    Gtk::Box m_box_v;
    Gtk::Label m_label;
    Gtk::Entry m_entry;
    Gtk::Button m_button_oK;
    Gtk::Button m_button_cancel;

};

#endif //SNAKE_DIALOG_H
