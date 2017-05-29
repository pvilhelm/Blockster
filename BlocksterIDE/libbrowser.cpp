#include "libbrowser.h"
#include "ui_libbrowser.h"
#include <QDrag>
#include "block.h"
#include <QtDebug>
#include <QRegExp>

extern BlocksterSession blocksterS;

LibBrowser::LibBrowser(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::LibBrowser)
{
    ui->setupUi(this);
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setWindowTitle("Library browser");
    root_scene = new LibScene(this);
    root_view = new QGraphicsView(root_scene,this);

    this->ui->horizontalLayout->addWidget(root_view);
    //root_scene->addBlock("");

    root_view->setObjectName(QStringLiteral("rootView"));
    ParseBlocks();


    //Add each block to libbrowser
    for(QString path : libpaths_list){
        root_scene->addBlock(path);
    }
    qDebug() << "LibBrowser is set up" << __LINE__ <<":"<< __FILE__;
}

LibBrowser::~LibBrowser()
{
    delete ui;
}

void LibBrowser::ParseBlocks()
{
    QString root_libpath = blocksterS.config.confParHashtable->value("BLOCKSTER_BLOCKLIBS_PATH");
    if(root_libpath ==""){
        throw std::runtime_error("BLOCKSTER_BLOCKLIBS_PATH missing in config.txt");
    }
    this->lib_path = root_libpath;

    ParseFolder(root_libpath);
    qDebug() << "Parsed"<<libpaths_list.count()<<"blocks into LibBrowser"<<__LINE__<<":"<<__FILE__;
}

void LibBrowser::ParseFolder(QString folder_path){
    QDir dir(folder_path);
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks);

    for(QString folder : dir.entryList()){
        if(folder == ".." || folder == ".")
            continue;
        ParseFolder(folder_path+"/"+folder);
    }

    QString folder_name = dir.dirName();
    QDir dir_files(folder_path);
    dir_files.setFilter(QDir::Files | QDir::NoSymLinks);
    for(QString file : dir_files.entryList()){
        QRegExp is_block_template("^template_node_"+folder_name+".xml$");

        if(is_block_template.exactMatch(file)){
            this->libpaths_list.append(folder_path+"/"+file);
        }
    }

}
