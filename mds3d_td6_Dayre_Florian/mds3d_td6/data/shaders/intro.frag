#version 330 core

in vec2 v_text_coord;

out vec4 out_color;

void main(void) {
  out_color = vec4(v_text_coord, 0, 0);
}
