static int
font_desc_gc(lua_State *L) {
    PangoFontDescription **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_DESC);
    pango_font_description_free(*obj);
    *obj = 0;
    return 0;
}

static const luaL_Reg
font_desc_methods[] = {
    { "__gc", font_desc_gc },
    { 0, 0 }
};

/* vi:set ts=4 sw=4 expandtab: */
