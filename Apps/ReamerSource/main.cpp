#include "mitkReamerSource.h"
#include <mitkIOUtil.h>

int main(int argc, char *argv[])
{
  auto s = mitk::ReamerSource::New();
  s->SetSphereRadius(25);
  s->SetTubeRadius(2);
  s->SetLength(30);
  s->SetThetaResolution(10);
  s->Update();

  mitk::IOUtil::Save(s->GetOutput(), "reamer.vtp");


  return 0;
}