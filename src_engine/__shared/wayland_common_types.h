#pragma once

#include <wayland-client.h>

struct SWaylandWindowNativeHandle
{
    wl_display* display;
    wl_surface* surface;
};
