R"(#version 460 core
// the bottom-left corner of the current triangle.
flat in vec2 origin;

// the position of the current pixel.
smooth in vec2 frag_pos;

out vec4 color;

// a texture containing the tile sprites used on the tilemap.
uniform sampler2D tileset;

// the tile data of the tilemap.
// uses the red channel for encoding the tile index in the tileset.
uniform sampler2D tile_data;

// number of tiles within the tileset.
uniform float tileset_size;

// size of the tile data texture in tiles.
uniform vec2 map_size;

void main() {
    /* Get the tilemap data position to sample the tilemap data */
    vec2 tile_data_pos = (frag_pos - origin) / map_size;
    tile_data_pos.y = 1.0 - tile_data_pos.y; 

    /* Sample the tiledata texture to get the relative tile index */
    float index = texture(tile_data, tile_data_pos).r * 255.0 - 1.0/* <- because it is incremented once on the CPU */;

    /* Offset of the current tile */
    vec2 map_pos = frag_pos - origin;
    vec2 offset = map_pos - floor(map_pos);

    /* Sample the tileset texture using the tile index */
    color = texture(tileset, vec2((index + offset.x) / tileset_size, 1.0 - offset.y));

    /* Any tiles that are '0' are transparent */
    color.a *= index + 1;
    color.a /= 10.0;
})"