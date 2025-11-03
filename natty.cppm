#ifdef LECO_TARGET_APPLE
#pragma leco add_impl apple
#elif LECO_TARGET_WINDOWS
#pragma leco add_impl windows
#endif

export module natty;
import dotz;
import hai;
import jute;
import stubby;

export namespace natty {
  struct font;
  using font_t = hai::pimpl<font *>;
  font_t create_font(const char *name, unsigned size);

  struct surface;
  using surface_t = hai::pimpl<surface *>;
  surface_t create_surface(unsigned w, unsigned h);

  struct draw_params {
    surface_t & surface;
    font_t & font;
    dotz::ivec2 position;
    jute::view text;
  };
  void draw(const draw_params & cmd);

  static_assert(sizeof(unsigned) == 4);
  const hai::array<unsigned> &surface_data(surface *);
} // namespace natty
