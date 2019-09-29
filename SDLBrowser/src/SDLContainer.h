#pragma once

#include <litehtml.h>
#include <unordered_map>

struct SDL_Renderer;
typedef struct _TTF_Font TTF_Font;
class el_input;



class SDLContainer : public litehtml::document_container
{
public:
    SDLContainer(litehtml::context* html_context, SDL_Renderer* renderer);
    virtual ~SDLContainer(void);

    virtual litehtml::uint_ptr			create_font(const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm) override;
    virtual void						delete_font(litehtml::uint_ptr hFont) override;
    virtual int							text_width(const litehtml::tchar_t* text, litehtml::uint_ptr hFont) override;
    virtual void						draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos) override;

    virtual int							pt_to_px(int pt) override;
    virtual int							get_default_font_size() const override;
    virtual const litehtml::tchar_t*	get_default_font_name() const override;
    virtual void						draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker) override;
    virtual void						load_image(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready) override;
    virtual void						get_image_size(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz) override;
    virtual void						draw_image(litehtml::uint_ptr hdc, const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, const litehtml::position& pos) {};
    virtual void						draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg) override;
    virtual void						draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) override;

    virtual	void						transform_text(litehtml::tstring& text, litehtml::text_transform tt) override;
    virtual void						set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y) override;
    virtual void						del_clip() override;
    virtual std::shared_ptr<litehtml::element>	create_element(const litehtml::tchar_t* tag_name, const litehtml::string_map& attributes, const std::shared_ptr<litehtml::document>& doc) override;
    virtual void						get_media_features(litehtml::media_features& media) const override;
    virtual void						get_language(litehtml::tstring& language, litehtml::tstring & culture) const override;
    virtual void						link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el) override;
    //virtual litehtml::tstring			resolve_color(const litehtml::tstring& color) const override;

    // litehtml interface not yet implemented
    void get_client_rect(litehtml::position& client) const;
    void on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el);
    void set_cursor(const litehtml::tchar_t* cursor);
    void import_css(litehtml::tstring& text, const litehtml::tstring& url, litehtml::tstring& baseurl);
    void set_caption(const litehtml::tchar_t* caption);
    void set_base_url(const litehtml::tchar_t* base_url);


    virtual void                        make_url(const litehtml::tchar_t* url, const litehtml::tchar_t* basepath, litehtml::tstring& out);
  //  virtual image_ptr					get_image(LPCWSTR url, bool redraw_on_ready) = 0;
    void								clear_images();
  //  void								add_image(std::wstring& url, image_ptr& img);
  //  void								remove_image(std::wstring& url);
  //  void								make_url_utf8(const char* url, const char* basepath, std::wstring& out);

protected:
 //   virtual void						draw_ellipse(cairo_t* cr, int x, int y, int width, int height, const litehtml::web_color& color, double line_width);
  //  virtual void						fill_ellipse(cairo_t* cr, int x, int y, int width, int height, const litehtml::web_color& color);
  //  virtual void						rounded_rectangle(cairo_t* cr, const litehtml::position &pos, const litehtml::border_radiuses &radius);

    //void								set_color(cairo_t* cr, litehtml::web_color color) { cairo_set_source_rgba(cr, color.red / 255.0, color.green / 255.0, color.blue / 255.0, color.alpha / 255.0); }
private:

  //  void								apply_clip(cairo_t* cr);
  //  bool								add_path_arc(cairo_t* cr, double x, double y, double rx, double ry, double a1, double a2, bool neg);

  //  void								draw_txdib(cairo_t* cr, CTxDIB* bmp, int x, int y, int cx, int cy);
  //  void								lock_images_cache();
   // void								unlock_images_cache();

    SDL_Renderer* m_renderer;
    std::unordered_map<std::string, TTF_Font*> m_fonts;

    std::vector<std::shared_ptr<el_input>> m_vecInput;
};
