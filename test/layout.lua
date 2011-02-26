require "lunit"

local Pango = require "oopango"
local Cairo = require "oocairo"

module("test.layout", lunit.testcase, package.seeall)

function setup ()
	surface = Cairo.image_surface_create("rgb24", 100, 100)
	cr = Cairo.context_create(surface)
	layout = Pango.cairo.layout_create(cr)
end
function teardown ()
	surface = nil
	cr = nil
	layout = nil
end

local function test_rect(rect)
	assert_table(rect)
	for k, v in pairs({ "x", "y", "width", "height" }) do
		assert_number(rect[v])
		assert_true(rect[v] >= 0)
	end
end

function test_double_gc()
	layout:__gc()
	layout:__gc()
end

function test_properties()
	assert_number(layout:get_width())
	assert_number(layout:get_height())
	assert_string(layout:get_ellipsize())
	assert_string(layout:get_wrap())
	assert_string(layout:get_alignment())
	assert_boolean(layout:get_justify())
	assert_boolean(layout:get_single_paragraph_mode())

	layout:set_width(100)
	layout:set_height(10)

	for k, v in pairs({ "justify", "single_paragraph_mode" }) do
		local get = "get_" .. v
		local set = "set_" .. v
		assert_boolean(layout[get](layout))
		for k, b in pairs({ true, false }) do
			layout[set](layout, b)
			assert_equal(layout[get](layout, b), b)
		end
	end

	for k, v in pairs({ "none", "start", "middle", "end" }) do
		layout:set_ellipsize(v)
		assert_equal(v, layout:get_ellipsize())
	end
	for k, v in pairs({ "word", "char", "word_char" }) do
		layout:set_wrap(v)
		assert_equal(v, layout:get_wrap())
	end
	for k, v in pairs({ "left", "right", "center" }) do
		layout:set_alignment(v)
		assert_equal(v, layout:get_alignment())
	end
end

function test_empty_layout()
	Pango.cairo.update_layout(cr, layout)
	Pango.cairo.show_layout(cr, layout)
	Pango.cairo.layout_path(cr, layout)
	local x1,y1,x2,y2 = cr.path_extents(cr)
	assert_equal(x1, 0)
	assert_equal(y1, 0)
	assert_equal(x2, 0)
	assert_equal(y2, 0)
end

function test_set_text()
	layout:set_text("")
	layout:set_text("text")
	layout:set_text("text with <valid non-markup<")
	assert_error(function() layout:set_markup(">invalid<") end)
	assert_pass(function() layout:set_markup("<b>valid</b>") end)
	assert_equal(layout:get_unknown_glyphs_count(), 0)
end

function test_font_desc()
	local desc = Pango.font_description_from_string("MyFont Italic 12")
	assert_equal(nil, layout:get_font_description())
	layout:set_font_description(desc)
	assert_equal(desc, layout:get_font_description())
end

function test_size()
	local function test(func)
		local w, h = layout[func](layout)
		assert_number(w)
		assert_number(h)
	end
	test("get_size")
	test("get_pixel_size")
end

function test_extents()
	local function test(func)
		local ink, logical = layout[func](layout)
		test_rect(ink)
		test_rect(logical)
	end
	test("get_extents")
	test("get_pixel_extents")
end

function test_index()
	layout:set_text("This is some random text.")

	test_rect(layout:index_to_pos(0))
	test_rect(layout:index_to_pos(5))
	test_rect(layout:index_to_pos(1000))

	local pos = layout:index_to_pos(10)
	local res, idx, trail = layout:xy_to_index(pos.x, pos.y)
	-- The coordinates should be valid
	assert_equal(true, res)
	assert_equal(10, idx)
	assert_equal(0, trail)

	local res, idx, trail = layout:xy_to_index(-100, -30)
	-- The coordinates should be invalid
	assert_equal(false, res)
	assert_equal(0, idx)
	assert_equal(0, trail)

	local line, x = layout:index_to_line_x(5)
	assert_number(line)
	assert_number(x)
	assert_true(line >= 0)
	assert_true(x >= 0)
end

function test_cursor()
	layout:set_text("Yet another random text.")

	local strong, weak = layout:get_cursor_pos(5)
	test_rect(strong)
	test_rect(weak)
	assert_equal(0, strong.y)
	assert_equal(0, weak.y)

	local n_idx, n_trail = layout:move_cursor_visually(true, 3, 0, true)
	assert_equal(4, n_idx)
	assert_equal(0, n_trail)
	local n_idx, n_trail = layout:move_cursor_visually(true, 3, 0, false)
	assert_equal(2, n_idx)
	assert_equal(0, n_trail)
end
