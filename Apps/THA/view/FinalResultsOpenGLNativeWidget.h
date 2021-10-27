#ifndef FINAL_RESULTS_OPENGL_NATIVE_WIDGETS_H
#define FINAL_RESULTS_OPENGL_NATIVE_WIDGETS_H

// vtk
#include <QVTKOpenGLNativeWidget.h>

class FinalResultsOpenGLNativeWidget : public QVTKOpenGLNativeWidget
{
public:
  explicit FinalResultsOpenGLNativeWidget(QWidget *parent = nullptr);
  void initializeWidget();
};

#endif  // !FINAL_RESULTS_OPENGL_NATIVE_WIDGETS_H