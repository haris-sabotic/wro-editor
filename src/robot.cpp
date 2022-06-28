#include "robot.hpp"

Rect RobotData::screen_rect(const Rect &map_rect, int board_width,
                                        int board_height) const {
    float ux = rect.x / board_width;
    float uy = rect.y / board_height;

    float uw = rect.width / board_width;
    float uh = rect.height / board_height;

    return Rect(map_rect.width * ux, map_rect.height * uy,
                map_rect.width * uw, map_rect.height * uh);
}
