#pragma once
#include <iostream>
#include <sstream>
#include <cassert>
#include <vector>

using CONSOLE_CODE = const char*;

struct CONSOLE_COLOR    
{
    //FORMAT
    static constexpr CONSOLE_CODE RESET{ "0" };
    static constexpr CONSOLE_CODE BOLD{ "1" };
    static constexpr CONSOLE_CODE DIM{ "2" };
    static constexpr CONSOLE_CODE ITALIC{ "3" };
    static constexpr CONSOLE_CODE UNDERLINE{ "4" };
    static constexpr CONSOLE_CODE BLINK{ "5" };    
    static constexpr CONSOLE_CODE REVERSE{ "7" };
    static constexpr CONSOLE_CODE HIDDEN{ "8" };
    static constexpr CONSOLE_CODE STRIKETHROUGH{ "9" };
    //FG COLOR
    static constexpr CONSOLE_CODE BLACK{ "30" };
    static constexpr CONSOLE_CODE RED{ "31" };
    static constexpr CONSOLE_CODE GREEN{ "32" };
    static constexpr CONSOLE_CODE YELLOW{ "33" };
    static constexpr CONSOLE_CODE BLUE{ "34" };
    static constexpr CONSOLE_CODE MAGENTA{ "35" };
    static constexpr CONSOLE_CODE CYAN{ "36" };
    static constexpr CONSOLE_CODE WHITE{ "37" };
    static constexpr CONSOLE_CODE DEFAULT{ "39" };
    //BG COLOR
    static constexpr CONSOLE_CODE BG_BLACK{ "40" };
    static constexpr CONSOLE_CODE BG_RED{ "41" };
    static constexpr CONSOLE_CODE BG_GREEN{ "42" };
    static constexpr CONSOLE_CODE BG_YELLOW{ "43" };
    static constexpr CONSOLE_CODE BG_BLUE{ "44" };
    static constexpr CONSOLE_CODE BG_MAGENTA{ "45" };
    static constexpr CONSOLE_CODE BG_CYAN{ "46" };
    static constexpr CONSOLE_CODE BG_WHITE{ "47" };
    static constexpr CONSOLE_CODE BG_DEFAULT{ "49" };
};
    

template <typename T>
void printCsv(std::ostream& os, T&& value) {
    os << std::forward<T>(value);
}

template <typename T, typename... Args>
void printCsv(std::ostream& os, T&& value, Args&&... args) {
    os << std::forward<T>(value) << " ";
    printCsv(os, std::forward<Args>(args)...);
}

template <typename... Args>
std::string LogConsole(std::vector<CONSOLE_CODE> consoleOptions, Args&&... args)
{    
    std::ostringstream result;
    result <<"\x1B[";
    for (const auto& option : consoleOptions)
        result << option <<';';
    result.seekp(result.tellp() - std::streampos{ 1 });
    result << "m";
    printCsv(result, std::forward<Args>(args)...);
    result << "\x1B[0m\n";
    return result.str();
}

#define LOG(COLOR,...) std::cout << LogConsole(COLOR,__VA_ARGS__);

#ifndef _DEBUG //vs built in macro
#define DLOG(COLOR,x) 
#define RLOG(COLOR,...) LOG(COLOR,__VA_ARGS__);
#else
#define DLOG(COLOR,...) LOG(COLOR,__VA_ARGS__);
#define RLOG(COLOR,x)
#endif



#ifndef DISABLE
#define DISABLE if(0)//disable code path
#endif // !DISABLE

#ifndef DEFAULT_BREAK//used in switch statments to indicate defualt case may be used in the future 
#define DEFAULT_BREAK default:break;
#endif // !DEFAULT_BREAK


#define CONSOLE_COLOR_TEMPLATE () printf("\x1B[31m'SampleText'033[0m ");