#include "polygon_sprite.h"

#include "btn_sprite_items_texture.h"
#include "polygon.h"
#include "../../butano/hw/include/btn_hw_sprites.h"

polygon_sprite::polygon_sprite(const polygon& polygon, int graphics_index, int z_order) :
    _polygons(1, &polygon),
    _tiles(btn::sprite_items::texture.tiles_item().create_tiles(graphics_index)),
    _palette(btn::sprite_items::texture.palette_item().create_palette()),
    _z_order(z_order)
{
}

polygon_sprite::polygon_sprite(const btn::span<const polygon*>& polygons, int graphics_index, int z_order) :
    _tiles(btn::sprite_items::texture.tiles_item().create_tiles(graphics_index)),
    _palette(btn::sprite_items::texture.palette_item().create_palette()),
    _z_order(z_order)
{
    for(const polygon* polygon : polygons)
    {
        _polygons.push_back(polygon);
    }
}

void polygon_sprite::update(int max_polygon_sprites, uint16_t* hdma_source)
{
    btn::array<hline, btn::display::height()> hlines;
    hline* hlines_data = hlines.data();
    int new_minimum_y = btn::display::height();
    int new_maximum_y = 0;

    for(const polygon* polygon : _polygons)
    {
        const btn::fixed_point* vertices_data = polygon->vertices().data();

        btn::point vertices[] = {
            btn::point(vertices_data[0].x().integer(), vertices_data[0].y().integer()),
            btn::point(vertices_data[1].x().integer(), vertices_data[1].y().integer()),
            btn::point(vertices_data[2].x().integer(), vertices_data[2].y().integer()),
            btn::point(vertices_data[3].x().integer(), vertices_data[3].y().integer())
        };

        int top_index = 0;
        int top_index_y = vertices[0].y();

        if(top_index_y < new_minimum_y)
        {
            new_minimum_y = top_index_y;
        }
        else if(top_index_y > new_maximum_y)
        {
            new_maximum_y = top_index_y;
        }

        for(int index = 1; index < 4; ++index)
        {
            int y = vertices[index].y();

            if(y < top_index_y)
            {
                top_index = index;
                top_index_y = y;
            }

            if(y < new_minimum_y)
            {
                new_minimum_y = y;
            }
            else if(y > new_maximum_y)
            {
                new_maximum_y = y;
            }
        }

        int left_index, right_index, bottom_index;

        if(top_index == 0)
        {
            left_index = 3;
            right_index = 1;
            bottom_index = 2;
        }
        else
        {
            left_index = top_index - 1;
            right_index = (top_index + 1) % 4;
            bottom_index = (top_index + 2) % 4;
        }

        const btn::point& top_vertex = vertices[top_index];
        const btn::point& left_vertex = vertices[left_index];
        const btn::point& right_vertex = vertices[right_index];
        const btn::point& bottom_vertex = vertices[bottom_index];
        _draw_line(true,
                   top_vertex.x(), top_vertex.y(), left_vertex.x(), left_vertex.y(), hlines_data);
        _draw_line(false,
                   top_vertex.x(), top_vertex.y(), right_vertex.x(), right_vertex.y(), hlines_data);
        _draw_line(left_vertex.y() < bottom_vertex.y(),
                   left_vertex.x(), left_vertex.y(), bottom_vertex.x(), bottom_vertex.y(), hlines_data);
        _draw_line(right_vertex.y() > bottom_vertex.y(),
                   right_vertex.x(), right_vertex.y(), bottom_vertex.x(), bottom_vertex.y(), hlines_data);
    }

    btn::hw::sprites::handle_type base_sprite_handle;
    btn::hw::sprites::setup_regular(btn::sprite_items::texture.shape_size(), _tiles.id(), _palette.id(),
                                    _palette.bpp_mode(), base_sprite_handle);

    int minimum_y = btn::min(_minimum_y, new_minimum_y);
    int maximum_y = btn::max(_maximum_y, new_maximum_y);
    _setup_attributes(&base_sprite_handle, hlines_data, _z_order, max_polygon_sprites, minimum_y, maximum_y,
                      hdma_source);
    _minimum_y = new_minimum_y;
    _maximum_y = new_maximum_y;
}
