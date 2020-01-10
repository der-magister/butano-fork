#ifndef BTN_DISPLAY_H
#define BTN_DISPLAY_H

#include "btn_common.h"

namespace btn
{
    class size;
}

namespace btn::display
{
    [[nodiscard]] BTN_CODE_CONST int width();

    [[nodiscard]] BTN_CODE_CONST int height();

    [[nodiscard]] BTN_CODE_CONST size dimensions();
}

#endif
