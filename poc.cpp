#pragma leco tool

import hai;
import natty;
import stubby;

int main() {
  auto font = natty::create_font("Helvetica", 64);
  auto surf = natty::create_surface(256, 256);

  unsigned w = 256;
  unsigned h = 256;
  hai::array<stbi::pixel> data{w * h};
  boosh(w, h, data);
  stbi::write_rgba("out/test.png", w, h, data);
}
