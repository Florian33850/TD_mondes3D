#version 330 core

out vec4 out_color;

in vec3 var_light_dir;
in vec3 var_view_pos;
in vec3 var_normal;
in vec3 var_color;

vec3 blinn(vec3 normal, vec3 view_dir, vec3 light_dir, vec3 diffuse_col, vec3 specular_col, float s) {
    float diffuse = max(dot(normal,light_dir),0);
    vec3 h = normalize(light_dir + view_dir);
    float specular = pow((max(dot(normal,h),0)),s);
    vec3 phong= vec3(0,0,0);

    /* Specular color
   phong= phong + specular_col * specular;
    */

    /* Difffuse color
     phong = phong + diffuse * diffuse_col;
     */
    
    //Phong
    vec3 vec= diffuse_col + specular_col * specular;
    phong= phong + diffuse * vec;
    
    return phong;
}

void main(void) {

    float light_intensity=1;
 
    vec3 color = light_intensity * blinn(var_normal,normalize(-var_view_pos.xyz),var_light_dir,var_color,vec3(1,1,1),26);
    out_color=vec4(color.xyz ,1);
    
}