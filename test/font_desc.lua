require "lunit"

local Pango = require "oopango"

module("test.font_desc", lunit.testcase, package.seeall)

function setup ()
	desc = Pango.font_description_new()
end
function teardown ()
	desc = nil
end

function test_double_gc()
	desc:__gc()
	desc:__gc()
end

function test_eq()
	local desc2 = Pango.font_description_from_string("MyFont Italic 12")
	desc:set_size(Pango.units_from_number(12))
	desc:set_family("MyFont")
	desc:set_style("italic")
	assert_equal(desc, desc2)
end

function test_properties()
	assert_string(desc:__tostring())
	assert_equal(desc, Pango.font_description_new())

	-- Unset family is nil, so set one first
	desc:set_family("MyFamily")
	assert_equal("MyFamily", desc:get_family())
	assert_string(desc:get_family())

	assert_string(desc:get_style())
	assert_string(desc:get_variant())
	assert_string(desc:get_weight())
	assert_string(desc:get_stretch())
	assert_string(desc:get_gravity())
	assert_number(desc:get_size())
	assert_boolean(desc:get_size_is_absolute())

	desc:set_absolute_size(Pango.units_from_number(10))
	assert_equal(10, Pango.units_to_number(desc:get_size()))
	assert_equal(true, desc:get_size_is_absolute())

	desc:set_size(Pango.units_from_number(10))
	assert_equal(10, Pango.units_to_number(desc:get_size()))
	assert_equal(false, desc:get_size_is_absolute())
end

function test_property_values_style()
	for k, v in pairs({ "normal", "oblique", "italic" }) do
		desc:set_style(v)
		assert_equal(v, desc:get_style())
	end
end

function test_property_values_variant()
	for k, v in pairs({ "normal", "small-caps" }) do
		desc:set_variant(v)
		assert_equal(v, desc:get_variant())
	end
end

function test_property_values_weight()
	for k, v in pairs({ "thin", "ultralight", "light", "book", "normal",
			"medium", "semibold", "bold", "ultrabold", "heavy", "ultraheavy" }) do
		desc:set_weight(v)
		assert_equal(v, desc:get_weight())
	end
end

function test_property_values_stretch()
	for k, v in pairs({ "ultra-condensed", "extra-condensed", "condensed", "semi-condensed", "normal",
			"semi-expanded", "expanded", "extra-expanded", "ultra-expanded" }) do
		desc:set_stretch(v)
		assert_equal(v, desc:get_stretch())
	end
end

function test_property_values_gravity()
	for k, v in pairs({ "south", "east", "north", "west" }) do
		desc:set_gravity(v)
		assert_equal(v, desc:get_gravity())
	end

	-- "auto" unsets the gravity masks and thus needs special handling
	desc:set_gravity("west")
	desc:set_gravity("auto")
	assert_equal("south", desc:get_gravity())
end
