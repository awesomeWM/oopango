require "lunit"

local Pango = require "oopango"

module("test.context", lunit.testcase, package.seeall)

function test_flags()
	assert_number(Pango.PANGO_SCALE)
	assert_equal("pango", Pango._NAME)
	assert_string(Pango._VERSION)
	assert_string(Pango._PANGO_VERSION)
end
