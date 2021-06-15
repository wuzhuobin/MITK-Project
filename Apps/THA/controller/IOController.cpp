#include "IOController.h"

#include "mitkReamerSource.h"

// mitk
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>
#include <mitkSurface.h>

// vtk
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

// qt
#include <qcoreapplication.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>

#include <QSharedPointer>

// QSharedPointer<IOController> gInstance = QSharedPointer<IOController>(new IOController);

IOController* IOController::getInstance()
{
    static IOController gInstance;
    return &gInstance;
}

IOController::IOController(QObject* parent) : QObject(parent)
{
    initCaseDataBase();
}

void IOController::loadScene(const QString& fileName)
{
    mitk::SceneIO::Pointer sceneIO = mitk::SceneIO::New();
    mitk::DataStorage* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
    mitk::DataStorage::SetOfObjects::ConstPointer dataNodes = sceneIO->LoadScene(fileName.toStdString(), ds)->GetAll();
    MITK_INFO << "DataStorage, num of nodes: " << dataNodes->size();
    for (const mitk::DataNode* one : *dataNodes)
    {
        MITK_INFO << one->GetName();
    }

    this->addReamer();
    this->addReamerTrajectory();
    Q_EMIT this->sceneLoaded();
}

void IOController::addReamerTrajectory()
{
    mitk::SceneIO::Pointer sceneIO = mitk::SceneIO::New();
    mitk::DataStorage* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

    mitk::ReamerSource::Pointer reamerSource = mitk::ReamerSource::New();
    reamerSource->SetSphereRadius(25);
    reamerSource->SetTubeRadius(25);
    reamerSource->SetLength(70);
    reamerSource->Update();
    mitk::Surface* reamer = reamerSource->GetOutput();

    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
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

    vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformPolyData->SetInputData(reamer->GetVtkPolyData());
    transformPolyData->SetTransform(transform);
    transformPolyData->Update();
    reamer->SetVtkPolyData(transformPolyData->GetOutput());

    mitk::DataNode::Pointer reamerNode = mitk::DataNode::New();
    reamerNode->SetData(reamer);
    reamerNode->SetName("reamer_trajectory");
    reamerNode->SetVisibility(false);

    ds->Add(reamerNode);
}

void IOController::addReamer()
{
    mitk::SceneIO::Pointer sceneIO = mitk::SceneIO::New();
    mitk::DataStorage* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

    mitk::ReamerSource::Pointer reamerSource = mitk::ReamerSource::New();
    reamerSource->SetSphereRadius(25);
    reamerSource->SetTubeRadius(2);
    reamerSource->SetLength(70);
    reamerSource->Update();
    mitk::Surface* reamer = reamerSource->GetOutput();

    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply();
    transform->RotateY(-90);
    transform->RotateX(0);
    transform->RotateZ(0);
    transform->Translate(-58.54936906586104, 14.204643754424644, 1578.9678664012258);
    transform->Update();

    vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformPolyData->SetInputData(reamer->GetVtkPolyData());
    transformPolyData->SetTransform(transform);
    transformPolyData->Update();
    reamer->SetVtkPolyData(transformPolyData->GetOutput());

    mitk::DataNode::Pointer reamerNode = mitk::DataNode::New();
    reamerNode->SetData(reamer);
    reamerNode->SetName("reamer");
    reamerNode->SetVisibility(false);

    ds->Add(reamerNode);
}

void IOController::addReamerCuter() {}

void IOController::initCaseDataBase()
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