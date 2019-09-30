// Impl

#include "el_input.h"

// std
#include <iostream>

// SDL
#include <SDL2_gfxPrimitives.h>

// App
#include "inputtypes.h"

el_input::el_input(
    const std::shared_ptr<litehtml::document>& doc,
    SDL_Renderer* renderer,
    SDLContainer* container,
    HtmlInputType inputType) :
    litehtml::html_tag(doc), 
    m_renderer(renderer),
    m_container(container),
    m_inputType(inputType)
{
    //m_display = litehtml::display_inline_block;
}

el_input::~el_input()
{

}

void el_input::get_content_size(litehtml::size& sz, int max_width)
{
    sz.width = max_width/*100*/;
    sz.height = 20;
}

litehtml::style_display el_input::get_display() const
{
    return litehtml::display_table_cell;
}

litehtml::element_position el_input::get_element_position(litehtml::css_offsets* offsets) const
{
    return litehtml::element_position_relative;
}

void el_input::draw(litehtml::uint_ptr hdc, int x, int y, const litehtml::position* clip)
{
    litehtml::position pos = m_pos;
    pos.x += x;
    pos.y += y;
    
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(m_renderer, &r, &g, &b, &a);

    switch (m_inputType) {
    case HtmlInputType::TEXT:
        rectangleRGBA(m_renderer, pos.x, pos.y, pos.x + pos.width, y + pos.height, 255, 0, 0, 255);
        break;
    case HtmlInputType::BUTTON:
        rectangleRGBA(m_renderer, pos.x, pos.y, pos.x + pos.width, y + pos.height, 0, 0, 255, 255);
        break;
    }
        
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

int el_input::line_height() const
{
    return height();
}

bool el_input::is_replaced() const
{
    return true;
}

int	el_input::render(int x, int y, int max_width, bool second_pass)
{
    using namespace litehtml;

    int parent_width = max_width;

    calc_outlines(parent_width);

    m_pos.move_to(x, y);

    document::ptr doc = get_document();

    litehtml::size sz;
    sz.width = max_width;
    sz.height = 20;
    //doc->container()->get_image_size(m_src.c_str(), 0, sz);

    m_pos.width = sz.width;
    m_pos.height = sz.height;

    if (m_css_height.is_predefined() && m_css_width.is_predefined())
    {
        m_pos.height = sz.height;
        m_pos.width = sz.width;

        // check for max-height
        if (!m_css_max_width.is_predefined())
        {
            int max_width = doc->cvt_units(m_css_max_width, m_font_size, parent_width);
            if (m_pos.width > max_width)
            {
                m_pos.width = max_width;
            }
            if (sz.width)
            {
                m_pos.height = (int)((float)m_pos.width * (float)sz.height / (float)sz.width);
            }
            else
            {
                m_pos.height = sz.height;
            }
        }

        // check for max-height
        if (!m_css_max_height.is_predefined())
        {
            int max_height = doc->cvt_units(m_css_max_height, m_font_size);
            if (m_pos.height > max_height)
            {
                m_pos.height = max_height;
            }
            if (sz.height)
            {
                m_pos.width = (int)(m_pos.height * (float)sz.width / (float)sz.height);
            }
            else
            {
                m_pos.width = sz.width;
            }
        }
    }
    else if (!m_css_height.is_predefined() && m_css_width.is_predefined())
    {
        if (!get_predefined_height(m_pos.height))
        {
            m_pos.height = (int)m_css_height.val();
        }

        // check for max-height
        if (!m_css_max_height.is_predefined())
        {
            int max_height = doc->cvt_units(m_css_max_height, m_font_size);
            if (m_pos.height > max_height)
            {
                m_pos.height = max_height;
            }
        }

        if (sz.height)
        {
            m_pos.width = (int)(m_pos.height * (float)sz.width / (float)sz.height);
        }
        else
        {
            m_pos.width = sz.width;
        }
    }
    else if (m_css_height.is_predefined() && !m_css_width.is_predefined())
    {
        m_pos.width = (int)m_css_width.calc_percent(parent_width);

        // check for max-width
        if (!m_css_max_width.is_predefined())
        {
            int max_width = doc->cvt_units(m_css_max_width, m_font_size, parent_width);
            if (m_pos.width > max_width)
            {
                m_pos.width = max_width;
            }
        }

        if (sz.width)
        {
            m_pos.height = (int)((float)m_pos.width * (float)sz.height / (float)sz.width);
        }
        else
        {
            m_pos.height = sz.height;
        }
    }
    else
    {
        m_pos.width = (int)m_css_width.calc_percent(parent_width);
        m_pos.height = 0;
        if (!get_predefined_height(m_pos.height))
        {
            m_pos.height = (int)m_css_height.val();
        }

        // check for max-height
        if (!m_css_max_height.is_predefined())
        {
            int max_height = doc->cvt_units(m_css_max_height, m_font_size);
            if (m_pos.height > max_height)
            {
                m_pos.height = max_height;
            }
        }

        // check for max-height
        if (!m_css_max_width.is_predefined())
        {
            int max_width = doc->cvt_units(m_css_max_width, m_font_size, parent_width);
            if (m_pos.width > max_width)
            {
                m_pos.width = max_width;
            }
        }
    }

    calc_auto_margins(parent_width);

    m_pos.x += content_margins_left();
    m_pos.y += content_margins_top();

    return m_pos.width + content_margins_left() + content_margins_right();
}

void el_input::parse_styles(bool is_reparse)
{
    litehtml::html_tag::parse_styles(is_reparse);

    //m_edit.setFont((cairo_font*)get_font(), get_color(_t("color"), true));
}

void el_input::on_click()
{
    /*if (!m_haveFocus)
    {
        SendMessage(m_hWndParent, WM_OMNIBOX_CLICKED, 0, 0);
    }*/
}

