#include "FileManager.h"

using namespace FileManagerNS;
using namespace std;

bool FileManager::CheckFileExists(std::string filePath)
{
   ifstream ifile(filePath, std::ios::binary);

   if (ifile)
   {
      return true;
   }
   else
   {
      cout << "Cannot find file " << filePath << endl;
      return false;
   }
   return (bool)ifile;
}


#ifdef TEST_FILEMANAGER

int main()
{
    return 0;
}

#endif

