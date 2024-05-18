#ifdef LECO_TARGET_APPLE
#pragma leco add_impl apple
#elif LECO_TARGET_WINDOWS
#pragma leco add_impl windows
#endif

export module natty;
import hai;

namespace natty {
export struct font;
export using font_t = hai::pimpl<font *>;
export font_t create_font(const char *name, unsigned size);

export struct surface;
export using surface_t = hai::pimpl<surface *>;
export surface_t create_surface(unsigned w, unsigned h);

} // namespace natty

export void boosh();
