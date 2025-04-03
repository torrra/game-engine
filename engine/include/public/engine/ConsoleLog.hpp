#pragma once

#pragma region Standard

#include <stdio.h>

#pragma endregion

namespace engine
{
    enum EBGColor : uint8_t
    {
        BG_BLACK = 40,
        BG_RED,
        BG_GREEN,
        BG_YELLOW,
        BG_BLUE,
        BG_MAGENTA,
        BG_CYAN,
        BG_WHITE,
        BG_BR_BLACK = 100,
        BG_BR_RED,
        BG_BR_GREEN,
        BG_BR_YELLOW,
        BG_BR_BLUE,
        BG_BR_MAGENTA,
        BG_BR_CYAN,
        BG_BR_WHITE

    }; // !Enum EBGColor

    enum EFGColor : uint8_t
    {
        FG_BLACK = 30,
        FG_RED,
        FG_GREEN,
        FG_YELLOW,
        FG_BLUE,
        FG_MAGENTA,
        FG_CYAN,
        FG_WHITE,
        FG_BR_BLACK = 90,
        FG_BR_RED,
        FG_BR_GREEN,
        FG_BR_YELLOW,
        FG_BR_BLUE,
        FG_BR_MAGENTA,
        FG_BR_CYAN,
        FG_BR_WHITE

    }; // !Enum EFGColor

    enum EGRP
    {
        NORMAL,
        BOLD, 
        THIN, 
        ITALIC, 
        UNDERLINE, 
        BLINK

    }; // !Enum EGRP

    struct ConsoleLog
    {
        EBGColor    m_bgColor       = EBGColor::BG_BLACK;
        EFGColor    m_fgColor       = EFGColor::FG_WHITE;
        EGRP        m_textFormat;
        std::string m_prefix        = "[]";

    }; // !Struct ConsoleLog

    void PrintLog(ConsoleLog consoleLog, const std::string& message)
    {
        printf("\033[%d;%d;%dm%s %s\033[0m\n", (int)consoleLog.m_textFormat, (int)consoleLog.m_bgColor,
               (int)consoleLog.m_fgColor, consoleLog.m_prefix.c_str(), message.c_str());
    }

    ConsoleLog ErrorPreset()
    {
        ConsoleLog consoleLog;

        consoleLog.m_bgColor    = EBGColor::BG_BLACK;
        consoleLog.m_fgColor    = EFGColor::FG_RED;
        consoleLog.m_textFormat = EGRP::NORMAL;
        consoleLog.m_prefix     = "[ERROR]";

        return consoleLog;
    }

    ConsoleLog WarningPreset()
    {
        ConsoleLog consoleLog;

        consoleLog.m_bgColor    = EBGColor::BG_BLACK;
        consoleLog.m_fgColor    = EFGColor::FG_YELLOW;
        consoleLog.m_textFormat = EGRP::NORMAL;
        consoleLog.m_prefix     = "[WARNING]";

        return consoleLog;
    }

    ConsoleLog InfoPreset()
    {
        ConsoleLog consoleLog;

        consoleLog.m_bgColor    = EBGColor::BG_BLACK;
        consoleLog.m_fgColor    = EFGColor::FG_BLUE;
        consoleLog.m_textFormat = EGRP::NORMAL;
        consoleLog.m_prefix     = "[INFO]";

        return consoleLog;
    }

    ConsoleLog SuccessPreset()
    {
        ConsoleLog consoleLog;

        consoleLog.m_bgColor    = EBGColor::BG_BLACK;
        consoleLog.m_fgColor    = EFGColor::FG_GREEN;
        consoleLog.m_textFormat = EGRP::NORMAL;
        consoleLog.m_prefix     = "[SUCCESS]";

        return consoleLog;
    }

} // !Namespace engine