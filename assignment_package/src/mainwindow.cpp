#include "mainwindow.h"
#include <iostream>
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <QFileDialog>
#include <QJsonDocument>
#include "mygl.h"
#include "scene/mesh.h"
#include "drawable.h"
#include "smartpointerhelp.h"
#include <QTreeWidget>

#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>

#include "MPMSolver.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    vertsListWidget(new QListWidget(this)),
    halfEdgesListWidget(new QListWidget(this)),
    facesListWidget(new QListWidget(this)),
    jointsTreeWidget(new QTreeWidget(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();
    //populateLists(ui->mygl->m_mesh.getVertss().size(), ui->mygl->m_mesh.getHalfEdgess().size(), ui->mygl->m_mesh.getFacess().size());

    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(updateMeshComponentWidgetV(QListWidgetItem*)));
    connect(ui->halfEdgesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(updateMeshComponentWidgetH(QListWidgetItem*)));
    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(updateMeshComponentWidgetF(QListWidgetItem*)));
    connect(ui->jointsTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(updateJointComponentWidget(QTreeWidgetItem*)));

    connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setVX(double)));
    connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setVY(double)));
    connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setVZ(double)));

    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setB(double)));
    connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setG(double)));
    connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setR(double)));

    connect(ui->jointPosXSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setJX(double)));
    connect(ui->jointPosYSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setJY(double)));
    connect(ui->jointPosZSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setJZ(double)));


    ///STYLING
    //    ui->txSpinBox->setStyleSheet("QDoubleSpinBox { background-color: #003f9e; }");
    //    ui->tySpinBox->setStyleSheet("QDoubleSpinBox { background-color: #003f9e; }");

    //    ui->sxSpinBox->setStyleSheet("QDoubleSpinBox { background-color: #a30070; }");
    //    ui->sySpinBox->setStyleSheet("QDoubleSpinBox { background-color: #a30070; }");

    //    ui->rSpinBox->setStyleSheet("QDoubleSpinBox { background-color: #5ba100; }");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::on_chooseOBJ_clicked()
{
    QString filename = QFileDialog::getOpenFileName(0, QString("Load Scene File"), QDir::currentPath().append(QString("../..")), QString("*.obj"));
    int i = filename.length() - 1;
    while(QString::compare(filename.at(i), QChar('/')) != 0)
    {
        i--;
    }
    QString local_path = filename.left(i+1);

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning("Could not open the OBJ file.");
        return;
    }
    QByteArray file_data = file.readAll();

    ui->mygl->read = true;
    ui->mygl->on_loadButton_clicked();

    vertsListWidget->clear();
    for (int i = 0; i < (int) ui->mygl->m_mesh.getVertss().size(); ++i) {
        QListWidgetItem *itmv = ui->mygl->m_mesh.getVertss()[i];
        ui->vertsListWidget->addItem(itmv);
        ui->vertsListWidget->item(i)->setText(QString::number(ui->mygl->m_mesh.getVertss()[i]->ind));
    }
    halfEdgesListWidget->clear();
    for (int i = 0; i < (int) ui->mygl->m_mesh.getHalfEdgess().size(); ++i) {
        QListWidgetItem *itmhe = ui->mygl->m_mesh.getHalfEdgess()[i];
        ui->halfEdgesListWidget->addItem(itmhe);
        ui->halfEdgesListWidget->item(i)->setText(QString::number(ui->mygl->m_mesh.getHalfEdgess()[i]->ind));
    }
    facesListWidget->clear();
    for (int i = 0; i < (int) ui->mygl->m_mesh.getFacess().size(); ++i) {
        QListWidgetItem *itmf = ui->mygl->m_mesh.getFacess()[i];
        emit sig_sendVert(itmf);
        ui->facesListWidget->addItem(itmf);
        ui->facesListWidget->item(i)->setText(QString::number(ui->mygl->m_mesh.getFacess()[i]->ind));
    }


    file.close();
    //call a helper in mygl, give it the file name, then that calls the tinyobj in mesh, populate data

}

void MainWindow::on_MPM_clicked(){
    MPMSolver solver(10, 10, 1.0f);
    solver.addParticle(MPMParticle(QVector2D(2.0f, 5.0f), QVector2D(0.0f, 0.0f), 1.0f));

    for (int step = 0; step < 100; ++step) {
        solver.computeForcesAndIntegrate();
    }

    qDebug() << "Simulation complete.";
    return 0;
}

void MainWindow::on_loadJSON_clicked() {
    //ui->mygl->on_loadButton2_clicked(jointsTreeWidget);

    QString filename = QFileDialog::getOpenFileName(this, tr("Load Scene File"), QDir::currentPath().append(QString("../..")), tr("*.json"));

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open the JSON file.");
        return;
    }

    QByteArray jsonData = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qWarning("Invalid JSON data.");
        return;
    }

    QJsonObject rootJointObject = doc.object()["root"].toObject();

    ui->mygl->m_joint = mkU<Joint>(ui->mygl);
    QJsonArray pos = rootJointObject["pos"].toArray();
    glm::vec3 position = glm::vec3(pos[0].toDouble(), pos[1].toDouble(), pos[2].toDouble());
    std::cout << "ROOT POSITION" <<std::endl;
    std::cout << pos[0].toDouble() << ", " << pos[1].toDouble() << ", " << pos[2].toDouble() << std::endl;
    Joint* skeleton = ui->mygl->m_joint.get();
    skeleton->parseJson(rootJointObject);
    skeleton->setText(0, QString("root"));
    ui->mygl->m_joint->name = rootJointObject["name"].toString();
    skeleton->position = position;
    skeleton->create();

    jointsTreeWidget->clear();
    slot_addRootToTreeWidget(skeleton);

    file.close();
}

void MainWindow::on_exportUSD_clicked()
{
    QString usdContent;

    usdContent += "#usda 1.0\n";
    usdContent += "(\n";
    usdContent += "    defaultPrim = \"Mesh\"\n";
    usdContent += "    upAxis = \"Y\"\n";
    usdContent += ")\n\n";

    usdContent += "def Xform \"Mesh\" {\n";
    usdContent += "    def Mesh \"Mesh\" {\n";

    // Collect unique vertex positions
    auto vertss = ui->mygl->m_mesh.getVertss();
    auto faces = ui->mygl->m_mesh.getFacess(); // Assuming you have a function to retrieve faces.

    usdContent += "        int[] faceVertexCounts = [\n";
    for (size_t i = 0; i < faces.size(); ++i) {
        usdContent += QString("            %1%2\n")
                          .arg(faces[i]->getVertexCount())
                          .arg(i == faces.size() - 1 ? "" : ",");
    }
    usdContent += "        ]\n\n";

    usdContent += "        int[] faceVertexIndices = [\n";
    for (size_t i = 0; i < faces.size(); ++i) {
        auto verts = faces[i]->getVertices();
        for (size_t j = 0; j < verts.size(); ++j) {
            usdContent += QString("            %1%2\n")
                              .arg(verts[j]->ind) // Assuming each vertex has a unique index
                              .arg((i == faces.size() - 1 && j == verts.size() - 1) ? "" : ",");
        }
    }
    usdContent += "        ]\n\n";

    usdContent += "        point3f[] points = [\n";
    for (size_t i = 0; i < vertss.size(); ++i) {
        const auto& vertex = vertss[i];
        glm::vec3 vertpos = vertex->getPosition();
        usdContent += QString("            (%1, %2, %3)%4\n")
                          .arg(vertpos.x)
                          .arg(vertpos.y)
                          .arg(vertpos.z)
                          .arg(i == vertss.size() - 1 ? "" : ",");
    }
    usdContent += "        ]\n";

    usdContent += "    }\n";
    usdContent += "}\n";

    // Write USD to file
    QString filename = "/Users/christinaqiu/Downloads/GitHub/homework-05-06-07-micro-maya-mesh-editor-christinaqiu3/minimaya3.usda";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << usdContent;
        file.close();
    } else {
        qWarning() << "Failed to open file for writing: " << filename;
    }

    qDebug() << "Exported USD to: " << filename;
}

void MainWindow::on_changeColor_clicked()
{
    face* selectedFaceItem = dynamic_cast<face*>(ui->facesListWidget->currentItem());
    if (!selectedFaceItem) {
        std::cout << ui->facesListWidget->currentItem() << std::endl;
        return;
    }

    face* selectedFace = ui->mygl->m_faceDisplay.getFace();

    // Open color picker dialog
    QColor chosenColor = QColorDialog::getColor(Qt::white, this, "Select Face Color");

    if (!chosenColor.isValid()) { // Ensure user picked a valid color
        return;
    }

    // Convert QColor to glm::vec3 (normalize to [0,1] range)
    glm::vec3 newColor(
        chosenColor.redF(),
        chosenColor.greenF(),
        chosenColor.blueF()
        );

    // Apply color change
    selectedFace->setColor(newColor);

    ui->mygl->m_faceDisplay.updateFace(selectedFace);
    ui->mygl->m_mesh.create();
    // Trigger a redraw to update UI
    ui->mygl->update();
}


vertex* MainWindow::splitEdge(halfedge* e, bool smooth, std::unordered_map<face*, vertex*> faceCentroids, int numOrigHalfEdges) {
    glm::vec3 pos;
    if (smooth) {
       //check numOrigHalfEdges
        glm::vec3 smoothedMidpoint;
        if (e->ind < numOrigHalfEdges && e->getSymHalfEdge()->ind < numOrigHalfEdges) {
            vertex* v1 = e->getNextVertex();
            vertex* v2 = e->getSymHalfEdge()->getNextVertex();
            if (e->getIncidentFace() && e->getSymHalfEdge()->getIncidentFace()) {
                    // Both faces are incident to the edge
                face* f1 = e->getIncidentFace();
                face* f2 = e->getSymHalfEdge()->getIncidentFace();
                smoothedMidpoint = (v1->getPosition() + v2->getPosition() + faceCentroids[f1]->getPosition() + faceCentroids[f2]->getPosition()) / 4.0f;
            } else {
                    // Only one face is incident to the edge
                face* f = (e->getIncidentFace()) ? e->getIncidentFace() : e->getSymHalfEdge()->getIncidentFace();
                smoothedMidpoint = (v1->getPosition() + v2->getPosition() + faceCentroids[f]->getPosition()) / 3.0f;
            }
        }
        pos = smoothedMidpoint;

    } else {
        pos = (e->getNextVertex()->getPosition() + e->getSymHalfEdge()->getNextVertex()->getPosition()) / 2.f;
    }

    vertex* midpointVertex = new vertex();
//    uPtr<vertex> midpointVertex = mkU<vertex>();
    midpointVertex->setPosition(pos);
    ui->mygl->m_mesh.vertices.push_back(midpointVertex);  // Add the vertex to the mesh
    QListWidgetItem *itmv = midpointVertex;
    ui->vertsListWidget->addItem(itmv);
    itmv->setText(QString::number(ui->mygl->m_mesh.vertices.size()-1));

    halfedge* newHalfEdge = new halfedge(e->getNextHalfEdge(), e->getSymHalfEdge(), e->getIncidentFace(), e->getNextVertex(), ui->mygl->m_mesh.getHalfEdgess().size());
    ui->mygl->m_mesh.halfedges.push_back(newHalfEdge);
    QListWidgetItem *itmh1 = newHalfEdge;
    ui->halfEdgesListWidget->addItem(itmh1);
    itmh1->setText(QString::number(ui->mygl->m_mesh.halfedges.size()-1));

    halfedge* newSymHalfEdge = new halfedge(e->getSymHalfEdge()->getNextHalfEdge(), e, e->getSymHalfEdge()->getIncidentFace(), e->getSymHalfEdge()->getNextVertex(), ui->mygl->m_mesh.getHalfEdgess().size());
    ui->mygl->m_mesh.halfedges.push_back(newSymHalfEdge);
    QListWidgetItem *itmh2 = newSymHalfEdge;
    ui->halfEdgesListWidget->addItem(itmh2);
    itmh2->setText(QString::number(ui->mygl->m_mesh.halfedges.size()-1));

    vertex *v1 = e->getNextVertex();
    vertex *v2 = e->getSymHalfEdge()->getNextVertex();

    //currentEdge->setNextHalfEdge(e);
    e->setNextVertex(midpointVertex);
    e->setNextHalfEdge(newHalfEdge);
    e->getSymHalfEdge()->setNextVertex(midpointVertex);
    e->getSymHalfEdge()->setNextHalfEdge(newSymHalfEdge);
    e->getSymHalfEdge()->setSymHalfEdge(newHalfEdge);
    e->setSymHalfEdge(newSymHalfEdge);
    midpointVertex->setIncidentHalfEdge(e);
    v1->setIncidentHalfEdge(e->getNextHalfEdge());
    v2->setIncidentHalfEdge(e->getSymHalfEdge());

    return midpointVertex;
}

void MainWindow::on_addMidpoint_clicked()
{
    std::cout << "addMid" << std::endl;

    halfedge* selectedHalfEdge = dynamic_cast<halfedge*>(ui->halfEdgesListWidget->currentItem());
    if (!selectedHalfEdge) {
        std::cout << ui->halfEdgesListWidget->currentItem() << std::endl;
        return;
    }

    std::unordered_map<face*, vertex*> nullFaceCentroids;
    splitEdge(selectedHalfEdge, false, nullFaceCentroids, 0);

    ui->mygl->m_halfedgeDisplay.updateHalfEdge(selectedHalfEdge);
    ui->mygl->update();

}

void MainWindow::on_triangulateFace_clicked() {
    std::cout << "tri" << std::endl;

    face* selectedFaceItem = dynamic_cast<face*>(ui->facesListWidget->currentItem());
    if (!selectedFaceItem) {
        std::cout << ui->facesListWidget->currentItem() << std::endl;
        return;
    }

    face* selectedFace = ui->mygl->m_faceDisplay.getFace();

    halfedge* startEdge = selectedFace->getIncidentHalfEdge();
    halfedge* currentEdge = startEdge->getNextHalfEdge()->getNextHalfEdge();

    if (startEdge->ind != currentEdge->getNextHalfEdge()->ind) {
        do {
            vertex* vert = currentEdge->getNextVertex();
            halfedge* he1 = new halfedge(currentEdge->getNextHalfEdge(), nullptr, selectedFace, vert, ui->mygl->m_mesh.halfedges.size());
            ui->mygl->m_mesh.halfedges.push_back(he1);

            halfedge* he2 = new halfedge(startEdge->getNextHalfEdge(), nullptr, nullptr, startEdge->getNextVertex(), ui->mygl->m_mesh.halfedges.size());
            ui->mygl->m_mesh.halfedges.push_back(he2);

            face* newFace = new face();
            he2->setIncidentFace(newFace);

            currentEdge = he1->getNextHalfEdge();
            startEdge->getNextHalfEdge()->getNextHalfEdge()->setNextHalfEdge(he2);
            startEdge->setNextHalfEdge(he1);

            he1->setSymHalfEdge(he2);
            he2->setSymHalfEdge(he1);

            newFace->setIncidentHalfEdge(he2);

            glm::vec3 col = glm::vec3((double)rand()/(RAND_MAX), (double)rand()/(RAND_MAX), (double)rand()/(RAND_MAX));
            newFace->setColor(col);
            ui->mygl->m_mesh.faces.push_back(newFace);

            //create new widget items

            QListWidgetItem *itmh = he2;
            ui->halfEdgesListWidget->addItem(itmh);
            itmh->setText(QString::number(ui->mygl->m_mesh.halfedges.size()-1));

            QListWidgetItem *itmf = newFace;
            ui->facesListWidget->addItem(itmf);
            itmf->setText(QString::number(ui->mygl->m_mesh.faces.size()-1));

            // Oupdate GUI lists
        } while (startEdge->ind != currentEdge->getNextHalfEdge()->ind);
    }
    ui->mygl->m_mesh.create();
    ui->mygl->update();
}

void MainWindow::on_jointRotXPlus_clicked() {
    ui->mygl->setRot(1, 0, 0);
    updateDisplayRot();
}

void MainWindow::on_jointRotXMinus_clicked() {
    ui->mygl->setRot(-1, 0, 0);
    updateDisplayRot();
}

void MainWindow::on_jointRotYPlus_clicked() {
    ui->mygl->setRot(0, 1, 0);
    updateDisplayRot();
}

void MainWindow::on_jointRotYMinus_clicked() {
    ui->mygl->setRot(0, -1, 0);
    updateDisplayRot();
}

void MainWindow::on_jointRotZPlus_clicked() {
    ui->mygl->setRot(0, 0, 1);
    updateDisplayRot();
}

void MainWindow::on_jointRotZMinus_clicked() {
    ui->mygl->setRot(0, 0, -1);
    updateDisplayRot();
}

void MainWindow::on_meshSkinning_clicked() {
    if (!ui->mygl->m_mesh.vertices.empty()){
        ui->mygl->skinMesh();
    }
}

void MainWindow::populateLists() {
    vertsListWidget->clear();
    for (int i = 0; i < (int) ui->mygl->m_mesh.getVertss().size(); ++i) {
        QListWidgetItem *itmv = ui->mygl->m_mesh.getVertss()[i];
        vertsListWidget->addItem(itmv);
        vertsListWidget->item(i)->setText(QString::number(ui->mygl->m_mesh.getVertss()[i]->ind));
    }
    halfEdgesListWidget->clear();
    for (int i = 0; i < (int) ui->mygl->m_mesh.getHalfEdgess().size(); ++i) {
        QListWidgetItem *itmhe = ui->mygl->m_mesh.getHalfEdgess()[i];
        halfEdgesListWidget->addItem(itmhe);
        halfEdgesListWidget->item(i)->setText(QString::number(ui->mygl->m_mesh.getHalfEdgess()[i]->ind));
    }
    facesListWidget->clear();
    for (int i = 0; i < (int) ui->mygl->m_mesh.getFacess().size(); ++i) {
        QListWidgetItem *itmf = ui->mygl->m_mesh.getFacess()[i];
        emit sig_sendVert(itmf);
        facesListWidget->addItem(itmf);
        facesListWidget->item(i)->setText(QString::number(ui->mygl->m_mesh.getFacess()[i]->ind));
    }
}

void MainWindow::updateMeshComponentWidgetV(QListWidgetItem *selectedItem) {
    if (ui->mygl->m_vertDisplay.getVertex() != dynamic_cast<vertex*>(selectedItem)) {
        ui->mygl->m_vertDisplay.updateVertex(dynamic_cast<vertex*>(selectedItem));
    }
    ui->mygl->update();
}

void MainWindow::updateMeshComponentWidgetH(QListWidgetItem *selectedItem) {
    if (ui->mygl->m_halfedgeDisplay.getHalfEdge() != dynamic_cast<halfedge*>(selectedItem)) {
        ui->mygl->m_halfedgeDisplay.updateHalfEdge(dynamic_cast<halfedge*>(selectedItem));
    }
    ui->mygl->update();
}

void MainWindow::updateMeshComponentWidgetF(QListWidgetItem *selectedItem) {

    if (ui->mygl->m_faceDisplay.getFace() != dynamic_cast<face*>(selectedItem)) {
        ui->mygl->m_faceDisplay.updateFace(dynamic_cast<face*>(selectedItem));
    }
    ui->mygl->update();
}

void MainWindow::updateJointComponentWidget(QTreeWidgetItem *selectedItem) {
    //setSelected

    Joint* selectedJoint = static_cast<Joint*>(selectedItem);

    if (selectedJoint) {

        ui->mygl->m_joint->updateSelectedJoint(selectedJoint);
        //ui->mygl->m_joint->create();//maybe?
    }
    ui->mygl->update();

}

void MainWindow::updateDisplayRot() {
    glm::vec4 temp = ui->mygl->m_joint->getSelectedJoint()->rotation * glm::vec4(0,0,0,1);

    ui->jointRotX->setText(QString::number(temp.x));
    ui->jointRotY->setText(QString::number(temp.y));
    ui->jointRotZ->setText(QString::number(temp.z));
}

void MainWindow::on_catmullClark_clicked() {
    std::unordered_map<face*, vertex*> faceCentroids;
    int numOrigVerts = ui->mygl->m_mesh.vertices.size();
    int numOrigHalfEdges = ui->mygl->m_mesh.halfedges.size();
    int numOrigFaces = ui->mygl->m_mesh.faces.size();

    //go thru orig faces and add centroids
    for (int i = 0; i < numOrigFaces; i++) {
        face* f = ui->mygl->m_mesh.faces[i];
        glm::vec3 totalPos = glm::vec3(0,0,0);
        float count = 0;
        halfedge* startEdge = f->getIncidentHalfEdge();
        halfedge* currentEdge = startEdge;

        do {
            vertex* vert = currentEdge->getNextVertex();
            totalPos += vert->getPosition();
            currentEdge = currentEdge->getNextHalfEdge();
            count++;
        } while (startEdge != currentEdge);

        glm::vec3 centroid = totalPos / count;
        vertex* newVert = new vertex(centroid, nullptr, ui->mygl->m_mesh.vertices.size());
        faceCentroids[f] = newVert;
        ui->mygl->m_mesh.vertices.push_back(newVert);

        QListWidgetItem *itmv = newVert;
        ui->vertsListWidget->addItem(itmv);
        itmv->setText(QString::number(ui->mygl->m_mesh.vertices.size()-1));
    }

    std::unordered_map<halfedge*, vertex*> midpointEdges = smoothEdgeMidpoints(faceCentroids, numOrigHalfEdges);

    // smooth the original vertices
    smoothOriginalVertices(faceCentroids, numOrigVerts);

    for (int i = 0; i < numOrigFaces; i++) {
        face* f = ui->mygl->m_mesh.faces[i];
    // connect these vertices with half-edges
        Quadrangulate(f, faceCentroids[f]);
    }
    ui->mygl->m_mesh.create();
    ui->mygl->update();
}

std::unordered_map<halfedge*, vertex*> MainWindow::smoothEdgeMidpoints(std::unordered_map<face*, vertex*> faceCentroids, int numOrigHalfEdges) {
    std::unordered_map<halfedge*, vertex*> halfEdgeMidpoints;
    std::unordered_map<halfedge*, vertex*> doneHalfEdges;

    // when testing, there should be + 6 verts for centroids, then + 12 verts for midpoints, then edges are doubled.

    for (int i = 0; i < numOrigHalfEdges; i++) {
        halfedge* edge = ui->mygl->m_mesh.halfedges[i];
        if (doneHalfEdges.count(edge) == 0 && doneHalfEdges.count(edge->getSymHalfEdge()) == 0) { // && edge->getSymHalfEdge()->ind < numOrigHalfEdges
            vertex* vert;
            halfEdgeMidpoints[edge] = vert;
            halfEdgeMidpoints[edge->getSymHalfEdge()] = vert;
            doneHalfEdges[edge] = halfEdgeMidpoints[edge];
            doneHalfEdges[edge->getSymHalfEdge()] = halfEdgeMidpoints[edge];
            vert = splitEdge(edge, true, faceCentroids, numOrigHalfEdges);
            std::cout << "midpoint pos: " << vert->getPosition()[0] << ", " << vert->getPosition()[1] << ", " << vert->getPosition()[2] << std::endl;
        }
//        doneHalfEdges[edge] = halfEdgeMidpoints[edge];
//        doneHalfEdges[edge->getSymHalfEdge()] = halfEdgeMidpoints[edge];
    }
    return halfEdgeMidpoints;
}

void MainWindow::smoothOriginalVertices(std::unordered_map<face*, vertex*> faceCentroids, int numOrigVerts) {
    int countV = 0;

    for (vertex* v : ui->mygl->m_mesh.vertices) {
        if (countV < numOrigVerts) {
            glm::vec3 sumE(0.0f); //sum of all adjacent midpoints
            glm::vec3 sumF(0.0f); //sum of the centroids of all faces incident to v

            float count = 0;

            halfedge* startEdge = v->getIncidentHalfEdge();
            halfedge* currentEdge = startEdge;

            //std::cout << "original position: " << v->getPosition()[0] << ", " << v->getPosition()[1] << ", " << v->getPosition()[2] << std::endl;

            do {
                currentEdge = currentEdge->getNextHalfEdge();
                sumF += faceCentroids[currentEdge->getIncidentFace()]->getPosition();
                sumE += currentEdge->getNextVertex()->getPosition();

                count++;

                currentEdge = currentEdge->getSymHalfEdge();
            } while (currentEdge != startEdge);

            // Compute the updated vertex position
            glm::vec3 updatedPos = ((count - 2.f) * v->getPosition() + sumE / count + sumF / count) / count;

            //std::cout << "sumE: " << sumE[0] << ", " << sumE[1] << ", " << sumE[2] << std::endl;
            //std::cout << "sumF: " << sumF[0] << ", " << sumF[1] << ", " << sumF[2] << std::endl;
            //std::cout << "count: " << count << std::endl;
            //std::cout << "updated position: " << updatedPos[0] << ", " << updatedPos[1] << ", " << updatedPos[2] << std::endl;

            // Set the updated position for the vertex
            v->setPosition(updatedPos);

        }
        countV ++;
    }
    ui->mygl->m_mesh.create();
    update();
}

void MainWindow::Quadrangulate(face* originalFace, vertex* centroid) {

    halfedge* startEdge = originalFace->getIncidentHalfEdge();
    halfedge* currentEdge = startEdge;

    halfedge* tempToSetNext1 = nullptr;
    halfedge* tempToSetNext2 = nullptr;
    halfedge* tempToSetSym = nullptr;

    do {

        halfedge* newHalfEdge = new halfedge(nullptr, nullptr, originalFace, centroid, ui->mygl->m_mesh.getHalfEdgess().size());
        ui->mygl->m_mesh.halfedges.push_back(newHalfEdge);
        QListWidgetItem *itmh1 = newHalfEdge;
        ui->halfEdgesListWidget->addItem(itmh1);
        itmh1->setText(QString::number(ui->mygl->m_mesh.halfedges.size()-1));

        halfedge* newHalfEdge2 = new halfedge(currentEdge->getNextHalfEdge(), nullptr, originalFace, currentEdge->getNextVertex(), ui->mygl->m_mesh.getHalfEdgess().size());
        ui->mygl->m_mesh.halfedges.push_back(newHalfEdge2);
        QListWidgetItem *itmh2 = newHalfEdge2;
        ui->halfEdgesListWidget->addItem(itmh2);
        itmh2->setText(QString::number(ui->mygl->m_mesh.halfedges.size()-1));

        newHalfEdge->setNextHalfEdge(newHalfEdge2);

        currentEdge = currentEdge->getNextHalfEdge();
        currentEdge = currentEdge->getNextHalfEdge();

        //set next
        if (tempToSetNext1 != nullptr) {
            tempToSetNext1-> setNextHalfEdge(tempToSetNext2);
        }
        tempToSetNext1 = currentEdge;
        tempToSetNext2 = newHalfEdge;

        //need to set sym
        if (tempToSetSym != nullptr) {
            newHalfEdge2->setSymHalfEdge(tempToSetSym);
            tempToSetSym->setSymHalfEdge(newHalfEdge2);
        }
        tempToSetSym = newHalfEdge;

    } while (currentEdge != startEdge);

    //temp for last setting he
    halfedge* tempLastSetSym = currentEdge->getNextHalfEdge()->getNextHalfEdge()->getNextHalfEdge()->getNextHalfEdge();

    //set next
    tempToSetNext1-> setNextHalfEdge(tempToSetNext2);
    tempToSetNext2->setSymHalfEdge(tempLastSetSym);
    tempLastSetSym->setSymHalfEdge(tempToSetNext2);

    centroid->setIncidentHalfEdge(tempLastSetSym->getSymHalfEdge()); //currentedge

    bool checkFirst = false;
    do {
        if (checkFirst) {
            face* newFace = new face(currentEdge, glm::vec3((double)rand()/(RAND_MAX), (double)rand()/(RAND_MAX), (double)rand()/(RAND_MAX)), ui->mygl->m_mesh.faces.size());
            ui->mygl->m_mesh.faces.push_back(newFace);
            QListWidgetItem *itmf = newFace;
            ui->facesListWidget->addItem(itmf);
            itmf->setText(QString::number(ui->mygl->m_mesh.faces.size()-1));

            currentEdge->setIncidentFace(newFace);
            currentEdge->getNextHalfEdge()->setIncidentFace(newFace);
            currentEdge->getNextHalfEdge()->getNextHalfEdge()->setIncidentFace(newFace);
            currentEdge->getNextHalfEdge()->getNextHalfEdge()->getNextHalfEdge()->setIncidentFace(newFace);
        } else {
            checkFirst = true;
        }
        currentEdge = currentEdge->getNextHalfEdge()->getSymHalfEdge()->getNextHalfEdge()->getNextHalfEdge();

    } while (currentEdge != startEdge);

    // Clean up the original face
//    ui->mygl->m_mesh.faces.erase(std::remove(ui->mygl->m_mesh.faces.begin(), ui->mygl->m_mesh.faces.end(), originalFace), ui->mygl->m_mesh.faces.end());
//    delete originalFace;

}

void MainWindow::slot_addRootToTreeWidget(QTreeWidgetItem *i) {
    ui->jointsTreeWidget->addTopLevelItem(i);
}
