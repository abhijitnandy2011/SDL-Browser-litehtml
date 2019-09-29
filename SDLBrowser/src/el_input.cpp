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

/*
void el_input::get_content_size(litehtml::size& sz, int max_width)
{
    sz.width = 100;
    sz.height = 20;
}*/

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
    //litehtml::html_tag::draw(hdc, x, y, clip);
    litehtml::media_query_list::ptr mql = std::make_shared<litehtml::media_query_list>();
    litehtml::css_selector::ptr sel = std::make_shared<litehtml::css_selector>(mql);
    
    /*auto ancestor = litehtml::html_tag::find_ancestor(*sel);
    auto name = ancestor->get_tagName();
    auto pos1 = ancestor->get_position();*/
    
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(m_renderer, &r, &g, &b, &a);

    litehtml::position pos = litehtml::element::get_placement();   
    switch (m_inputType) {
    case HtmlInputType::TEXT:
        rectangleRGBA(m_renderer, x, y, x + m_width, y + 20, 255, 0, 0, 255);
        break;
    case HtmlInputType::BUTTON:
        rectangleRGBA(m_renderer, x, y, x + m_width, y + 20, 0, 0, 255, 255);
        break;
    }
        
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

int	el_input::render(int x, int y, int max_width, bool second_pass)
{
    std::cout << "render\n";
    if (max_width > 0)
        m_width = max_width;
    return max_width;
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

