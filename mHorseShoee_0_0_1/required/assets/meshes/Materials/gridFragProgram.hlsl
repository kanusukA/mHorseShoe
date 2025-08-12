
#define mod(x,y) ((x) - (y) * floor((x)/(y)))


float4 mainPS(
    //uniform float gridSize,
    float4 pos : SV_Position,
    float2 uv : TEXCOORD0,

    uniform float3 cameraPos,

    //uniform float grid_size,

    uniform float cell_size,
    uniform float sub_cell_size,
    
    uniform float4 cell_col,
    uniform float4 sub_cell_col,

    uniform float grid_line_width,
    uniform float sub_grid_line_width

    //uniform float subcell_size


) : SV_Target
{

    //const float half_cell_size = cell_size * 0.5f;
    //const float half_subcell_size = subcell_size * 0.5f;

    float cell_coords_x = (uv.x % cell_size) * (1 / cell_size);
    float cell_coords_y = (uv.y % cell_size) * (1 / cell_size);

    float4 fill_col = (step(grid_line_width,cell_coords_y) || step(grid_line_width,cell_coords_x)) * cell_col;

    float sub_cell_coords_x = (uv.x % sub_cell_size) * (1 / sub_cell_size);
    float sub_cell_coords_y = (uv.y % sub_cell_size) * (1 / sub_cell_size);

    float4 sub_fill_col = (step(sub_grid_line_width,sub_cell_coords_x) || step(sub_grid_line_width,sub_cell_coords_y)) * sub_cell_col;

    float uv_x = uv.x * 10000;
    float uv_y = uv.y * 10000;
    
    float camX = cameraPos.x + 5000;
    float camZ = cameraPos.z + 5000;

    float x_alpha = smoothstep(1000,800,abs(camX - uv_x));
    float Z_alpha = smoothstep(1000,800,abs(camZ - uv_y));

    float4 final_col = (fill_col + sub_fill_col) * (x_alpha * Z_alpha);
    
    

    //float2 subcell_coords = mod( uv + half_subcell_size, subcell_size);

    
    return final_col;
}