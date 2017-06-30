
#include "catch.hpp"
#include "b_meta_signal.h"

using namespace bster;

TEST_CASE("Test b_meta_signal()", "[std]") {

	SECTION("Constructor") {
		REQUIRE_NOTHROW(b_meta_signal());
	}
}