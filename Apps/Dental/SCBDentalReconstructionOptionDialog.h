#ifndef SCBDENTALRECONSTRUCTIONOPTIONDIALOG_H
#define SCBDENTALRECONSTRUCTIONOPTIONDIALOG_H

#include <QDialog>
#include "ui_SCBDentalReconstructionOptionDialog.h"

class SCBDentalReconstructionOptionDialog :
	public QDialog,
        public Ui::SCBDentalReconstructionOptionDialog
{
    Q_OBJECT

public:
    explicit SCBDentalReconstructionOptionDialog(QWidget *parent = 0);
    ~SCBDentalReconstructionOptionDialog();

public slots:
	void changeVolumeRenderingShift(int value);

};

#endif // SCBDENTALRECONSTRUCTIONOPTIONDIALOG_H
