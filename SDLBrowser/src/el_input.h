#pragma once

#include <litehtml.h>

struct SDL_Renderer;
class SDLContainer;
enum HtmlInputType;

class el_input : public litehtml::html_tag
{
public:
    el_input(
        const std::shared_ptr<litehtml::document>& doc, 
        SDL_Renderer* renderer, 
        SDLContainer* container,
        HtmlInputType inputType);
    virtual ~el_input(void);

    virtual int	 line_height() const override;
    virtual bool is_replaced() const override;
    virtual void get_content_size(litehtml::size& sz, int max_width) override;
    virtual litehtml::style_display    get_display() const override;
    virtual litehtml::element_position get_element_position(litehtml::css_offsets* offsets = 0) const override;
    virtual int	 render(int x, int y, int max_width, bool second_pass = false) override;
    virtual void draw(litehtml::uint_ptr hdc, int x, int y, const litehtml::position* clip);
    virtual void parse_styles(bool is_reparse) override;
    virtual void on_click();

   /* BOOL have_focus()
    {
        return m_haveFocus;
    }
    void update_position();
    void set_url(LPCWSTR url);
    std::wstring get_url();
    void set_parent(HWND parent);
    void SetFocus();
    void KillFocus();
    void select_all()
    {
        //m_edit.setSelection(0, -1);
    }
    BOOL OnKeyDown(WPARAM wParam, LPARAM lParam)
    {
        return m_edit.OnKeyDown(wParam, lParam);
    }
    BOOL OnKeyUp(WPARAM wParam, LPARAM lParam)
    {
        return m_edit.OnKeyUp(wParam, lParam);
    }
    BOOL OnChar(WPARAM wParam, LPARAM lParam)
    {
        return m_edit.OnChar(wParam, lParam);
    }
    BOOL OnLButtonDown(int x, int y);
    BOOL OnLButtonUp(int x, int y);
    BOOL OnLButtonDblClick(int x, int y);
    BOOL OnMouseMove(int x, int y);*/

private:
    SDL_Renderer *m_renderer;
    SDLContainer *m_container;
    HtmlInputType m_inputType;
};
