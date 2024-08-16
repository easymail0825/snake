//
// Created by 肖科 on 2024/8/16.
//

#include "dialog.h"

NameDialog::NameDialog()
        : m_label("Name"),
          m_box_h1(Gtk::Orientation::HORIZONTAL, 10),
          m_box_h2(Gtk::Orientation::HORIZONTAL, 10),
          m_box_v(Gtk::Orientation::VERTICAL, 10),
          m_button_oK("OK", true),
          m_button_cancel("Cancel", true) {
    set_destroy_with_parent(true);

    set_title("Please enter your name");
    set_child(m_box_v);
    m_label.set_margin_top(20);
    m_label.set_margin_bottom(20);
    m_label.set_margin_start(20);
    m_box_h1.append(m_label);
    m_entry.set_margin_top(20);
    m_entry.set_margin_bottom(20);
    m_entry.set_margin_end(20);
    m_box_h1.append(m_entry);
    m_box_v.append(m_box_h1);
    m_box_h2.append(m_button_oK);
    m_box_h2.append(m_button_cancel);
    m_button_oK.set_margin_start(20);
    m_button_oK.set_margin_end(10);
    m_button_oK.set_expand(true);
    m_button_oK.set_sensitive(false);
    m_entry.signal_changed().connect(sigc::mem_fun(*this, &NameDialog::on_entry_changed));

    m_button_cancel.set_margin_start(10);
    m_button_cancel.set_margin_end(20);
    m_box_h2.set_margin_bottom(20);
    m_button_cancel.set_expand(true);
    m_box_v.append(m_box_h2);
}

NameDialog::~NameDialog() {
}

void NameDialog::buttons_clicked_connect(const sigc::slot<void(const Glib::ustring &)> &slot) {
    m_button_oK.signal_clicked().connect(sigc::bind(slot, "OK"));
    m_button_cancel.signal_clicked().connect(sigc::bind(slot, "Cancel"));
}

Glib::ustring NameDialog::get_entry_name() const {
    return m_entry.get_text();
}

void NameDialog::on_entry_changed() {
    // 检查Entry中的文本长度
    std::string text = m_entry.get_text();
    if (text.length() < 2) {
        // 如果文本长度少于5个字符，则禁用提交按钮
        m_button_oK.set_sensitive(false);
    } else {
        // 否则，启用提交按钮
        m_button_oK.set_sensitive(true);
    }
}
