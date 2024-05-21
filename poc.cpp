#pragma leco tool

import hai;
import natty;
import stubby;

int main() {
  auto font = natty::create_font("Helvetica", 64);
  auto surf = natty::create_surface(256, 256);

  unsigned w = 256;
  unsigned h = 256;
  boosh(*surf, w, h);
  stbi::write_rgba("out/test.png", w, h, natty::surface_data(*surf));
}
