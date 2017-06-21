#include "b_lib_tree.h"

#include <regex>
#include <fstream>
#include "boost\filesystem.hpp"



bster::b_lib_tree::b_lib_tree()
{
}


bster::b_lib_tree::~b_lib_tree()
{
}

void bster::b_lib_tree::parseFolderTree(std::string str_path, bool is_root)
{
    using namespace boost::filesystem;
    
    path p(str_path);
    p.normalize();

     
}
