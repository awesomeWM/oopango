#include "oopango.h"
#include <pango/pangocairo.h>
#include <cairo.h>
#include <oocairo.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define OOPANGO_MT_NAME_LAYOUT     "b5011104-af8a-11df-b66c-fadbdfd72085"
#define OOPANGO_MT_NAME_FONT_DESC  "40144036-af8a-11df-b66c-fadbdfd72085"

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

static const char * const ellipsize_mode_names[] = {
    "none", "start", "middle", "end", 0
};
static const PangoEllipsizeMode ellipsize_mode_values[] = {
    PANGO_ELLIPSIZE_NONE, PANGO_ELLIPSIZE_START, PANGO_ELLIPSIZE_MIDDLE, PANGO_ELLIPSIZE_END
};
ENUM_VAL_TO_LUA_STRING_FUNC(ellipsize_mode, PangoEllipsizeMode)
ENUM_VAL_FROM_LUA_STRING_FUNC(ellipsize_mode, PangoEllipsizeMode)

static const char * const wrap_names[] = {
    "word", "char", "word_char", 0
};
static const PangoWrapMode wrap_values[] = {
    PANGO_WRAP_WORD, PANGO_WRAP_CHAR, PANGO_WRAP_WORD_CHAR
};
ENUM_VAL_TO_LUA_STRING_FUNC(wrap, PangoWrapMode)
ENUM_VAL_FROM_LUA_STRING_FUNC(wrap, PangoWrapMode)

static const char * const alignment_names[] = {
    "left", "center", "right", 0
};
static const PangoAlignment alignment_values[] = {
    PANGO_ALIGN_LEFT, PANGO_ALIGN_CENTER, PANGO_ALIGN_RIGHT
};
ENUM_VAL_TO_LUA_STRING_FUNC(alignment, PangoAlignment)
ENUM_VAL_FROM_LUA_STRING_FUNC(alignment, PangoAlignment)

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
#include "obj_layout.c"

static int
font_description_from_string(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    PangoFontDescription **obj = create_font_desc_userdata(L);
    *obj = pango_font_description_from_string(name);
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

static int
cairo_layout_create(lua_State *L) {
    cairo_t **context = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    PangoLayout **obj = create_layout_userdata(L);
    *obj = pango_cairo_create_layout(*context);
    return 1;
}

static int
cairo_update_layout(lua_State *L) {
    cairo_t **context = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    PangoLayout **layout = luaL_checkudata(L, 2, OOPANGO_MT_NAME_LAYOUT);
    pango_cairo_update_layout(*context, *layout);
    return 0;
}

static int
cairo_show_layout(lua_State *L) {
    cairo_t **context = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    PangoLayout **layout = luaL_checkudata(L, 2, OOPANGO_MT_NAME_LAYOUT);
    pango_cairo_show_layout(*context, *layout);
    return 0;
}

static int
cairo_layout_path(lua_State *L) {
    cairo_t **context = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    PangoLayout **layout = luaL_checkudata(L, 2, OOPANGO_MT_NAME_LAYOUT);
    pango_cairo_layout_path(*context, *layout);
    return 0;
}

static const luaL_Reg
constructor_funcs[] = {
    { "cairo_layout_create", cairo_layout_create },
    { "cairo_update_layout", cairo_update_layout },
    { "cairo_show_layout", cairo_show_layout },
    { "cairo_layout_path", cairo_layout_path },
    { "font_description_from_string", font_description_from_string },
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

    lua_pushliteral(L, "PANGO_SCALE");
    lua_pushnumber(L, PANGO_SCALE);
    lua_rawset(L, -3);

    /* Create the metatables for objects of different types. */
    create_object_metatable(L, OOPANGO_MT_NAME_LAYOUT, "pango layout object",
                            layout_methods);
    create_object_metatable(L, OOPANGO_MT_NAME_FONT_DESC, "pango font description object",
                            font_desc_methods);

    return 1;
}

/* vi:set ts=4 sw=4 expandtab: */
