/* Copyright (C) 2010      Uli Schlachter
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

static int
font_desc_gc(lua_State *L) {
    PangoFontDescription **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_DESC);
    pango_font_description_free(*obj);
    *obj = 0;
    return 0;
}

static int
font_desc_eq(lua_State *L) {
    PangoFontDescription **obj1 = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_DESC);
    PangoFontDescription **obj2 = luaL_checkudata(L, 2, OOPANGO_MT_NAME_FONT_DESC);
    gboolean res = pango_font_description_equal(*obj1, *obj2);
    lua_pushboolean(L, res);
    return 1;
}

static int
font_desc_tostring(lua_State *L) {
    PangoFontDescription **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_DESC);
    char *name = pango_font_description_to_string(*obj);
    lua_pushstring(L, name);
    g_free(name);
    return 1;
}

#define PROPERTY(name, getter, setter) \
static int \
font_desc_set_ ## name (lua_State *L) { \
    PangoFontDescription **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_DESC); \
    pango_font_description_set_ ## name(*obj, getter(L, 2)); \
    return 0; \
} \
static int \
font_desc_get_ ## name (lua_State *L) { \
    PangoFontDescription **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_DESC); \
    setter(L, pango_font_description_get_ ## name(*obj)); \
    return 1; \
}

PROPERTY(family, luaL_checkstring, lua_pushstring)
PROPERTY(style, style_from_lua, style_to_lua)
PROPERTY(variant, variant_from_lua, variant_to_lua)
PROPERTY(weight, weight_from_lua, weight_to_lua)
PROPERTY(stretch, stretch_from_lua, stretch_to_lua)
PROPERTY(gravity, gravity_from_lua, gravity_to_lua)
PROPERTY(size, luaL_checknumber, lua_pushnumber)

static int
font_desc_set_absolute_size(lua_State *L) {
    PangoFontDescription **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_DESC);
    double size = luaL_checknumber(L, 2);
    pango_font_description_set_absolute_size(*obj, size);
    return 0;
}

static int
font_desc_get_size_is_absolute(lua_State *L) {
    PangoFontDescription **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_DESC);
    lua_pushboolean(L, pango_font_description_get_size_is_absolute(*obj));
    return 1;
}

#undef PROPERTY

#define PROPERTY(name) \
    { "set_" #name, font_desc_set_ ## name }, \
    { "get_" #name, font_desc_get_ ## name }

static const luaL_Reg
font_desc_methods[] = {
    { "__gc", font_desc_gc },
    { "__eq", font_desc_eq },
    { "__tostring", font_desc_tostring },
    PROPERTY(family),
    PROPERTY(style),
    PROPERTY(variant),
    PROPERTY(weight),
    PROPERTY(stretch),
    PROPERTY(gravity),
    PROPERTY(size),
    { "set_absolute_size", font_desc_set_absolute_size },
    { "get_size_is_absolute", font_desc_get_size_is_absolute },
    { 0, 0 }
};
#undef PROPERTY

/* vi:set ts=4 sw=4 expandtab: */
