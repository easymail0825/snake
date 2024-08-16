//
// Created by 肖科 on 2024/8/15.
//

#ifndef SNAKE_SCORE_H
#define SNAKE_SCORE_H

#include <gtkmm/object.h>

class ScoreModelColumns : public Glib::Object {
public:
    unsigned int m_col_id;
    Glib::ustring m_col_name;
    int m_col_score;

    static Glib::RefPtr<ScoreModelColumns> create(unsigned int col_id,
                                                  const Glib::ustring &col_name, int col_score) {
        return Glib::make_refptr_for_instance<ScoreModelColumns>(
                new ScoreModelColumns(col_id, col_name, col_score));
    }

protected:
    ScoreModelColumns(unsigned int col_id, const Glib::ustring &col_name,
                      int col_score)
            : m_col_id(col_id), m_col_name(col_name),
              m_col_score(col_score) {}
};

class Score {
public:
    Score() = default;

    Score( std::string name, int score) : m_name(std::move(name)), m_score(score) {}

    ~Score() = default;

    std::string m_name;
    int m_score;
};

#endif //SNAKE_SCORE_H
