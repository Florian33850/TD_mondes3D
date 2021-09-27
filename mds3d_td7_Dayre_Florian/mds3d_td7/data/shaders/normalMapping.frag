#version 330 core

uniform sampler2D text2D_1;
uniform sampler2D text2D_2;
uniform sampler2D text2D_3;
uniform sampler2D text2D_4;
uniform sampler2D text2D_5;
uniform sampler2D text2D_mipmap;
uniform sampler2D text2D_normals;
uniform sampler2D text2D_diffuse;
uniform sampler2D text2D_earth_normal;

uniform vec3 lightDir;

in vec3 v_color;
in vec3 v_normal;
in vec3 v_view;
in vec2 v_text_coord;
in vec3 v_tangent;
in vec3 v_bitangent;

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
  //Earth texture
  vec4 texture_1 = texture(text2D_1,v_text_coord);
  //out_color = light * texture_1;

  //Earth cloud texture
  vec4 texture_2 = texture(text2D_2,v_text_coord);
  vec4 texture_1_2 = mix(texture_1, texture_2, texture_2);
  //out_color = light * texture_1_2;

  //Earth night texture
  vec4 texture_3 = texture(text2D_3,v_text_coord);
  //vec4 texture_1_2_3 = mix(texture_3, texture_1_2, light);
  //out_color = texture_1_2_3;

  //Plane
  vec4 texture_4 = texture(text2D_4,v_text_coord);
  //out_color = texture_4;

  //Cow
  vec4 texture_5 = texture(text2D_5,v_text_coord);
  //out_color = light * texture_5;

  //Mipmap
  vec4 texture_mipmap = texture(text2D_mipmap,v_text_coord);
  //out_color = light * texture_mipmap;

  //Normal mapping sur plan
  vec4 texture_normals = texture(text2D_normals,v_text_coord);
  vec3 n = (texture_normals.xyz*2)-1;
  float light = max(dot(n,lightDir), 0);

  vec4 texture_diffuse = texture(text2D_diffuse,v_text_coord);
  vec3 diffuse_color = texture_diffuse.xyz;

  //out_color = vec4(blinn(normalize(n), normalize(v_view), lightDir, diffuse_color, vec3(1,1,1), 50), 1);

  //Calcul repère tangeant
  //out_color = vec4(abs(v_tangent), 1);
  //out_color = vec4(abs(v_bitangent), 1);

  //Normal mapping sur sphère
   vec4 texture_earth_normal = texture(text2D_earth_normal,v_text_coord);
    vec3 normal_map=(2*texture_earth_normal.xyz-1);
    vec3 color_normal= texture(text2D_diffuse,v_text_coord).xyz;

    vec3 v_n=normalize(v_normal);
    vec3 v_t=normalize(v_tangent);
    vec3 v_b=normalize(v_bitangent);

    mat3 tbnvMatrix = transpose(mat3(v_t, v_b, v_n));


    out_color = vec4(blinn(normalize(normal_map),tbnvMatrix * normalize(v_view), tbnvMatrix*lightDir, color_normal, vec3(1,1,1), 50),1.0);
    out_color=vec4(v_t,1);
}
  
