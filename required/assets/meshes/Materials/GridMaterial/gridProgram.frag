#version 450 core




layout (location = 0) out vec4 FragColor;

layout (location = 1) in vec2 uv;
  
layout (std140, binding = 1,row_major) uniform OgreUniforms{
    //vec3 cameraPos;

    float fade_radius;
    float fade_cutoff;

    float cell_size;
    float sub_cell_size;

    vec4 cell_col;
    vec4 sub_cell_col;

    float grid_line_width;
    float sub_grid_line_width;

};

void main()
{

    //const float half_cell_size = cell_size * 0.5f;
    //const float half_subcell_size = subcell_size * 0.5f;

    float cell_coords_x = clamp((mod(uv.x , cell_size)) * (1 / cell_size),0.0,1.0);
    float cell_coords_y = clamp((mod(uv.y , cell_size)) * (1 / cell_size),0.0,1.0);

    vec4 fill_col = max(step(grid_line_width,cell_coords_y) , step(grid_line_width,cell_coords_x)) * cell_col;

    float sub_cell_coords_x = clamp((mod(uv.x , sub_cell_size)) * (1 / sub_cell_size),0.0,1.0);
    float sub_cell_coords_y = clamp((mod(uv.y , sub_cell_size)) * (1 / sub_cell_size),0.0,1.0);

    vec4 sub_fill_col = mod(step(sub_grid_line_width,sub_cell_coords_x) , step(sub_grid_line_width,sub_cell_coords_y)) * sub_cell_col;

    vec2 cur_pos_uv = vec2(0.5,0.5);

    float fade = 1 - smoothstep(fade_cutoff/10, (fade_cutoff/10) + (fade_radius/10), distance(cur_pos_uv,uv));

    FragColor = (fill_col + sub_fill_col) * fade;
    

    //float2 subcell_coords = mod( uv + half_subcell_size, subcell_size);
    
}