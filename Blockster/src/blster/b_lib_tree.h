#pragma once

#include <map>

namespace bster {
    class b_lib_tree
    {
    public:
        b_lib_tree();
        ~b_lib_tree();
        
        std::map<std::string, std::string> map_libpath_to_xml_str;

        void parseFolderTree(std::string path, bool is_root = true);

    };
}
