#version 330 core

uniform sampler2D text2D_1;
uniform sampler2D text2D_2;
uniform sampler2D text2D_3;
uniform sampler2D text2D_4;
uniform sampler2D text2D_5;
uniform sampler2D text2D_mipmap;

uniform vec3 lightDir;

in vec3 v_color;
in vec3 v_normal;
in vec3 v_view;
in vec2 v_text_coord;

out vec4 out_color;

vec3 blinn(vec3 n, vec3 v, vec3 l, vec3 dCol, vec3 sCol, float s)
{
  vec3 res = vec3(0,0,0);
  float dc = max(0,dot(n,l));
  if(dc>0) {
    res = dCol * dc;
    float sc = max(0,dot(n,normalize(v+l)));
    if(sc>0)
      res += sCol * pow(sc,s) * dc;
  }
  return res;
}

void main(void) {
  float light = max(dot(v_normal,lightDir), 0);

  //Earth texture
  vec4 texture_1 = texture(text2D_1,v_text_coord);
  //out_color = light * texture_1;

  //Earth cloud texture
  vec4 texture_2 = texture(text2D_2,v_text_coord);
  vec4 texture_1_2 = mix(texture_1, texture_2, texture_2);
  //out_color = light * texture_1_2;

  //Earth night texture
  vec4 texture_3 = texture(text2D_3,v_text_coord);
  vec4 texture_1_2_3 = mix(texture_3, texture_1_2, light);
  //out_color = texture_1_2_3;

  //Plane
  vec4 texture_4 = texture(text2D_4,v_text_coord);
  //out_color = texture_4;

  //Cow
  vec4 texture_5 = texture(text2D_5,v_text_coord);
  //out_color = light * texture_5;

  //Mipmap
  vec4 texture_mipmap = texture(text2D_mipmap,v_text_coord);
  out_color = light * texture_mipmap;
}
  
