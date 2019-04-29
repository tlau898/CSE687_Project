#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>

namespace FileManagerNS
{
   class FileManager
   {
   public:
      FileManager() = default;
      static bool CheckFileExists(std::string filePath);

   private:

   };
}

#endif