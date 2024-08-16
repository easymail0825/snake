//
// Created by 肖科 on 2024/8/15.
//

#include <fstream>
#include "window.h"

GameWindow::GameWindow(int w, int h)
        : m_gameScene(w, h),
          m_paned(Gtk::Orientation::HORIZONTAL),
          m_box_right(Gtk::Orientation::VERTICAL, 10),
          m_button_start("Start"),
          m_button_pause("Pause"),
          m_label_score("Score: 0"),
          m_button_save("Save"),
          m_frame_top10(),
          m_scrolled_window(),
          m_nameDialog(),
          m_data_file("data.txt") {
    set_title("Greedy Snake Game");
    set_default_size(w, h);
    set_resizable(false);

    m_paned.set_margin(10);
    m_paned.set_position(600);
    m_paned.set_resize_start_child(false);
    m_paned.set_resize_end_child(false);

    m_paned.set_start_child(m_gameScene);

    m_box_right.set_margin(10);

    // start button
    m_button_start.set_margin(10);
    m_button_start.signal_clicked().connect(
            sigc::mem_fun(*this, &GameWindow::on_start_clicked));
    m_box_right.append(m_button_start);

    // pause button
    m_button_pause.set_margin(10);
    m_button_pause.set_visible(m_gameScene.get_status() != GAME_RUNNING);
    m_button_pause.signal_clicked().connect(
            sigc::mem_fun(*this, &GameWindow::on_pause_clicked));
    m_box_right.append(m_button_pause);
    // score label

    m_label_score.set_margin(10);
    m_box_right.append(m_label_score);
    auto m_slot = sigc::mem_fun(*this, &GameWindow::update_score);
    Glib::signal_timeout().connect(m_slot, 200);

    // save button
    m_button_save.set_visible(false);
    m_button_save.set_margin(10);
    m_button_save.signal_clicked().connect(
            sigc::mem_fun(*this, &GameWindow::on_save_clicked));
    m_box_right.append(m_button_save);
    m_nameDialog.buttons_clicked_connect(
            sigc::mem_fun(*this, &GameWindow::on_dialog_response));

    // top 10
    m_frame_top10.set_label("Top 10");
    m_frame_top10.set_label_align(Gtk::Align::CENTER);
    m_frame_top10.set_expand(true);
    m_frame_top10.set_hexpand(true);

    // Create the List model:
    load_data();

    m_ListStore = Gio::ListStore<ScoreModelColumns>::create();
    for (int i = 0; i < std::min(int(m_score_list.size()), 10); i++) {
        m_ListStore->append(ScoreModelColumns::create(i + 1, m_score_list.at(i).m_name, m_score_list.at(i).m_score));
    }

    // Set list model and selection model.
    auto selection_model = Gtk::SingleSelection::create(m_ListStore);
    selection_model->set_autoselect(false);
    selection_model->set_can_unselect(true);
    m_column_view_top10.set_model(selection_model);

    // Make the columns reorderable.
    // This is not necessary, but it's nice to show the feature.
    m_column_view_top10.set_reorderable(true);

    // Add the ColumnView's columns:

    // Id column
    auto factory = Gtk::SignalListItemFactory::create();
    factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this,
                                                             &GameWindow::on_setup_score_label), Gtk::Align::END));
    factory->signal_bind().connect(
            sigc::mem_fun(*this, &GameWindow::on_bind_score_id));
    auto column = Gtk::ColumnViewColumn::create("NO.", factory);
    m_column_view_top10.append_column(column);

    // Name column
    factory = Gtk::SignalListItemFactory::create();
    factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this,
                                                             &GameWindow::on_setup_score_label), Gtk::Align::START));
    factory->signal_bind().connect(
            sigc::mem_fun(*this, &GameWindow::on_bind_score_name));
    column = Gtk::ColumnViewColumn::create("Name", factory);
    m_column_view_top10.append_column(column);

    // Score column
    for (auto d: m_score_list) {
        std::cout << d.m_name << " " << d.m_score << std::endl;
    }
    factory = Gtk::SignalListItemFactory::create();
    factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this,
                                                             &GameWindow::on_setup_score_label), Gtk::Align::END));
    factory->signal_bind().connect(
            sigc::mem_fun(*this, &GameWindow::on_bind_score_number));
    column = Gtk::ColumnViewColumn::create("Score", factory);
    m_column_view_top10.append_column(column);

    m_scrolled_window.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_scrolled_window.set_expand();
    m_scrolled_window.set_child(m_column_view_top10);
    m_frame_top10.set_child(m_scrolled_window);
    m_box_right.append(m_frame_top10);

    m_paned.set_end_child(m_box_right);
    set_child(m_paned);

    auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(
            sigc::mem_fun(*this, &GameWindow::on_key_pressed), false);
    add_controller(controller);
}

bool GameWindow::on_key_pressed(guint key_val, guint key_code, Gdk::ModifierType state) {
    if (key_val == GDK_KEY_Left) {
        m_gameScene.set_direction(0);
    } else if (key_val == GDK_KEY_Up) {
        m_gameScene.set_direction(1);
    } else if (key_val == GDK_KEY_Right) {
        m_gameScene.set_direction(2);
    } else if (key_val == GDK_KEY_Down) {
        m_gameScene.set_direction(3);
    } else if (key_val == GDK_KEY_p || key_val == GDK_KEY_P) {
        on_pause_clicked();
    }
    return true;
}

void GameWindow::on_start_clicked() {
    m_gameScene.start();
}

void GameWindow::on_save_clicked() {
    m_nameDialog.show();
}

bool GameWindow::update_score() {
    m_label_score.set_text("Score: " + std::to_string(m_gameScene.get_score()) + ", level: " +
                           std::to_string(m_gameScene.get_level()));
    GameStatus s = m_gameScene.get_status();
    m_button_save.set_visible(s == GAME_OVER);
    return true;
}


void GameWindow::on_setup_score_label(
        const Glib::RefPtr<Gtk::ListItem> &list_item, Gtk::Align halign) {
    list_item->set_child(*Gtk::make_managed<Gtk::Label>("", halign));
}


void GameWindow::on_bind_score_id(const Glib::RefPtr<Gtk::ListItem> &list_item) {
    auto col = std::dynamic_pointer_cast<ScoreModelColumns>(list_item->get_item());
    if (!col)
        return;
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());
    if (!label)
        return;
    label->set_text(Glib::ustring::format(col->m_col_id));
}

void GameWindow::on_bind_score_name(const Glib::RefPtr<Gtk::ListItem> &list_item) {
    auto col = std::dynamic_pointer_cast<ScoreModelColumns>(list_item->get_item());
    if (!col)
        return;
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());
    if (!label)
        return;
    label->set_text(col->m_col_name);
}

void GameWindow::on_bind_score_number(const Glib::RefPtr<Gtk::ListItem> &list_item) {
    auto col = std::dynamic_pointer_cast<ScoreModelColumns>(list_item->get_item());
    if (!col)
        return;
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());
    if (!label)
        return;
    // 10 digits, using leading zeroes.
    label->set_text(Glib::ustring::sprintf("%d", col->m_col_score));
}

void GameWindow::on_pause_clicked() {
    GameStatus s = m_gameScene.get_status();
    if (s == GAME_RUNNING) {
        m_button_pause.set_label("Resume");
        m_gameScene.set_status(GAME_PAUSE);
    } else if (s == GAME_PAUSE) {
        m_gameScene.set_status(GAME_RUNNING);
        m_button_pause.set_label("Pause");
    }
}

void GameWindow::load_data() {
    m_score_list.clear();

    // 首先检查文件是否存在
    std::ifstream infile(m_data_file);
    if (!infile.is_open()) {
        // 文件不存在，创建它
        std::ofstream outfile(m_data_file);
        if (!outfile.is_open()) {
            std::cerr << "Failed to create file: " << m_data_file << std::endl;
            return;
        }
        // 文件创建成功，可以根据需要写入数据
        outfile.close();
    }
    infile.close();

    // 现在文件存在，以只读方式打开
    infile.open(m_data_file);
    std::string line;
    if (infile.is_open()) {
        while (getline(infile, line)) {
            std::stringstream ss(line);
            std::string item;
            std::vector<std::string> row;

            // 使用逗号作为分隔符读取每个字段
            while (getline(ss, item, ',')) {
                row.push_back(item);
            }

            if (row.size() == 2) {
                std::string name = row[0];
                int score = std::stoi(row[1]);
                m_score_list.emplace_back(name, score);
            }
        }
        infile.close();
    } else {
        std::cerr << "Failed to open file for reading: " << m_data_file << std::endl;
    }

    // 倒序排序
    std::sort(m_score_list.begin(), m_score_list.end(), [](const Score &a, const Score &b) {
        return a.m_score > b.m_score;
    });
}

void GameWindow::on_dialog_response(const Glib::ustring &response) {
    m_nameDialog.hide();
    if (response == "OK") {
        // 获取用户输入的名称
        Glib::ustring player_name = m_nameDialog.get_entry_name();
        // 写入到data.txt文件中的末尾
        std::ofstream outfile(m_data_file, std::ios::app);
        if (outfile.is_open()) {
            outfile << player_name << "," << m_gameScene.get_score() << std::endl;
            m_score_list.emplace_back(player_name, m_gameScene.get_score());
        }
        outfile.close();
        // 重新加载数据
        load_data();

        // 刷新m_ListStore模型的数据
        auto t = Gio::ListStore<ScoreModelColumns>::create();
        for (int i = 0; i < std::min(int(m_score_list.size()), 10); i++) {
            t->append(ScoreModelColumns::create(i + 1, m_score_list.at(i).m_name, m_score_list.at(i).m_score));
        }
        m_ListStore = std::move(t);
        auto selection_model = Gtk::SingleSelection::create(m_ListStore);
        selection_model->set_autoselect(false);
        selection_model->set_can_unselect(true);
        m_column_view_top10.set_model(selection_model);
    } else {
        std::cout << response << " button clicked" << std::endl;
    }
    this->present();

}

