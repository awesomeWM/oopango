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
font_map_gc(lua_State *L) {
    PangoFontMap **obj = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_MAP);
    g_object_unref(*obj);
    *obj = 0;
    return 0;
}

static int
font_map_list_families(lua_State *L)
{
    PangoFontFamily **families;
    int num;
    PangoFontMap **map = luaL_checkudata(L, 1, OOPANGO_MT_NAME_FONT_MAP);
    pango_font_map_list_families(*map, &families, &num);

    lua_newtable(L);
    for (int i = 0; i < num; i++)
    {
        PangoFontFamily *font = families[i];

        lua_pushnumber(L, lua_objlen(L, -1) + 1);
        lua_newtable(L);

        lua_pushstring(L, "name");
        lua_pushstring(L, pango_font_family_get_name(font));
        lua_rawset(L, -3);

        lua_pushstring(L, "monospace");
        lua_pushboolean(L, pango_font_family_is_monospace(font));
        lua_rawset(L, -3);

        lua_pushstring(L, "faces");
        lua_newtable(L);

        PangoFontFace **faces;
        int n_faces;
        pango_font_family_list_faces(font, &faces, &n_faces);
        for (int n = 0; faces && n < n_faces; n++)
        {
            PangoFontFace *face = faces[n];

            lua_pushnumber(L, lua_objlen(L, -1) + 1);
            lua_newtable(L);

            lua_pushstring(L, "name");
            lua_pushstring(L, pango_font_face_get_face_name(face));
            lua_rawset(L, -3);

            lua_pushstring(L, "description");
            PangoFontDescription **obj = create_font_desc_userdata(L);
            *obj = pango_font_face_describe(face);
            lua_rawset(L, -3);

            /* This adds the current face to the list of faces */
            lua_rawset(L, -3);
        }
        g_free(faces);

        /* This saves "faces" in the table for this family */
        lua_rawset(L, -3);

        /* This saves the table for this font family in our returned table */
        lua_rawset(L, -3);
    }

    g_free(families);
    return 1;
}

static int
font_map_type(lua_State *L) {
    lua_pushstring(L, "PangoFontMap");
    return 1;
}

static const luaL_Reg
font_map_methods[] = {
    { "__gc", font_map_gc },
    { "list_families", font_map_list_families },
    { "type", font_map_type },
    { 0, 0 }
};
