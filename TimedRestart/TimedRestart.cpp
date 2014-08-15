#include "TimedRestart.h"
#include "HandlerAPI.h"
#include <sstream>
#include <stdlib.h>

#include <stdio.h>
#include <time.h>

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

   if( (nHour - g_HourLastRan) < 4 )
      return 0;

   int nHourOfDay = nHour % 24;

   //if( nHourOfDay == 0 )
   {
      g_HourLastRan = nHour;
      pIHandler->RestartCampfire(strRoom.c_str());
   }

   return 0;
}



