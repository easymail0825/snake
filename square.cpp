//
// Created by 肖科 on 2024/8/15.
//

#include "square.h"


void Square::draw(const Cairo::RefPtr <Cairo::Context> &cr)  {
    cr->set_source_rgb(m_color_red, m_color_green, m_color_blue);
    cr->move_to(m_row * m_width, m_col * m_width);
    cr->rectangle(m_row * m_width, m_col * m_width, m_width, m_width);
    cr->fill();
}
