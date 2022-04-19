#include "IOController.h"

#include "mitkReamerSource.h"

// mitk
#include <mitkIOUtil.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkSurface.h>
#include <mitkVtkRepresentationProperty.h>

// vtk
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

// qt
#include <qapplication.h>
#include <qcoreapplication.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>

#include <QSharedPointer>

QString IOController::getBaseProject()
{
    static QString gBaseProject(qApp->applicationDirPath() + "/THA.mitk");
    return gBaseProject;
}

IOController* IOController::getInstance()
{
    static IOController gInstance;
    return &gInstance;
}

IOController::IOController(QObject* parent) : QObject(parent)
{
    initCaseDataBase();
}

void IOController::loadScene(const QString& fileName) const
{
    auto sceneIO = mitk::SceneIO::New();
    auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
    auto dataNodes = sceneIO->LoadScene(fileName.toStdString(), ds)->GetAll();
    MITK_INFO << "DataStorage, num of nodes: " << dataNodes->size();
    for (const auto& one : *dataNodes)
    {
        MITK_INFO << one->GetName();
    }

    addReamer();
    addReamerTrajectory();
    Q_EMIT sceneLoaded();
}

bool IOController::createScene(const QString& filename, const QStringList& dicoms) const
{
    auto sceneIO = mitk::SceneIO::New();
    auto ds = mitk::StandaloneDataStorage::New();
    sceneIO->LoadScene(getBaseProject().toStdString(), ds);
    ds->Remove(ds->GetNamedNode("image"));

    auto setObObjects = mitk::IOUtil::Load(dicoms.first().toStdString(), *ds);
    setObObjects->front()->SetName("image");

    return sceneIO->SaveScene(ds->GetAll(), ds, filename.toStdString());
}

void IOController::addReamerTrajectory() const
{
    auto sceneIO = mitk::SceneIO::New();
    auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

    auto reamerSource = mitk::ReamerSource::New();
    reamerSource->SetSphereRadius(25);
    reamerSource->SetTubeRadius(25);
    reamerSource->SetLength(70);
    reamerSource->Update();
    auto* reamer = reamerSource->GetOutput();

    auto transform = vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply();
    transform->RotateY(-90);
    transform->RotateX(0);
    transform->RotateZ(0);
    transform->Translate(-58.54936906586104, 14.204643754424644, 1578.9678664012258);
    // transform->Translate(
    //     -28.54936906586104,
    //     14.204643754424644,
    //     1578.9678664012258);
    transform->Update();

    auto transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformPolyData->SetInputData(reamer->GetVtkPolyData());
    transformPolyData->SetTransform(transform);
    transformPolyData->Update();
    reamer->SetVtkPolyData(transformPolyData->GetOutput());

    auto reamerNode = mitk::DataNode::New();
    reamerNode->SetData(reamer);
    reamerNode->SetName("reamer_trajectory");
    reamerNode->SetVisibility(false);

    ds->Add(reamerNode);
}

void IOController::addReamer() const
{
    auto sceneIO = mitk::SceneIO::New();
    auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

    auto reamerSource = mitk::ReamerSource::New();
    reamerSource->SetSphereRadius(25);
    reamerSource->SetTubeRadius(2);
    reamerSource->SetLength(70);
    reamerSource->SetThetaResolution(10);
    reamerSource->SetPhiResolution(10);
    reamerSource->Update();
    auto* reamer = reamerSource->GetOutput();

    auto transform = vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply();
    transform->RotateY(-90);
    transform->RotateX(0);
    transform->RotateZ(0);
    // transform->Translate(-58.54936906586104, 14.204643754424644, 1578.9678664012258);
    transform->Translate(
        -108.54936906586104,
        14.204643754424644,
        1578.9678664012258);
    transform->Update();

    auto transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformPolyData->SetInputData(reamer->GetVtkPolyData());
    transformPolyData->SetTransform(transform);
    transformPolyData->Update();
    reamer->SetVtkPolyData(transformPolyData->GetOutput());

    auto reamerNode = mitk::DataNode::New();
    reamerNode->SetData(reamer);
    reamerNode->SetName("reamer");
    reamerNode->SetVisibility(false);
    reamerNode->SetProperty("material.representation",
                            mitk::VtkRepresentationProperty::New("Wireframe"));

    ds->Add(reamerNode);
}

void IOController::initCaseDataBase() const
{
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    MITK_INFO << (qApp->applicationDirPath() + "/tha.sql").toStdString();
    db.setDatabaseName(qApp->applicationDirPath() + "/tha.sql");
    if (!db.open())
    {
        MITK_INFO << db.lastError().text().toStdString();
    }

    QSqlQuery createTable(
        "CREATE TABLE IF NOT EXISTS \"Cases\" ("
        "\"ID\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
        "\"Name\"	TEXT NOT NULL,"
        "\"DateTime\"	TEXT NOT NULL,"
        "\"Description\"	TEXT,"
        "\"File\"	TEXT"
        ")");
    if (!createTable.exec())
    {
        MITK_INFO << createTable.lastError().text().toStdString();
    }
    QSqlQuery insert("");

    if (insert.exec())
    {
        MITK_INFO << createTable.lastError().text().toStdString();
    }
}