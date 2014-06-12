#include "CommandProcessor.h"
#include <iostream>
#include <algorithm>
#include "ConnectionManager.h"

using namespace std;

int StringToInt(const std::string& str)
{
   return atoi(str.c_str());
}

std::string Trim(std::string str)
{
   while(str.size()>0 && (str[0]==' ' || str[str.size()-1]==' '))
   {
      if( str[0]==' ' )
         str.erase(0, 1);
      if( str[str.size()-1]==' ' )
         str.erase(str.size()-1, 1);
   }
   return str;
}

CommandProcessor::CommandProcessor(ConnectionManager* pConnectionManager)
: m_pConnectionManager(pConnectionManager)
{

}

void CommandProcessor::Run()
{
   DisplayHelp();

   while(true)
   {
      cout << "> ";
      std::string strCommand;
      getline(cin, strCommand);

      if( IsExitCommand(strCommand) )
      {
         m_pConnectionManager->Exit();
         return;
      }

      else if( IsHelpCommand(strCommand) )
      {
         DisplayHelp();
      }

      else if( IsReloadHandlersCommand(strCommand) )
      {
         m_pConnectionManager->ReloadChatHandlers();
      }

      else if( IsJoinCommand(strCommand) )
      {
         string strRoomName = Trim(strCommand.substr(4));

         //cout << "Joining room..." << endl;
         m_pConnectionManager->JoinRoom("testwith", "534c7a1597bfb3c862de90f69310a8ad1a1eda04", strRoomName, true);
         //m_pConnectionManager->JoinRoom("camtasiaslate", "0c2f691569082638c0469f52dabfa5a0fdb29b6e", strRoomName, true);
      }

      else if( IsLeaveCommand(strCommand) )
      {
         string strRoomName = Trim(strCommand.substr(5));

         //cout << "Joining room..." << endl;
         m_pConnectionManager->LeaveRoom(strRoomName);
         //m_pConnectionManager->JoinRoom("camtasiaslate", "build_bot@yahoo.com", "a3.1415926", "CS 8 Room", true);
      }

      else if( IsSayCommand(strCommand) )
      {
         string strMessage = strCommand.substr(4);

         string::size_type nPos = strMessage.find_first_of(';');
         if( nPos == string::npos )
            return;

         string strRoom = Trim(strMessage.substr(0, nPos));
         strMessage = Trim(strMessage.substr(nPos+1));

         m_pConnectionManager->Say(strRoom, strMessage);
      }

      else if( IsUpdateFrequencyCommand(strCommand) )
      {
         string strFreqency = strCommand.substr(16);
         if( strFreqency.length() > 0 )
         {
            int nFrequencyMS = StringToInt(strFreqency);
            if( nFrequencyMS > 0 )
            {
               m_pConnectionManager->ChangeUpdateFrequency(nFrequencyMS);

               cout << "Updated frequency: " << nFrequencyMS << endl;
            }
         }
      }

      else if( IsStartTrelloCommand(strCommand) )
      {
         string::size_type nPos = strCommand.find_first_of(';');
         if( nPos == string::npos )
            return;

         string strRoom = Trim(strCommand.substr(nPos+1));

         m_pConnectionManager->StartTrelloUpdate(strRoom);

      }

   }
}

bool CommandProcessor::IsExitCommand(const std::string& strCommand) const
{
   return ToLower(strCommand) == "exit";
}

bool CommandProcessor::IsHelpCommand(const std::string& strCommand) const
{
   return ToLower(strCommand) == "help";
}

bool CommandProcessor::IsReloadHandlersCommand(const std::string& strCommand) const
{
   return ToLower(strCommand) == "reload";
}

bool CommandProcessor::IsJoinCommand(const std::string& strCommand) const
{
   return ToLower(strCommand).find("join") == 0;
}

bool CommandProcessor::IsLeaveCommand(const std::string& strCommand) const
{
   return ToLower(strCommand).find("leave") == 0;
}

bool CommandProcessor::IsSayCommand(const std::string& strCommand) const
{
   return ToLower(strCommand).find("say ") == 0;
}

bool CommandProcessor::IsUpdateFrequencyCommand(const std::string& strCommand) const
{
   return ToLower(strCommand).find("update frequency ") == 0;
}

bool CommandProcessor::IsStartTrelloCommand(const std::string& strCommand) const
{
   return ToLower(strCommand).find("start trello") == 0;
}

std::string CommandProcessor::ToLower(const std::string& strCommand)
{
   std::string strReturn(strCommand);
   std::transform(strReturn.begin(), strReturn.end(), strReturn.begin(), ::tolower);
   return strReturn;
}

void CommandProcessor::DisplayHelp()
{
   cout << "******CampfireBot******" << endl;
   cout << "Available commands:" << endl;
   cout << "Help - This help" << endl;
   cout << "Reload - Reloads the chat handlers" << endl;
   cout << "Join <RoomName> - Joins a campfire room" << endl;
   cout << "Leave <RoomName> - Leaves a campfire room" << endl;
   cout << "Say <RoomName>;<text> - Say something" << endl;
   cout << "Update Frequency <MS> -- Change the listen rate" << endl;
   cout << "Start Trello;<RoomName> -- Starts listening for Trello updates" << endl;
   cout << "Exit - Closes the program" << endl;
}


