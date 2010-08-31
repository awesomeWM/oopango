static int
layout_gc(lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    g_object_unref(*obj);
    *obj = 0;
    return 0;
}

#define PROPERTY(name, getter, setter) \
static int \
font_desc_set_ ## name (lua_State *L) { \
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT); \
    pango_layout_set_ ## name(*obj, getter(L, 2)); \
    return 0; \
} \
static int \
font_desc_get_ ## name (lua_State *L) { \
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT); \
    setter(L, pango_layout_get_ ## name(*obj)); \
    return 1; \
}

PROPERTY(width, luaL_checknumber, lua_pushnumber)
PROPERTY(height, luaL_checknumber, lua_pushnumber)
PROPERTY(ellipsize, ellipsize_mode_from_lua, ellipsize_mode_to_lua)
PROPERTY(wrap, wrap_from_lua, wrap_to_lua)
PROPERTY(alignment, alignment_from_lua, alignment_to_lua)
PROPERTY(justify, lua_toboolean, lua_pushboolean)

#undef PROPERTY

static int
layout_get_extents(lua_State *L) {
    PangoRectangle ink, logical;
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    pango_layout_get_extents(*obj, &ink, &logical);
    push_rect(L, &ink);
    push_rect(L, &logical);
    return 2;
}

static int
layout_get_pixel_extents(lua_State *L) {
    PangoRectangle ink, logical;
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    pango_layout_get_pixel_extents(*obj, &ink, &logical);
    push_rect(L, &ink);
    push_rect(L, &logical);
    return 2;
}

static int
layout_get_size(lua_State *L) {
    int width, height;
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    pango_layout_get_size(*obj, &width, &height);
    lua_pushnumber(L, width);
    lua_pushnumber(L, height);
    return 2;
}

static int
layout_get_pixel_size(lua_State *L) {
    int width, height;
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    pango_layout_get_pixel_size(*obj, &width, &height);
    lua_pushnumber(L, width);
    lua_pushnumber(L, height);
    return 2;
}

static int
layout_set_font_description(lua_State *L) {
    PangoFontDescription *font_desc = NULL;
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    if (!lua_isnil(L, 2))
    {
        PangoFontDescription **font =
            luaL_checkudata(L, 2, OOPANGO_MT_NAME_FONT_DESC);
        font_desc = *font;
    }
    pango_layout_set_font_description(*obj, font_desc);
    return 0;
}

static int
layout_get_font_description(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    const PangoFontDescription *font_desc = pango_layout_get_font_description(*obj);
    if (font_desc == NULL)
        return 0;
    PangoFontDescription **copy = create_font_desc_userdata(L);
    *copy = pango_font_description_copy(font_desc);
    return 1;
}

static int
layout_set_markup(lua_State *L) {
    size_t markup_len;
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    const char *markup = luaL_checklstring(L, 2, &markup_len);

    /* Instead of calling pango_layout_set_markup(), we use pango_parse_markup()
     * ourselves so that we can do error checking on the result.
     */
    GError *error = NULL;
    char *text = NULL;
    PangoAttrList *list = NULL;

    if (!pango_parse_markup(markup, markup_len, 0, &list, &text, NULL, &error))
    {
        /* Can't use luaL_error() since we have to free error */
        lua_pushfstring(L, "oopango: %s while parsing '%s'",
                error ? error->message : "unknown error", markup);
        if (error)
            g_error_free(error);
        return lua_error(L);
    }

    pango_layout_set_text(*obj, text, -1);
    pango_layout_set_attributes(*obj, list);
    pango_attr_list_unref(list);
    g_free(text);
    return 0;
}

static int
layout_set_text(lua_State *L) {
    size_t text_len;
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    const char *text = luaL_checklstring(L, 2, &text_len);
    pango_layout_set_text(*obj, text, text_len);
    return 0;
}

#define PROPERTY(name) \
    { "set_" #name, font_desc_set_ ## name }, \
    { "get_" #name, font_desc_get_ ## name }

static const luaL_Reg
layout_methods[] = {
    { "__gc", layout_gc },
    PROPERTY(width),
    PROPERTY(height),
    PROPERTY(ellipsize),
    PROPERTY(wrap),
    PROPERTY(alignment),
    PROPERTY(justify),
    { "set_font_description", layout_set_font_description },
    { "get_font_description", layout_get_font_description },
    { "get_extents", layout_get_extents },
    { "get_pixel_extents", layout_get_pixel_extents },
    { "get_size", layout_get_size },
    { "get_pixel_size", layout_get_pixel_size },
    { "set_markup", layout_set_markup },
    { "set_text", layout_set_text },
    { 0, 0 }
};
#undef PROPERTY

/* vi:set ts=4 sw=4 expandtab: */
