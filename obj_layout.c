static int
layout_gc(lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    g_object_unref(*obj);
    *obj = 0;
    return 0;
}

static int
layout_set_width(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    pango_layout_set_width(*obj, luaL_checknumber(L, 2));
    return 0;
}

static int
layout_get_width(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    lua_pushnumber(L, pango_layout_get_width(*obj));
    return 1;
}

static int
layout_set_height(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    pango_layout_set_height(*obj, luaL_checknumber(L, 2));
    return 0;
}

static int
layout_get_height(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    lua_pushnumber(L, pango_layout_get_height(*obj));
    return 1;
}

static int
layout_set_ellipsize(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    pango_layout_set_ellipsize(*obj, ellipsize_mode_from_lua(L, 2));
    return 0;
}

static int
layout_get_ellipsize(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    return ellipsize_mode_to_lua(L, pango_layout_get_ellipsize(*obj));
}

static int
layout_set_wrap(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    pango_layout_set_wrap(*obj, wrap_from_lua(L, 2));
    return 0;
}

static int
layout_get_wrap(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    return wrap_to_lua(L, pango_layout_get_wrap(*obj));
}

static int
layout_set_alignment(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    pango_layout_set_alignment(*obj, alignment_from_lua(L, 2));
    return 0;
}

static int
layout_get_alignment(lua_State *L) {
    PangoLayout **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_LAYOUT);
    return alignment_to_lua(L, pango_layout_get_alignment(*obj));
}

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

static const luaL_Reg
layout_methods[] = {
    { "__gc", layout_gc },
    { "set_width", layout_set_width },
    { "get_width", layout_get_width },
    { "set_height", layout_set_height },
    { "get_height", layout_get_height },
    { "set_ellipsize", layout_set_ellipsize },
    { "get_ellipsize", layout_get_ellipsize },
    { "set_wrap", layout_set_wrap },
    { "get_wrap", layout_get_wrap },
    { "set_alignment", layout_set_alignment },
    { "get_alignment", layout_get_alignment },
    { "get_extents", layout_get_extents },
    { "get_pixel_extents", layout_get_pixel_extents },
    { "get_size", layout_get_size },
    { "get_pixel_size", layout_get_pixel_size },
    { "set_font_description", layout_set_font_description },
    { "set_markup", layout_set_markup },
    { "set_text", layout_set_text },
    { 0, 0 }
};

/* vi:set ts=4 sw=4 expandtab: */
