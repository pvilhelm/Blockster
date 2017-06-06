#include "b_task.h"



bster::b_task::b_task(int task_id)
{
	this->task_id = task_id;
}


bster::b_task::~b_task()
{
}

void bster::b_task::addNode(b_node node)
{
	this->v_nodes.push_back(node);
}

void bster::b_task::processAllNodes()
{

}
