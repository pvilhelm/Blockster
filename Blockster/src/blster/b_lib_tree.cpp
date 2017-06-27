#include "b_lib_tree.h"

#include <regex>
#include "boost\filesystem.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

bster::b_lib_tree::b_lib_tree()
{
}


bster::b_lib_tree::~b_lib_tree()
{
}

void bster::b_lib_tree::parseFolderTree(std::string str_path, bool is_root)
{
    using namespace boost::filesystem;

    path p_dir(str_path);
    p_dir.normalize();

    if (!is_directory(p_dir)) {
        throw std::runtime_error("Path is not a valid directory " + p_dir.string());
    }

    //The root dir on which the traversal began
    static thread_local path root_path;

    if (is_root) {
        this->root_dir = p_dir.generic_string();//sets root_dir
        root_path = p_dir;
    }



    for (auto i : directory_iterator(p_dir)) {

        path p = i.path();
        p.normalize();

        //if p is a dir, walk it
        if (is_directory(p)) parseFolderTree(p.generic_string());

        //check if its a file at p
        if (is_regular_file(p)) {
            //check if the file follows the pattern /(parent)/template_node_(parent).xml
            path parent = p.parent_path();
            std::string parent_name = parent.filename().generic_string();
            std::string comp = "template_node_" + parent_name + ".xml";
            if (p.filename().string() == comp) {
                //read the xml str and store it together with the files path from root in
                //a map
                fstream fs;
                fs.open(p);
                if (!fs.is_open()) {
                    throw std::runtime_error("Could not open file " + p.string());
                }
                std::stringstream ss;
                ss << fs.rdbuf();
                std::string xml_str = ss.str();
                path rel_path = p.lexically_relative(root_path);
                map_libpath_to_xml_str[rel_path.generic_string()] = xml_str;

                fs.close();
            }
        }
    }
}
