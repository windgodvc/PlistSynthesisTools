//
//  MainWindow.cpp
//  PlistSynthesisTools
//
//  Created by Windgod on 4/28/16.
//
//


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <iostream>

#include <QStandardItemModel>

enum class PlistType{
    String = 0,
    Int = 1,
    Double = 2,
    Dic = 3,
    Array = 4,
    Data = 5,
    Date = 6,
    Bool = 7
};

using namespace Plist;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("PlistSynthesisTools");
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI(){
    connect(addToolAction("&open file","open.png","open plist file."),SIGNAL(triggered()),this,SLOT(open()));
    connect(addToolAction("&remove All","delete.png","remove all content."),SIGNAL(triggered()),this,SLOT(removeAll()));
    connect(addToolAction("&publish","publish.png","publish"),SIGNAL(triggered()),this,SLOT(publish()));

    QApplication::setStyle("fusion");

    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->treeWidget->setAcceptDrops(true);
    setAcceptDrops(true);
    ui->treeWidget->setColumnWidth(0,160);
    ui->treeWidget->setAlternatingRowColors(true);

}

QAction* MainWindow::addToolAction(const char * action,const char *fileName,const char *describe){
    QAction* openAction = new QAction(action, this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr(describe));
    std::string str = std::string(":/res/") + fileName;
    openAction->setText(action);
    openAction->setIcon(QIcon(str.c_str()));
    ui->mainToolBar->addAction(openAction);
    return openAction;

}

PlistType getPlistType(const std::type_info &info){
    PlistType type;
    if(info == typeid(string_type)){
        type = PlistType::String;
    }else if(info == typeid(integer_type)){
        type = PlistType::Int;
    }else if(info == typeid(real_type)){
        type = PlistType::Double;
    }else if(info == typeid(dictionary_type)){
        type = PlistType::Dic;
    }else if(info == typeid(array_type)){
        type = PlistType::Array;
    }else if(info == typeid(date_type)){
        type = PlistType::Date;
    }else if(info == typeid(data_type)){
        type = PlistType::Data;
    }else if(info == typeid(boolean_type)){
        type = PlistType::Bool;
    }
    return type;
}


void MainWindow::traverse(Plist::dictionary_type map,QTreeWidgetItem* item,bool is_addfirst){
    if (map.empty()) {
        return ;
    }
    QString str;
    for (auto it = map.begin();it != map.end();it++) {

        auto itor = *it;
        const std::type_info & type = itor.second.type();

        switch(getPlistType(type)){
        case PlistType::String:

            if(is_addfirst){
               AddTreeNode(item,itor.first.c_str(),"String",boost::any_cast<std::string>(itor.second).c_str());
            }else{
               AddTreeNode(item,itor.first.c_str(),"String",boost::any_cast<std::string>(itor.second).c_str());
            }

            break;
        case PlistType::Int:
            str.sprintf("%zd",boost::any_cast<integer_type>(itor.second));
            AddTreeNode(item,itor.first.c_str(),"Number",str.toStdString());
            break;
        case PlistType::Double:
            str.sprintf("%f",boost::any_cast<real_type>(itor.second));
            AddTreeNode(item,itor.first.c_str(),"Double",str.toStdString());
            break;
        case PlistType::Dic:{
            auto dic =  boost::any_cast<dictionary_type>(itor.second);
            str.sprintf("(%d Items)",dic.size());
            auto root = AddTreeNode(item,itor.first,"Dictionary",str.toStdString());

            traverse(dic,root,false);
        }
            break;
        case PlistType::Array:{
            auto v = boost::any_cast<array_type>(itor.second);
            str.sprintf("(%d Items)",v.size());
            auto root = AddTreeNode(item,itor.first,"Array",str.toStdString());
            addArray(v,root);
        }
            break;
        case PlistType::Data:

        {
            auto v = boost::any_cast<data_type>(itor.second);
            std::string s(v.begin(),v.end());

            AddTreeNode(item,itor.first.c_str(),"Data",s);
        }
            break;
        case PlistType::Date:

        {
            auto date = boost::any_cast<date_type>(itor.second);
            AddTreeNode(item,itor.first.c_str(),"Date",date.timeAsXMLConvention());
        }
            break;
        case PlistType::Bool:
            str.sprintf("%d",boost::any_cast<boolean_type>(itor.second));
            AddTreeNode(item,itor.first.c_str(),"Boolean",str.toStdString());

            break;
        }

    }
}

void MainWindow::addArray(Plist::array_type map, QTreeWidgetItem *parent)
{


    QString str;
    int i = 0;

    for (auto &itor:map) {
        str.sprintf("Item%d",i++);
        const std::type_info & type = itor.type();

        switch(getPlistType(type)){
        case PlistType::String:
               AddTreeNode(parent,str.toStdString().c_str(),"String",boost::any_cast<std::string>(itor).c_str());
            break;
        case PlistType::Int:
            str.sprintf("%zd",boost::any_cast<integer_type>(itor));
            AddTreeNode(parent,str.toStdString().c_str(),"Number",str.toStdString());
            break;
        case PlistType::Double:
            str.sprintf("%f",boost::any_cast<real_type>(itor));
            AddTreeNode(parent,str.toStdString().c_str(),"Double",str.toStdString());
            break;
        case PlistType::Dic:{
            auto dic =  boost::any_cast<dictionary_type>(itor);
            QString str2;
            str2.sprintf("(%d Items)",dic.size());

            auto root = AddTreeNode(parent,str.toStdString().c_str(),"Dictionary",str2.toStdString());

            traverse(boost::any_cast<dictionary_type>(itor),root,false);
        }
            break;
        case PlistType::Array:{
            auto v = boost::any_cast<array_type>(itor);
            QString str2;
            str2.sprintf("(%d Items)",v.size());
            auto root = AddTreeNode(parent,str.toStdString().c_str(),"Array",str2.toStdString());
            addArray(v,root);
        }
            break;
        case PlistType::Data:

        {
            auto v = boost::any_cast<data_type>(itor);
            std::string s(v.begin(),v.end());
            AddTreeNode(parent,str.toStdString().c_str(),"Data",s);
        }
            break;
        case PlistType::Date:
        {
            auto date = boost::any_cast<date_type>(itor);
            AddTreeNode(parent,str.toStdString().c_str(),"Date",date.timeAsXMLConvention());
        }
            break;
        case PlistType::Bool:
            str.sprintf("%d",boost::any_cast<boolean_type>(itor));
            AddTreeNode(parent,str.toStdString().c_str(),"Boolean",str.toStdString());
            break;
        }

    }

}


void MainWindow::open()
{
    std::string filename = getOpenNameDialog(
                this, "open plist file",
                "",
                "text files (*.plist)").toStdString();

    if(filename.empty()) return ;



    open(filename);

}

void MainWindow::open(std::string file)
{
    if(file.find(".plist") == std::string::npos){
        QMessageBox::information(NULL,"格式不支持","请使用plist 文件打开");
        return ;
    }

    using namespace Plist;
    dictionary_type dic;

    readPlist(file.c_str(),dic);

    QString str;
    str.sprintf("(%d Items)",dic.size());
    traverse(dic,AddTreeRoot("Root","Dictionary",str.toStdString()),true);
    _vesselDic.insert(dic.begin(),dic.end());
    /*for(auto &itor:dic){
        _vesselDic[itor.first] = itor.second;
    }*/
}

void MainWindow::removeAll()
{
    ui->treeWidget->clear();
    std::map<std::string, boost::any>().swap(_vesselDic);
    _vesselDic.clear();
}

void MainWindow::publish()
{
    std::string filename = getSaveNameDialog(
                this, "save plist file",
                "",
                "text files (*.plist)").toStdString();
    if(filename.empty()) return ;



    using namespace Plist;

    writePlistXML(filename.c_str(),_vesselDic);

}

QTreeWidgetItem *MainWindow::AddTreeNode(QTreeWidgetItem *parent, std::string name,std::string type,std::string value)
{
    QTreeWidgetItem * item=new QTreeWidgetItem(QStringList()<<name.c_str()<<type.c_str()<<value.c_str());
    parent->addChild(item);
    return item;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
            event->acceptProposedAction();
        }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
        if (urls.isEmpty()) {
            return;
        }

        QString fileName = urls.first().toLocalFile();
        if (fileName.isEmpty()) {
            return;
        }

   for(auto &itor:urls){
        open(itor.toLocalFile().toStdString()) ;
   }
   setWindowTitle(tr("%1 - %2").arg(fileName, tr("PlistSynthesisTools")));

}

QTreeWidgetItem *MainWindow::AddTreeRoot(std::string name,std::string type,std::string value)
{
    QTreeWidgetItem * item=new QTreeWidgetItem(QStringList()<<name.c_str()<<type.c_str()<<value.c_str());
    ui->treeWidget->addTopLevelItem(item);
    return item;
}
