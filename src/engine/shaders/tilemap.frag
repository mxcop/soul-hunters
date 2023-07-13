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
    /* Not really sure why we need the 'map_size' multiplied by 4, but it works... */
    vec2 quad_size = map_size * 4.0;
    vec2 relative_pos = (frag_pos - origin) / quad_size;

    /* Sample the tiledata texture to get the relative tile index */
    float index = floor(texture(tile_data, relative_pos).r * 256.0) - 1.0/* <- because it is incremented once on the CPU */;

    /* Offset of the current tile */
    vec2 map_pos = relative_pos * map_size;
    vec2 offset = map_pos - floor(map_pos);

    /* Sample the tileset texture using the tile index */
    color = texture(tileset, vec2(index / tileset_size, 0.0) + (offset / vec2(tileset_size, 1.0)));

    /* Any tiles that are '0' are transparent */
    color.a *= index + 1;
})"