#ifndef B_NODE_STASH_H
#define B_NODE_STASH_H

#include "b_node.h"
#include "b_block.h"

#include <map>
#include <string>
#include <list>
#include <vector>
#include <memory>

namespace bster{

class b_node_stash
{
public:
    b_node_stash();

    void addNode(b_node node);
    int getCountByNodeType(std::string type);
	bool verifyNodes();
    //b_node* getNodePtrById(std::string id);
    //std::list<b_node*> getNodePtrListByType(std::string type);
	std::vector<std::shared_ptr<b_node>> v_nodes; //vector of all nodes

	static bool verify(std::shared_ptr<b_node>& node);//check that node is complete for next step
	

private:
    std::map<std::string,int> map_nodetype_to_ncount; //maps path to template to number of added nodes of that template
    //std::map<std::string,std::list<b_node*>> map_nodetype_to_nodeptrlist; //maps nodetype to a list of pointers to all nodes of that type
    //std::map<std::string, b_node*> map_nodeid_to_nodeptr; //maps node id to the pointer to that node
    
};

}
#endif // B_NODE_STASH_H
