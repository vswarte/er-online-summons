#include "core/Logging.h"

namespace EROnlineSummons {
    FILE* Logging::_stdoutHandle;
    FILE* Logging::_stderrHandle;
    FILE* Logging::_stdinHandle;
    std::ofstream Logging::_fileHandle;

    void Logging::Init() {
        allocateConsole();
        openLogFile();
    }

    void Logging::Deinit() {
        freeConsole();
        closeLogFile();
    }

    void Logging::WriteLine(std::string message) {
        std::ostringstream logEntry;
        logEntry << banner() << message << std::endl;
        log(logEntry.str());
    }

    void Logging::WriteLine(const char *format, ...) {
        std::ostringstream logEntry;
        char messageBuffer[1000]{};

        va_list args;
        va_start(args, format);
        vsprintf_s(&messageBuffer[0], 1000, format, args);
        va_end(args);

        logEntry << banner() << std::string(messageBuffer) << std::endl;
        log(logEntry.str());
    }

    void Logging::allocateConsole() {
        AllocConsole();
        SetConsoleTitleA("ER Online Summons");
        freopen_s(&_stdoutHandle, "CONOUT$", "w", stdout);
        freopen_s(&_stderrHandle, "CONOUT$", "w", stderr);
        freopen_s(&_stdinHandle, "CONIN$", "r", stdin);
    }

    void Logging::freeConsole() {
        std::cout.clear();
        fclose(_stdoutHandle);
        fclose(_stderrHandle);
        fclose(_stdinHandle);
        FreeConsole();
    }

    void Logging::openLogFile(){
        _fileHandle.open("er-online-summons.log");
        if (!_fileHandle.is_open()) {
            writeToConsole("Could not open file handle");
        }
    }

    void Logging::closeLogFile(){
        if (_fileHandle.is_open()) {
            _fileHandle.close();
        }
    }

    std::string Logging::banner()
    {
        std::ostringstream banner;
        banner << "[" << getTime() << " EROS] ";
        return banner.str();
    }

    std::string Logging::getTime() {
        time_t rawtime;
        struct tm timeinfo;
        char _buffer[80];

        time(&rawtime);
        localtime_s(&timeinfo, &rawtime);

        strftime(_buffer, sizeof(_buffer), "%H:%M:%S", &timeinfo);
        return { _buffer };
    }

    void Logging::writeToConsole(std::string message) {
        std::cout << message;
    }

    void Logging::writeToFile(std::string message) {
        if (_fileHandle.is_open()) {
            _fileHandle << message;
            _fileHandle.flush();
        } else {
            writeToConsole("could not write to file handle!");
        }
    }

    void Logging::log(std::string entry) {
        writeToConsole(entry);
        writeToFile(entry);
    }
}
