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
    lua_pushboolean(L, res ? 1 : 0);
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

#undef PROPERTY

#define PROPERTY(name) \
    { "set_" #name, font_desc_set_ ## name }, \
    { "get_" #name, font_desc_get_ ## name }

static const luaL_Reg
font_desc_methods[] = {
    { "__gc", font_desc_gc },
    { "__eq", font_desc_eq },
    PROPERTY(family),
    PROPERTY(style),
    PROPERTY(variant),
    PROPERTY(weight),
    PROPERTY(stretch),
    { 0, 0 }
};
#undef PROPERTY

/* vi:set ts=4 sw=4 expandtab: */
