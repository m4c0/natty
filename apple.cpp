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
  using cf_attr_str     = hay<CFAttributedStringRef, nullptr, CFRelease>;
  using cg_colour_space = hay<CGColorSpaceRef, nullptr, CGColorSpaceRelease>;
  using cg_ctx          = hay<CGContextRef, nullptr, CGContextRelease>;
  using cf_dict         = hay<CFMutableDictionaryRef, nullptr, CFRelease>;
  using ct_font         = hay<CTFontRef, nullptr, CFRelease>;
  using ct_line         = hay<CTLineRef, nullptr, CFRelease>;
  using cf_str          = hay<CFStringRef, nullptr, CFRelease>;
}

namespace natty {
struct font {
  ct_font font;
  cf_dict attrs;
  unsigned h;
};

font_t create_font(const char * name, unsigned size) {
  cf_str cfname { CFStringCreateWithCStringNoCopy(nullptr, name, kCFStringEncodingUTF8, kCFAllocatorNull) };

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
  cg_ctx ctx;
  hai::array<unsigned> data;
  unsigned h;
};
surface_t create_surface(unsigned w, unsigned h) {
  cg_colour_space colour_space { CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB) };

  hai::array<unsigned> data{w * h};

  return surface_t{new surface{
                       .ctx{CGBitmapContextCreate(
                           data.begin(), w, h, 8, w * 4, colour_space,
                           kCGImageAlphaPremultipliedLast)},
                       .data{traits::move(data)},
                       .h = h,
                   },
                   [](auto x) { delete x; }};
};

void draw(const draw_params & p) {
  auto h = (*p.surface)->h;
  auto & ctx = (*p.surface)->ctx;
  CGContextSetTextPosition(ctx, p.position.x, h - p.position.y - (*p.font)->h);

  cf_str cfstr {
    CFStringCreateWithBytesNoCopy(
      nullptr, reinterpret_cast<const UInt8 *>(p.text.begin()), p.text.size(),
      kCFStringEncodingUTF8, false, kCFAllocatorNull)
  };

  cf_attr_str attr_str { CFAttributedStringCreate(nullptr, cfstr, (*p.font)->attrs) };
  ct_line line { CTLineCreateWithAttributedString(attr_str) };
  CTLineDraw(line, ctx);
}

const hai::array<unsigned> &surface_data(surface *s) { return s->data; }
} // namespace natty
