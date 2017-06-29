#include "blockster_session.h"


#include "lib_container.h"
#include "program_container.h"
#include "b_lib_tree.h"
#include "b_xml.h"

BlocksterSession::BlocksterSession()
{
    QHBoxLayout* hb = new QHBoxLayout();
    this->setLayout(hb);
    this->setObjectName("session");
}

void BlocksterSession::addLibBrowser()
{
    LibContainer* lib_c = new LibContainer();
    lib_c->setObjectName("lib_browser");
    this->layout()->addWidget(lib_c);
}

void BlocksterSession::addProgram()
{
    ProgramContainer* p_c = new ProgramContainer();
    p_c->setObjectName("program");
    this->layout()->addWidget(p_c);
}

void BlocksterSession::addProgram(std::string path_to_xml)
{
    ProgramContainer* p_c = new ProgramContainer(path_to_xml);
    p_c->setObjectName("program");
    this->layout()->addWidget(p_c);
}
