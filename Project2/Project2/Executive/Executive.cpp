#include "Executive.h"

void Executive::CreateTests(string xmlTestRequest)
{
   testHarness.AddTests(xmlTestRequest);
}

void Executive::RunSampleTestSequence()
{
   string xmlTestRequest;

   xmlTestRequest = string("<testelement>SampleDriverA.dll</testelement>") +
      string("<testelement>SampleDriverB.dll</testelement>");

   testHarness.AddTests(xmlTestRequest);
   testHarness.RunTestList();
   testHarness.printLevelOneLog();
   testHarness.printLevelTwoLog();
   testHarness.printLevelThreeLog();
   testHarness.ClearTestList();

}

int main()
{
   Executive executive;
   executive.RunSampleTestSequence();
}

