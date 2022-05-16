/**
 * @file CupImpactionParameterGadget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "CupImpactionParameterGadget.h"

#include "ui_CupImpactionParameterGadget.h"

CupImpactionParameterGadget::CupImpactionParameterGadget(QWidget* parent) :
    QWidget(parent), mUi(new Ui::CupImpactionParameterGadget)
{
  mUi->setupUi(this);
}

CupImpactionParameterGadget::~CupImpactionParameterGadget() = default;