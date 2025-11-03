#pragma leco add_framework CoreFoundation
#pragma leco add_framework CoreGraphics
#pragma leco add_framework CoreText
module;

#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>

module natty;
import hai;
import hay;
import stubby;
import traits;

namespace {
struct deleter {
  void operator()(CGColorSpaceRef r) { CGColorSpaceRelease(r); }
  void operator()(CGContextRef r) { CGContextRelease(r); }
  void operator()(CFTypeRef r) { CFRelease(r); }
};
template <typename T> using ref = hai::value_holder<T, deleter>;

  using cf_dict = hay<CFMutableDictionaryRef, nullptr, CFRelease>;
  using ct_font = hay<CTFontRef, nullptr, CFRelease>;
} // namespace

namespace natty {
struct font {
  ct_font font;
  cf_dict attrs;
  unsigned h;
};
font_t create_font(const char * name, unsigned size) {
  CFStringRef cfname = CFStringCreateWithCStringNoCopy(nullptr, name, kCFStringEncodingUTF8, kCFAllocatorNull);

  auto f = new font{
      .font{CTFontCreateWithName(cfname, size, nullptr)},
      .attrs{CFDictionaryCreateMutable(nullptr, 2,
                                       &kCFTypeDictionaryKeyCallBacks,
                                       &kCFTypeDictionaryValueCallBacks)},
      .h = size,
  };

  CFDictionaryAddValue(f->attrs, kCTFontAttributeName, f->font);
  CFDictionaryAddValue(f->attrs, kCTForegroundColorAttributeName, CGColorGetConstantColor(kCGColorWhite));
  return font_t{f, [](auto x) { delete x; }};
}

struct surface {
  ref<CGContextRef> ctx;
  hai::array<unsigned> data;
  unsigned h;
};
surface_t create_surface(unsigned w, unsigned h) {
  ref<CGColorSpaceRef> colour_space { CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB) };

  hai::array<unsigned> data{w * h};

  return surface_t{new surface{
                       .ctx{CGBitmapContextCreate(
                           data.begin(), w, h, 8, w * 4, *colour_space,
                           kCGImageAlphaPremultipliedLast)},
                       .data{traits::move(data)},
                       .h = h,
                   },
                   [](auto x) { delete x; }};
};

void draw(const draw_params & p) {
  auto h = (*p.surface)->h;
  auto & ctx = (*p.surface)->ctx;
  CGContextSetTextPosition(*ctx, p.position.x, h - p.position.y - (*p.font)->h);

  ref<CFStringRef> cfstr { CFStringCreateWithBytesNoCopy(
      nullptr, reinterpret_cast<const UInt8 *>(p.text.begin()), p.text.size(),
      kCFStringEncodingUTF8, false, kCFAllocatorNull) };

  ref<CFAttributedStringRef> attr_str {
    CFAttributedStringCreate(nullptr, *cfstr, (*p.font)->attrs)
  };

  ref<CTLineRef> line { CTLineCreateWithAttributedString(*attr_str) };

  // TODO: adjust position to be top-down
  CTLineDraw(*line, *ctx);
}

const hai::array<unsigned> &surface_data(surface *s) { return s->data; }
} // namespace natty
