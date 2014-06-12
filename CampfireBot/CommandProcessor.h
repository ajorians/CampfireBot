#pragma once
#include <string>

class ConnectionManager;

class CommandProcessor
{
public:
   CommandProcessor(ConnectionManager* pConnectionManager);

   void Run();

protected:
   bool IsExitCommand(const std::string& strCommand) const;
   bool IsHelpCommand(const std::string& strCommand) const;
   bool IsReloadHandlersCommand(const std::string& strCommand) const;
   bool IsJoinCommand(const std::string& strCommand) const;
   bool IsLeaveCommand(const std::string& strCommand) const;
   bool IsSayCommand(const std::string& strCommand) const;
   bool IsUpdateFrequencyCommand(const std::string& strCommand) const;

   bool IsStartTrelloCommand(const std::string& strCommand) const;
   static std::string ToLower(const std::string& strCommand);

   void DisplayHelp();

protected:
   ConnectionManager* m_pConnectionManager;//Does not own
};
