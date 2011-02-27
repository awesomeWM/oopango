/* Copyright (C) 2010-2011 Uli Schlachter
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cairo.h>
#include <oocairo.h>

static int
cairo_layout_create(lua_State *L) {
    cairo_t **context = luaL_checkudata(L, 1, OOCAIRO_MT_NAME_CONTEXT);
    PangoLayout **obj = create_layout_userdata(L);
    *obj = pango_cairo_create_layout(*context);
    return 1;
}

static int
cairo_update_layout(lua_State *L) {
    cairo_t **context = luaL_checkudata(L, 1, OOCAIRO_MT_NAME_CONTEXT);
    PangoLayout **layout = luaL_checkudata(L, 2, OOPANGO_MT_NAME_LAYOUT);
    pango_cairo_update_layout(*context, *layout);
    return 0;
}

static int
cairo_show_layout(lua_State *L) {
    cairo_t **context = luaL_checkudata(L, 1, OOCAIRO_MT_NAME_CONTEXT);
    PangoLayout **layout = luaL_checkudata(L, 2, OOPANGO_MT_NAME_LAYOUT);
    pango_cairo_show_layout(*context, *layout);
    return 0;
}

static int
cairo_layout_path(lua_State *L) {
    cairo_t **context = luaL_checkudata(L, 1, OOCAIRO_MT_NAME_CONTEXT);
    PangoLayout **layout = luaL_checkudata(L, 2, OOPANGO_MT_NAME_LAYOUT);
    pango_cairo_layout_path(*context, *layout);
    return 0;
}

static int
cairo_font_map_get_default(lua_State *L) {
    PangoFontMap **map = create_font_map_userdata(L);
    *map = pango_cairo_font_map_get_default();
    g_object_ref(*map);
    return 1;
}

static const luaL_Reg
cairo_funcs[] = {
    { "layout_create", cairo_layout_create },
    { "update_layout", cairo_update_layout },
    { "show_layout", cairo_show_layout },
    { "layout_path", cairo_layout_path },
    { "font_map_get_default", cairo_font_map_get_default },
    { 0, 0 }
};
