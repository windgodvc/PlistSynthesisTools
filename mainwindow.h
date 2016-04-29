//
//  MainWindow.h
//  PlistSynthesisTools
//
//  Created by Windgod on 4/28/16.
//
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QDragEnterEvent>
#include <QtGui>

#include "FileUtils/Plist.hpp"


inline QString getOpenNameDialog(QWidget * parent = 0,
                                 const QString & tile = QString(),
                                 const QString & dir = QString(),
                                 const QString & filter = QString(),
                                 QString * selectedFilter = NULL,
                                 QFileDialog::Options options = 0){
    return QFileDialog::getOpenFileName(
                    parent, tile,
                    dir,
                    filter);
}

inline QString getSaveNameDialog(QWidget * parent = 0,
                                 const QString & tile = QString(),
                                 const QString & dir = QString(),
                                 const QString & filter = QString(),
                                 QString * selectedFilter = NULL,
                                 QFileDialog::Options options = 0){
    return QFileDialog::getSaveFileName(
                    parent, tile,
                    dir,
                    filter);
}


/*
 *
 * typedef std::string                          string_type;
    typedef int64_t                              integer_type;
    typedef double                               real_type;
    typedef std::map<std::string, boost::any>    dictionary_type;
    typedef std::vector<boost::any>              array_type;
    typedef Date                                 date_type;
    typedef std::vector<char>                    data_type;
    typedef bool                                 boolean_type;
 */


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void initUI();

    QAction* addToolAction(const char * action,const char *fileName,const char *describe);

    void traverse(Plist::dictionary_type map,QTreeWidgetItem *item,bool is_addfirst);

    void addArray(Plist::array_type map,QTreeWidgetItem *parent);

public slots:

    void open();

    void open(std::string file);

    void removeAll();

    void publish();

    QTreeWidgetItem * AddTreeRoot(std::string name,std::string type,std::string value);


    QTreeWidgetItem * AddTreeNode(QTreeWidgetItem *parent,std::string name,std::string type,std::string value);

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    Ui::MainWindow *ui;

    Plist::dictionary_type _vesselDic;
};

#endif // MAINWINDOW_H
