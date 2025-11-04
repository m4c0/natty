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

    auto fnt = CTFontCreateWithName(cfname, size, nullptr);

    auto attrs = CFDictionaryCreateMutable(nullptr, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFDictionaryAddValue(attrs, kCTFontAttributeName, fnt);
    CFDictionaryAddValue(attrs, kCTForegroundColorAttributeName, CGColorGetConstantColor(kCGColorWhite));
  
    return font_t {
      new font {
        .font { fnt },
        .attrs { attrs },
        .h = size,
      },
      [](auto x) { delete x; }
    };
  }

  struct surface {
    cg_ctx ctx;
    hai::array<unsigned> data;
    unsigned w;
    unsigned h;
  };
  surface_t create_surface(unsigned w, unsigned h) {
    cg_colour_space colour_space { CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB) };

    hai::array<unsigned> data { w * h };
    auto ctx = CGBitmapContextCreate(
        data.begin(), w, h, 8, w * 4, colour_space,
        kCGImageAlphaPremultipliedLast);

    return surface_t {
      new surface {
        .ctx { ctx },
        .data { traits::move(data) },
        .w = w,
        .h = h,
      },
      [](auto x) { delete x; }
    };
  };

  void clear(surface_t & s) {
    auto rect = CGRectMake(0, 0, (*s)->w, (*s)->h);
    CGContextClearRect((*s)->ctx, rect);
  }

  void draw(const draw_params & p) {
    auto h = p.surface->h;
    auto & ctx = p.surface->ctx;
    CGContextSetTextPosition(ctx, p.position.x, h - p.position.y - p.font->h);

    cf_str cfstr {
      CFStringCreateWithBytesNoCopy(
          nullptr, reinterpret_cast<const UInt8 *>(p.text.begin()), p.text.size(),
          kCFStringEncodingUTF8, false, kCFAllocatorNull)
    };

    cf_attr_str attr_str { CFAttributedStringCreate(nullptr, cfstr, p.font->attrs) };
    ct_line line { CTLineCreateWithAttributedString(attr_str) };
    CTLineDraw(line, ctx);
  }

  const hai::array<unsigned> &surface_data(surface *s) { return s->data; }
}
