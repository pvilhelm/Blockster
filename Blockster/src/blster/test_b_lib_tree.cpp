#include "catch.hpp"
 
#include "b_lib_tree.h"

using namespace bster;

TEST_CASE("Test b_lib_tree", "[std]") {

    std::string path_to_blocks = R"(C:\Repos\Blockster\Blockster\blocks)";

    b_lib_tree lt;

    lt.parseFolderTree(path_to_blocks);

}