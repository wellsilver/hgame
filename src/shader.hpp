const char *shader_code = R"(
struct rgb {
  unsigned char r,g,b,a;
};

kernel void pixel(__global struct rgb *out) {
  out[get_global_id(0)].r = 255;
})";