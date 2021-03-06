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

#include "oopango.h"
#include <pango/pangocairo.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#if LUA_VERSION_NUM >= 502
#define lua_objlen(a, b)      lua_rawlen(a, b)
#else
#define lua_rawlen(a, b) do_not_use_lua_rawlen_use_lua_objlen_instead;
#endif

#define OOPANGO_MT_NAME_LAYOUT     "b5011104-af8a-11df-b66c-fadbdfd72085"
#define OOPANGO_MT_NAME_FONT_DESC  "40144036-af8a-11df-b66c-fadbdfd72085"
#define OOPANGO_MT_NAME_FONT_MAP   "73a92d92-af8a-11df-b66c-fadbdfd72085"

#define ENUM_VAL_TO_LUA_STRING_FUNC(name, type) \
    static int \
    name ## _to_lua (lua_State *L, type val) { \
        int i; \
        for (i = 0; name ## _names[i]; ++i) { \
            if (val == name ## _values[i]) { \
                lua_pushstring(L, name ## _names[i]); \
                return 1; \
            } \
        } \
        return 0; \
    }
#define ENUM_VAL_FROM_LUA_STRING_FUNC(name, type) \
    static type \
    name ## _from_lua (lua_State *L, int pos) { \
        return name ## _values[luaL_checkoption(L, pos, 0, name ## _names)]; \
    }

#define ENUM_STRING_CONVERT(name, type) \
    ENUM_VAL_TO_LUA_STRING_FUNC(name, type) \
    ENUM_VAL_FROM_LUA_STRING_FUNC(name, type)

static const char * const ellipsize_mode_names[] = {
    "none", "start", "middle", "end", 0
};
static const PangoEllipsizeMode ellipsize_mode_values[] = {
    PANGO_ELLIPSIZE_NONE, PANGO_ELLIPSIZE_START, PANGO_ELLIPSIZE_MIDDLE, PANGO_ELLIPSIZE_END
};
ENUM_STRING_CONVERT(ellipsize_mode, PangoEllipsizeMode)

static const char * const wrap_names[] = {
    "word", "char", "word_char", 0
};
static const PangoWrapMode wrap_values[] = {
    PANGO_WRAP_WORD, PANGO_WRAP_CHAR, PANGO_WRAP_WORD_CHAR
};
ENUM_STRING_CONVERT(wrap, PangoWrapMode)

static const char * const alignment_names[] = {
    "left", "center", "right", 0
};
static const PangoAlignment alignment_values[] = {
    PANGO_ALIGN_LEFT, PANGO_ALIGN_CENTER, PANGO_ALIGN_RIGHT
};
ENUM_STRING_CONVERT(alignment, PangoAlignment)

static const char * const style_names[] = {
    "normal", "oblique", "italic", 0
};
static const PangoStyle style_values[] = {
    PANGO_STYLE_NORMAL, PANGO_STYLE_OBLIQUE, PANGO_STYLE_ITALIC
};
ENUM_STRING_CONVERT(style, PangoStyle)

static const char * const variant_names[] = {
    "normal", "small-caps", 0
};
static const PangoVariant variant_values[] = {
    PANGO_VARIANT_NORMAL, PANGO_VARIANT_SMALL_CAPS
};
ENUM_STRING_CONVERT(variant, PangoVariant)

static const char * const weight_names[] = {
    "thin", "ultralight", "light", "book", "normal", "medium", "semibold", "bold", "ultrabold", "heavy", "ultraheavy", 0
};
static const PangoWeight weight_values[] = {
    PANGO_WEIGHT_THIN, PANGO_WEIGHT_ULTRALIGHT, PANGO_WEIGHT_LIGHT, PANGO_WEIGHT_BOOK,
    PANGO_WEIGHT_NORMAL, PANGO_WEIGHT_MEDIUM, PANGO_WEIGHT_SEMIBOLD, PANGO_WEIGHT_BOLD,
    PANGO_WEIGHT_ULTRABOLD, PANGO_WEIGHT_HEAVY, PANGO_WEIGHT_ULTRAHEAVY
};
ENUM_STRING_CONVERT(weight, PangoWeight)

static const char * const stretch_names[] = {
    "ultra-condensed", "extra-condensed", "condensed", "semi-condensed", "normal",
    "semi-expanded", "expanded", "extra-expanded", "ultra-expanded"
};
static const PangoStretch stretch_values[] = {
    PANGO_STRETCH_ULTRA_CONDENSED, PANGO_STRETCH_EXTRA_CONDENSED,
    PANGO_STRETCH_CONDENSED, PANGO_STRETCH_SEMI_CONDENSED, PANGO_STRETCH_NORMAL,
    PANGO_STRETCH_SEMI_EXPANDED, PANGO_STRETCH_EXPANDED,
    PANGO_STRETCH_EXTRA_EXPANDED, PANGO_STRETCH_ULTRA_EXPANDED
};
ENUM_STRING_CONVERT(stretch, PangoStretch)

static const char * const gravity_names[] = {
    "south", "east", "north", "west", "auto", 0
};
static const PangoGravity gravity_values[] = {
    PANGO_GRAVITY_SOUTH, PANGO_GRAVITY_EAST, PANGO_GRAVITY_NORTH, PANGO_GRAVITY_WEST, PANGO_GRAVITY_AUTO,
};
ENUM_STRING_CONVERT(gravity, PangoGravity)

static PangoLayout **
create_layout_userdata(lua_State *L) {
    PangoLayout **obj = lua_newuserdata(L, sizeof(PangoLayout *));
    *obj = 0;
    luaL_getmetatable(L, OOPANGO_MT_NAME_LAYOUT);
    lua_setmetatable(L, -2);
    return obj;
}

static PangoFontDescription **
create_font_desc_userdata(lua_State *L) {
    PangoFontDescription **obj = lua_newuserdata(L, sizeof(PangoFontDescription *));
    *obj = 0;
    luaL_getmetatable(L, OOPANGO_MT_NAME_FONT_DESC);
    lua_setmetatable(L, -2);
    return obj;
}

static PangoFontMap **
create_font_map_userdata(lua_State *L) {
    PangoFontMap **obj = lua_newuserdata(L, sizeof(PangoFontMap *));
    *obj = 0;
    luaL_getmetatable(L, OOPANGO_MT_NAME_FONT_MAP);
    lua_setmetatable(L, -2);
    return obj;
}

static void
push_rect(lua_State *L, PangoRectangle *rect) {
    lua_newtable(L);
#define DO(coord) \
    lua_pushstring(L, #coord); \
    lua_pushnumber(L, rect->coord); \
    lua_rawset(L, -3)

    DO(x);
    DO(y);
    DO(height);
    DO(width);

#undef DO
}

#include "obj_font_desc.c"
#include "obj_font_map.c"
#include "obj_layout.c"
#include "pango_cairo.c"

static int
font_description_from_string(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    PangoFontDescription **obj = create_font_desc_userdata(L);
    *obj = pango_font_description_from_string(name);
    return 1;
}

static int
font_description_new(lua_State *L) {
    PangoFontDescription **obj = create_font_desc_userdata(L);
    *obj = pango_font_description_new();
    return 1;
}

static int
font_description_copy(lua_State *L) {
    PangoFontDescription **font = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_DESC);
    PangoFontDescription **obj = create_font_desc_userdata(L);
    *obj = pango_font_description_copy(*font);
    return 1;
}

static int
units_from_double(lua_State *L) {
    int res = pango_units_from_double(luaL_checknumber(L, 1));
    lua_pushnumber(L, res);
    return 1;
}

static int
units_to_double(lua_State *L) {
    double res = pango_units_to_double(luaL_checknumber(L, 1));
    lua_pushnumber(L, res);
    return 1;
}

static const luaL_Reg
constructor_funcs[] = {
    { "font_description_from_string", font_description_from_string },
    { "font_description_new", font_description_new },
    { "font_description_copy", font_description_copy },
    { "units_from_number", units_from_double },
    { "units_to_number", units_to_double },
    { 0, 0 }
};

static void
add_funcs_to_table (lua_State *L, const luaL_Reg *funcs) {
    const luaL_Reg *l;
    for (l = funcs; l->name; ++l) {
        lua_pushstring(L, l->name);
        lua_pushcfunction(L, l->func);
        lua_rawset(L, -3);
    }
}

static void
create_object_metatable(lua_State *L, const char *mt_name,
                         const char *debug_name, const luaL_Reg *methods)
{
    if (luaL_newmetatable(L, mt_name)) {
        lua_pushliteral(L, "_NAME");
        lua_pushstring(L, debug_name);
        lua_rawset(L, -3);
        add_funcs_to_table(L, methods);
        lua_pushliteral(L, "__index");
        lua_pushvalue(L, -2);
        lua_rawset(L, -3);
    }

    lua_pop(L, 1);
}

int
luaopen_oopango(lua_State *L) {

#ifdef VALGRIND_LUA_MODULE_HACK
    /* Hack to allow Valgrind to access debugging info for the module. */
    luaL_getmetatable(L, "_LOADLIB");
    lua_pushnil(L);
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);
#endif

    /* Create the table to return from 'require' */
    lua_newtable(L);
    lua_pushliteral(L, "_NAME");
    lua_pushliteral(L, "pango");
    lua_rawset(L, -3);
    lua_pushliteral(L, "_VERSION");
    lua_pushliteral(L, VERSION);
    lua_rawset(L, -3);
    lua_pushliteral(L, "_PANGO_VERSION");
    lua_pushstring(L, pango_version_string());
    lua_rawset(L, -3);
    add_funcs_to_table(L, constructor_funcs);
    lua_pushliteral(L, "cairo");
    lua_newtable(L);
    add_funcs_to_table(L, cairo_funcs);
    lua_rawset(L, -3);

    lua_pushliteral(L, "PANGO_SCALE");
    lua_pushnumber(L, PANGO_SCALE);
    lua_rawset(L, -3);

    /* Create the metatables for objects of different types. */
    create_object_metatable(L, OOPANGO_MT_NAME_LAYOUT, "pango layout object",
                            layout_methods);
    create_object_metatable(L, OOPANGO_MT_NAME_FONT_DESC, "pango font description object",
                            font_desc_methods);
    create_object_metatable(L, OOPANGO_MT_NAME_FONT_MAP, "pango font map object",
                            font_map_methods);

    return 1;
}

/* vi:set ts=4 sw=4 expandtab: */
