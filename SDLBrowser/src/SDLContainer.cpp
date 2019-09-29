// Impl

#include "SDLContainer.h"

// std
#include <math.h>
#include <iostream>

// SDL
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

// App
#include "el_input.h"
#include "inputtypes.h"


using namespace litehtml;

SDLContainer::SDLContainer(litehtml::context* html_context, SDL_Renderer* renderer)
{
  // std::cout << "#ctor\n";
  m_renderer = renderer;
  TTF_Init();
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
}

SDLContainer::~SDLContainer(void)
{
}

litehtml::uint_ptr SDLContainer::create_font( const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm )
{
    // std::cout << "[create_font] faceName: " << faceName
    //     << ", size: " << size
    //     << ", weight: " << weight
    //     << ", italic: " << italic
    //     << ", decoration: " << decoration << std::endl;

    // TODO load specific fonts
    // string_vector fonts;
    // split_string(faceName, fonts, _t(","));

    std::string fontPath = "./fonts/";

    std::string fontName = "Roboto-Medium";
    std::string key = "Roboto-Medium";

    if(weight >= 700) {
        fontName = "Roboto-Bold";
        key = "Roboto-Bold";
    }

    key += "-" + std::to_string(size) + "px";

    // std::cout << key << "\n" << fontName << "\n";

    TTF_Font* font;

    if (m_fonts[key]) {
        font = m_fonts[key];
    } else {
        // fontPath.c_str() + fnt_name
        font = TTF_OpenFont((fontName + ".ttf").c_str(), size);

        // std::cout << "[create_font] OpenFont\n";

        m_fonts[key] = font;
    }

    //If there was an error in loading the font
    if(font == nullptr) {
        std::cout << "[create_font] can't load ttf: " << fontName << std::endl;
        std::cout << "TTF_OpenFont:" << TTF_GetError();
        return 0;
    }

    int ttfStyle = TTF_STYLE_NORMAL;

    if(italic == fontStyleItalic) {
        ttfStyle = ttfStyle | TTF_STYLE_ITALIC;
    }

    if((decoration & font_decoration_linethrough) != 0) {
        ttfStyle = ttfStyle | TTF_STYLE_STRIKETHROUGH;
    }

    if((decoration & font_decoration_underline) != 0) {
        ttfStyle = ttfStyle | TTF_STYLE_UNDERLINE;
    }

    if(weight >= 700) {
        ttfStyle = ttfStyle | TTF_STYLE_BOLD;
    }

    TTF_SetFontStyle(font, ttfStyle);

    int iWidth = 0, iHeight = 0;
    TTF_SizeText(font, "x", &iWidth, &iHeight);

    if (fm) {
        // fixme cache this
        fm->ascent    = TTF_FontAscent(font);
        fm->descent   = TTF_FontDescent(font);
        fm->height    = TTF_FontHeight(font);
        fm->x_height  = iWidth;
        fm->draw_spaces = italic == fontStyleItalic || decoration;

        // std::cout << "[create_font] ascent: " << fm->ascent
        //     << ", descent: " << fm->descent
        //     << ", height: " << fm->height
        //     << ", x_height: " << fm->x_height
        //     << ", draw_spaces: " << fm->draw_spaces  << std::endl;
    }

    return (uint_ptr) font;
}

void SDLContainer::delete_font( litehtml::uint_ptr hFont )
{
    // todo fix segfault
    return;

    TTF_Font* font = (TTF_Font*)hFont;

    if(font) {
        TTF_CloseFont( font );
    }
}

int SDLContainer::text_width( const litehtml::tchar_t* text, litehtml::uint_ptr hFont )
{
    TTF_Font* font = (TTF_Font*)hFont;
    
    if(!font) {
        // std::cout << "[text_width](" << text << ") font: null" << std::endl;
        return 0;
    }

    int iWidth = 0, iHeight = 0;

    auto len = wcslen(text) + 1;
    std::shared_ptr<char> pText(new char[len], std::default_delete<char[]>());
    std::wcstombs(pText.get(), text, len);

    TTF_SizeText(font, pText.get(), &iWidth, &iHeight);
    return iWidth;
}

void SDLContainer::draw_text( litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos )
{
    SDL_Color sdlcolor={color.red, color.green, color.blue, color.alpha};
    SDL_Surface *info;
    TTF_Font* font = (TTF_Font*)hFont;

    auto len = wcslen(text) + 1;
    std::shared_ptr<char> pText(new char[len], std::default_delete<char[]>());
    std::wcstombs(pText.get(), text, len);

    if(!(info=TTF_RenderUTF8_Blended(font, pText.get(), sdlcolor))) {
        //handle error here, perhaps print TTF_GetError at least
    } else {
        // fixme - use baseline correctly

        SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, info);
        SDL_Rect src = { 0, 0, info->w, info->h };
        SDL_Rect dst = { pos.x, pos.y - (int) (pos.height * 0.5), info->w, info->h };
        SDL_RenderCopy(m_renderer, texture, &src, &dst);
        SDL_DestroyTexture(texture);
    }

}

int SDLContainer::pt_to_px( int pt )
{
    return pt; // (int) round(pt * 125.0 / 72.0);
}

int SDLContainer::get_default_font_size() const
{
    return 16;
}

void SDLContainer::draw_list_marker( litehtml::uint_ptr hdc, const litehtml::list_marker& marker )
{
    //std::cout << "#draw_list_marker\n";
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(m_renderer, &r, &g, &b, &a);

    if (!marker.image.empty())
    {
        /*std::wstring url;
        t_make_url(marker.image.c_str(), marker.baseurl, url);

        lock_images_cache();
        images_map::iterator img_i = m_images.find(url.c_str());
        if (img_i != m_images.end())
        {
            if (img_i->second)
            {
                draw_txdib((cairo_t*)hdc, img_i->second.get(), marker.pos.x, marker.pos.y, marker.pos.width, marker.pos.height);
            }
        }
        unlock_images_cache();*/
    }
    else
    {
        switch (marker.marker_type)
        {
        case litehtml::list_style_type_circle:
            circleRGBA(m_renderer, marker.pos.x, marker.pos.y, marker.pos.width, marker.color.red, marker.color.green, marker.color.blue, marker.color.alpha);
            break;
        case litehtml::list_style_type_disc:
            filledCircleRGBA(m_renderer, marker.pos.x, marker.pos.y, marker.pos.width, marker.color.red, marker.color.green, marker.color.blue, marker.color.alpha);
            break;
        case litehtml::list_style_type_square:
            SDL_Rect fillRect = { marker.pos.x, marker.pos.y, marker.pos.width, marker.pos.height };
            SDL_SetRenderDrawColor(m_renderer, marker.color.red, marker.color.green, marker.color.blue, marker.color.alpha);
            SDL_RenderFillRect(m_renderer, &fillRect);
            break;
        }
    }

    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void SDLContainer::load_image( const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready )
{
    std::cout << "#loadImage " << src << "\n";
}

void SDLContainer::get_image_size( const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz )
{
    auto len = wcslen(src) + 1;
    std::shared_ptr<char> pText(new char[len], std::default_delete<char[]>());
    std::wcstombs(pText.get(), src, len);
    auto image = IMG_Load(pText.get());
    sz.width = image->w;
    sz.height = image->h;

    //std::cout << "width:" << image->w << " height" << image->h << "\n";

    SDL_FreeSurface(image);
}

void SDLContainer::draw_background( litehtml::uint_ptr hdc, const litehtml::background_paint& bg )
{
    if (bg.image.empty()) {
      SDL_Rect fillRect = { bg.clip_box.x, bg.clip_box.y, bg.clip_box.width, bg.clip_box.height };
      SDL_SetRenderDrawColor(m_renderer, bg.color.red, bg.color.green, bg.color.blue, bg.color.alpha);
      SDL_RenderFillRect(m_renderer, &fillRect);
    } 
    else {
      // Drawing image
      SDL_Rect fillRect = { bg.clip_box.x, bg.clip_box.y, bg.image_size.width, bg.image_size.height };

      auto len = wcslen(bg.image.c_str()) + 1;
      std::shared_ptr<char> pText(new char[len], std::default_delete<char[]>());
      std::wcstombs(pText.get(), bg.image.c_str(), len);
      auto image = IMG_Load(pText.get());
      auto texture = SDL_CreateTextureFromSurface(m_renderer, image);

      SDL_Rect src = { 0, 0, image->w, image->h };
      SDL_RenderCopy(m_renderer, texture, &src, &fillRect);

      std::cout << "Drawing... " << bg.image.c_str() << " " << image->w << "," << image->h << "\n";

      std::cout << fillRect.x
         << "," << fillRect.y
         << "," << fillRect.w
         << "," << fillRect.h
        << "\n";

      SDL_DestroyTexture(texture);
      SDL_FreeSurface(image);
    }
}

void SDLContainer::make_url(const litehtml::tchar_t* url,  const litehtml::tchar_t* basepath, litehtml::tstring& out)
{
}

void SDLContainer::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root)
{
    if (borders.top.width != 0 && borders.top.style > litehtml::border_style_hidden) {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(m_renderer, &r, &g, &b, &a);
        SDL_Rect fillRect = { draw_pos.x, draw_pos.y, draw_pos.width, draw_pos.height };
        SDL_SetRenderDrawColor(m_renderer, borders.top.color.red, borders.top.color.green, borders.top.color.blue, borders.top.color.alpha);
        SDL_RenderDrawRect(m_renderer, &fillRect);       
        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    }
}

void SDLContainer::transform_text(litehtml::tstring& text, litehtml::text_transform tt)
{
}

void SDLContainer::set_clip( const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y )
{
}

void SDLContainer::del_clip()
{
}

void SDLContainer::clear_images()
{
}

void SDLContainer::get_client_rect(litehtml::position& client) const 
{
    client.width = 640;
    client.height = 480;
}

void SDLContainer::on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el) 
{
    std::cout << "Click\n";
}

void SDLContainer::set_cursor(const litehtml::tchar_t* cursor) 
{

}

void SDLContainer::import_css(litehtml::tstring& text, const litehtml::tstring& url, litehtml::tstring& baseurl) 
{

}

void SDLContainer::set_caption(const litehtml::tchar_t* caption) 
{

}

void SDLContainer::set_base_url(const litehtml::tchar_t* base_url) 
{

}


const litehtml::tchar_t* SDLContainer::get_default_font_name() const
{
    return _t("sans-serif");
}

std::shared_ptr<litehtml::element>  SDLContainer::create_element(const litehtml::tchar_t *tag_name,
                                    const litehtml::string_map &attributes,
                                    const std::shared_ptr<litehtml::document> &doc)
{
    if (!t_strcasecmp(tag_name, _t("input"))) {
        auto iter = attributes.find(_t("type"));
        if (iter != attributes.end()) {
            if (!t_strcasecmp(iter->second.c_str(), _t("text"))) {
                /*if (m_input)  {
                    m_input = nullptr;
                }*/

                auto input = std::make_shared<el_input>(doc, m_renderer, this, HtmlInputType::TEXT);
                m_vecInput.push_back(input);
                return input;
            }
            else if (!t_strcasecmp(iter->second.c_str(), _t("button"))) {
                auto input = std::make_shared<el_input>(doc, m_renderer, this, HtmlInputType::BUTTON);
                m_vecInput.push_back(input);
                return input;
            }
        }
    }
    return 0;
}

void SDLContainer::get_media_features(litehtml::media_features& media) const
{
    litehtml::position client;
    get_client_rect(client);
    media.type      = litehtml::media_type_screen;
    media.width     = client.width;
    media.height    = client.height;
    media.device_width  = 640;
    media.device_height = 480;
    media.color     = 8;
    media.monochrome  = 0;
    media.color_index = 256;
    media.resolution  = 96;
}

void SDLContainer::get_language(litehtml::tstring& language, litehtml::tstring& culture) const
{
    language = _t("en");
    culture = _t("");
}

void SDLContainer::link(const std::shared_ptr<litehtml::document> &ptr, const litehtml::element::ptr& el)
{
}
