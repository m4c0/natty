#pragma leco tool

import hai;
import natty;
import stubby;

int main() {
  auto font = natty::create_font("Arial", 32);
  auto surf = natty::create_surface(256, 256);
  natty::surface_font(*surf, *font);

  natty::surface_position(*surf, 40, 10);
  natty::draw(*surf, "Olá, tudo bem?");

  stbi::write_rgba("out/test.png", 256, 256, natty::surface_data(*surf));
}
