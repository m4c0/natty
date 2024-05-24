#pragma leco tool

import hai;
import natty;
import stubby;

int main() {
  auto font = natty::create_font("Helvetica", 32);
  auto surf = natty::create_surface(256, 256);

  boosh(*surf);
  stbi::write_rgba("out/test.png", 256, 256, natty::surface_data(*surf));
}
