#include "SCBDentalReconstructionOptionDialog.h"

#include <QDebug>
// #include "SCBScene.h"
// #include "SCBVolume.h"
// #include "SCBDentalHub.h"
// #include "SCBDentalMainWindow.h"
// #include "SCBNonPlanarViewer.h"

SCBDentalReconstructionOptionDialog::SCBDentalReconstructionOptionDialog(
    QWidget* parent) :
    QDialog(parent)
{
  this->setupUi(this);

  // connect either spin box or horizontal slider
  connect(this->horizontalSliderVolumeRendering,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(changeVolumeRenderingShift(int)),
          Qt::QueuedConnection);
}

SCBDentalReconstructionOptionDialog::~SCBDentalReconstructionOptionDialog() {}

void SCBDentalReconstructionOptionDialog::changeVolumeRenderingShift(int value)
{
  Q_UNUSED(value)
  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   QStringList volumes = scene->getAllDataByClassName("SCBVolume");
  //   if (volumes.isEmpty())
  //   {
  //     qCritical() << "Volume doesn't not exist";
  //     return;
  //   }

  //   SCBVolume* volume =
  //   scene->getDataByUniqueName<SCBVolume>(volumes.first()); if (!volume)
  //   {
  //     qCritical() << "Volume doesn't not exist";
  //     return;
  //   }

  //   volume->setShift(value);
  //   SCBDentalHub::self->mainWindow->nonPlanarViewer->Render();
}
