std::string shader_code = R"(
__kernel void pixel(write_only image2d_t out) {
  write_imageui(out, (get_global_id(0)/480, get_global_id(0)%480 ), (255,0,0));
})";