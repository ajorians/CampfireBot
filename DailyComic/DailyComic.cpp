#include "DailyComic.h"
#include "HandlerAPI.h"
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>

#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dirent.h>
#endif

int g_HourLastRan = 0;

int StringToInt(const std::string& str)
{
   return atoi(str.c_str());
}

std::string IntToString(int nValue)
{
   std::ostringstream oss;
   oss << nValue;

   return oss.str();
}

void Replace(std::string& s, const std::string& strToReplace, const std::string& strToReplaceWith)
{
   int nPos = 0;
   while(true)
   {
      nPos = s.find(strToReplace, nPos);
      if( nPos == std::string::npos )
         return;

      s.replace(nPos, strToReplace.length(), strToReplaceWith);
      nPos += strToReplaceWith.length();
   }

}

std::string UnEscapeXML(const std::string str)
{
   std::string strReturn(str);
   Replace(strReturn, "&quot;", "\"");
   Replace(strReturn, "&amp;", "&");
   Replace(strReturn, "&lt;", "<");
   Replace(strReturn, "&gt;", ">");
   Replace(strReturn, "&apos;", "\'");
   return strReturn;
}

HANDLER_EXTERN int HandlerMessageSaid(void* pICampfireManager, const char* pstrRoom, int nType, int nUserID, const char* pstrMessage)
{
   return 0;
}

HANDLER_EXTERN int HandlerTrelloAdjustment(void* pICampfireManager, const char* pstrRoom, const char* pstrCard, const char* strListBefore, const char* strListAfter, const char* strDescription, int nCreated, int nClosed)
{
   return 0;
}

#ifdef _WIN32
std::vector<std::string> get_all_files_names_within_folder(std::string folder)
{
   std::vector<std::string> names;
   char search_path[200];
   sprintf(search_path, "%s*.*", folder.c_str());
   WIN32_FIND_DATAA fd; 
   HANDLE hFind = ::FindFirstFileA(search_path, &fd); 
   if(hFind != INVALID_HANDLE_VALUE) 
   { 
      do 
      { 
         // read all (real) files in current folder
         // , delete '!' read other 2 default folder . and ..
         if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) 
         {
            names.push_back(fd.cFileName);
         }
      }while(::FindNextFileA(hFind, &fd)); 
      ::FindClose(hFind); 
   } 
   return names;
}
#else
std::vector<std::string> GetAllFilesInFolder(const char *path)
{
   std::vector<std::string> arrFiles;
   struct dirent *entry;
   int ret = 1;
   DIR *dir;
   dir = opendir (path);

   while ((entry = readdir (dir)) != NULL) {
      std::string str = entry->d_name;
      if( str == "." || str == ".." )
         continue;
      if( str.length() > 4 && str[str.length()-4] == 'h' && str[str.length()-3] == 't' &&str[str.length()-2] == 'm' &&str[str.length()-1] == 'l' )
         continue;
      if( str.length() > 2 && str[str.length()-2] == 's' && str[str.length()-1] == 'h' )
         continue;
      arrFiles.push_back(entry->d_name);
      printf("\n%s",entry->d_name);
   }

   return arrFiles;
}
#endif

std::string FindFileToUpload()
{
   std::vector<std::string> arrFiles = 
#ifdef _WIN32
      get_all_files_names_within_folder("c:\\crap\\");
#else
      GetAllFilesInFolder("/home/aorians/bin/");
#endif
   if( arrFiles.size() <= 0 )
      return std::string();

   return 
#ifdef _WIN32
      "c:\\crap\\" + 
#else
      "/home/aorians/bin/" +
#endif
      arrFiles[0];
}


HANDLER_EXTERN int HandlerTimeEvent(void* pICampfireManager, const char* pstrRoom)
{
   IHandler* pIHandler = (IHandler*)pICampfireManager;
   std::string strRoom(pstrRoom);

   time_t seconds;

   seconds = time (NULL);

   int nHour = (seconds/3600);

   if( g_HourLastRan == 0 )
   {
      g_HourLastRan = nHour;
      return 0;
   }

   if( (nHour - g_HourLastRan) < 1 )
   {
      return 0;
   }

   int nHourOfDay = nHour % 24;

   //if( nHourOfDay == 0 )
   {
      g_HourLastRan = nHour;

      std::string strFile = FindFileToUpload();
      if( strFile.length() > 0 )
      {
//         pIHandler->SayMessage(pstrRoom, strFile.c_str());
         pIHandler->UploadMessage(pstrRoom, strFile.c_str(), true);
      }
   }

   return 0;
}



