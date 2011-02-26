require "lunit"

local Pango = require "oopango"
local Cairo = require "oocairo"

module("test.font_map", lunit.testcase, package.seeall)

function setup ()
	map = Pango.cairo.font_map_get_default()
end
function teardown ()
	map = nil
end

function test_type()
	assert_equal(map.type(), "PangoFontMap")
end

function test_double_gc()
	map:__gc()
	map:__gc()
end

local function test_font_face(face)
	assert_table(face)
	assert_string(face.name)
	-- TODO test if face.description is font desc
	--test_font_desc(face.description)
	assert_string(face.description:__tostring())
	assert_number(face.description:get_size())
end

local function test_font_family(family)
	assert_table(family)
	assert_string(family.name)
	assert_boolean(family.monospace)
	assert_table(family.faces)
	assert_not_equal(#family.faces, 0)
	for k, v in pairs(family.faces) do
		test_font_face(v)
	end
end

function test_list_families()
	local found_sans = false
	local families = map:list_families()
	assert_table(families)
	assert_not_equal(#families, 0)
	for k, v in pairs(families) do
		test_font_family(v)
		if v.name == "Sans" then
			found_sans = true
		end
	end
	assert_true(found_sans)
end
