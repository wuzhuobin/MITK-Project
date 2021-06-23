#include "THADicomLocalStorageWidget.h"

// qt
#include <QmitkDicomLocalStorageWidget.h>
#include <qdebug.h>
#include <qwidget.h>

// ctk
#include <ctkDICOMThumbnailGenerator.h>

THADicomLocalStorageWidget::THADicomLocalStorageWidget(QWidget* parent) : QmitkDicomLocalStorageWidget(parent) {}

void THADicomLocalStorageWidget::setDatabaseDirectory(QString newDatabaseDirectory)
{
    SetDatabaseDirectory(newDatabaseDirectory);
    m_LocalDatabase->setThumbnailGenerator(new ctkDICOMThumbnailGenerator(this));
}

void THADicomLocalStorageWidget::onStartDicomImport(const QStringList& dicomData)
{
    if (m_LocalDatabase->isOpen())
    {
        m_LocalIndexer->addListOfFiles(dicomData, true);
    }
}