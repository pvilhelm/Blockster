#include "lib_scene.h"

LibScene::LibScene(std::string lib_path)
{
    this->lib.parseFolderTree(lib_path);
}

void LibScene::reparseLib()
{
    this->lib.parseFolderTree(lib.root_dir);
}
