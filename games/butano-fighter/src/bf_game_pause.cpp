#include "bf_game_pause.h"

#include "btn_music.h"
#include "btn_keypad.h"
#include "btn_sound_items.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_palettes.h"
#include "btn_regular_bg_builder.h"
#include "btn_regular_bg_items_pause.h"
#include "bf_butano_background.h"

namespace bf::game
{

void pause::update(const butano_background& butano_background)
{
    if(_active)
    {
        if(btn::keypad::pressed(btn::keypad::button_type::START))
        {
            _bg.reset();
            btn::bg_palettes::set_grayscale_intensity(0);
            btn::sprite_palettes::set_grayscale_intensity(0);
            btn::sound_items::pause_out.play();

            if(btn::music::paused())
            {
                btn::music::resume();
            }

            _active = false;
        }
    }
    else
    {
        if(! butano_background.silhouette_visible() && btn::keypad::pressed(btn::keypad::button_type::START))
        {
            btn::regular_bg_builder builder(btn::regular_bg_items::pause);
            builder.set_priority(0);
            builder.set_ignore_camera(true);
            _bg = builder.release_build();
            btn::bg_palettes::set_grayscale_intensity(1);
            btn::sprite_palettes::set_grayscale_intensity(1);
            btn::sound_items::pause_in.play();

            if(btn::music::playing())
            {
                btn::music::pause();
            }

            _active = true;
        }
    }
}

}