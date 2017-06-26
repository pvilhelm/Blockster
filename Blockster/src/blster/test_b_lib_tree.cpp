#include "catch.hpp"
 
#include "b_lib_tree.h"

using namespace bster;

TEST_CASE("Test b_lib_tree", "[std]") {

    std::string path_to_blocks = R"(C:\Repos\Blockster\Blockster\blocks)";

    b_lib_tree lt;

    lt.parseFolderTree(path_to_blocks,true);//sets root_dir

    REQUIRE(lt.root_dir == "C:/Repos/Blockster/Blockster/blocks");//assert nice /:s 
    REQUIRE(lt.map_libpath_to_xml_str.size() > 0);//atleast something got added ... 
    REQUIRE_THROWS(lt.parseFolderTree(R"(/Hopefully/Doesnt/Exist/Folderqeqwe)"));//throws on no found
}