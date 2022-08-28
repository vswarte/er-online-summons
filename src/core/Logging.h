#pragma once

#include <ctime>
#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>

namespace EROnlineSummons {
    class Logging {
    public:
        static void Init();
        static void Deinit();

        static void WriteLine(std::string message);
        static void WriteLine(const char* format, ...);

    private:
        static void allocateConsole();
        static void freeConsole();
        static void openLogFile();
        static void closeLogFile();

        static void log(std::string message);
        static void writeToConsole(std::string message);
        static void writeToFile(std::string message);

        static std::string banner();
        static std::string getTime();

        static FILE* _stdoutHandle;
        static FILE* _stderrHandle;
        static FILE* _stdinHandle;
        static std::ofstream _fileHandle;
    };
}
