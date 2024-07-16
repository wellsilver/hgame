const char *shader_code = R"(
struct rgb {
  unsigned char r,g,b,a;
};

__kernel void pixel(__global struct rgb *out) {
  // (get_global_id(0)/480, get_global_id(0)%480)
  out[get_global_id(0)].r = 255;
})";