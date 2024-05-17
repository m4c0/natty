#pragma leco tool

#ifdef LECO_TARGET_APPLE
#pragma leco add_impl apple
#endif

extern "C" void boosh();

int main() { boosh(); }
