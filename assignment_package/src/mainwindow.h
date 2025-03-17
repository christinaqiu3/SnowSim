#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtWidgets/qlistwidget.h"
#include "scene/mesh.h"
#include <QMainWindow>
#include <QTreeWidget>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    QListWidget *vertsListWidget;
    QListWidget *halfEdgesListWidget;
    QListWidget *facesListWidget;
    QTreeWidget *jointsTreeWidget;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    vertex* splitEdge(halfedge *e, bool smooth, std::unordered_map<face*, vertex*> faceCentroids, int numOrigHalfEdges);
    void updateDisplayRot();


signals:
    void sig_sendVert(QListWidgetItem *selectedItem);
    void sig_sendHalfEdge(QListWidgetItem *selectedItem);
    void sig_sendFace(QListWidgetItem *selectedItem);
    void sig_sendJoint(QTreeWidgetItem *selectedItem);


public slots:
    void on_actionQuit_triggered();
    void on_chooseOBJ_clicked();
    void on_addMidpoint_clicked();
    void on_triangulateFace_clicked();
    void on_catmullClark_clicked();
    void on_loadJSON_clicked();
    void on_exportUSD_clicked();
    void on_changeColor_clicked();

    void slot_addRootToTreeWidget(QTreeWidgetItem*);

    void on_jointRotXPlus_clicked();
    void on_jointRotXMinus_clicked();
    void on_jointRotYPlus_clicked();
    void on_jointRotYMinus_clicked();
    void on_jointRotZPlus_clicked();
    void on_jointRotZMinus_clicked();
    void on_meshSkinning_clicked();

    void on_MPM_clicked();


    std::unordered_map<halfedge*, vertex*> smoothEdgeMidpoints(std::unordered_map<face*, vertex*> faceCentroids, int numOrigHalfEdges);
    void smoothOriginalVertices(std::unordered_map<face*, vertex*> faceCentroids, int numOrigVerts);
    void Quadrangulate(face* originalFace, vertex* centroid);

    void on_actionCamera_Controls_triggered();
    void populateLists();
    void updateMeshComponentWidgetV(QListWidgetItem *selectedItem);
    void updateMeshComponentWidgetH(QListWidgetItem *selectedItem);
    void updateMeshComponentWidgetF(QListWidgetItem *selectedItem);
    void updateJointComponentWidget(QTreeWidgetItem *selectedItem);

private:
    Ui::MainWindow *ui;

};


#endif // MAINWINDOW_H
