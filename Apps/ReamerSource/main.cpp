#include "mitkReamerSource.h"
#include <mitkIOUtil.h>

int main(int argc, char *argv[])
{
  auto s = mitk::ReamerSource::New();
  s->SetRadius(25);
  s->SetLength(30);
  s->Update();

  mitk::IOUtil::Save(s->GetOutput(), "reamer.vtp");


  return 0;
}