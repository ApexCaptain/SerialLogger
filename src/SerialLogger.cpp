#include <SerialLogger.hpp>

#undef max
#undef min

HardwareSerial *staticOutputSerial = nullptr;
extern "C" {
    int _write(int fd, char *ptr, int len) {
        (void) fd;
        return staticOutputSerial -> write(ptr);
    }
};

class Logger {
    private :
        bool useLevelColor;
    public :
        Logger(HardwareSerial outputSerial, bool useLevelColor) {
            staticOutputSerial = &outputSerial;
            this -> useLevelColor = useLevelColor;
            while(!staticOutputSerial);
            std::cout << std::endl;
        }
        bool needToSetColor = true;
        const Log::Level PRINTABLE_LOG_LEVEL = Log::Level::VERBOSE;
        Log::Level prevLogLevel;
        std::streambuf* originBuffer = std::cout.rdbuf();
        std::ostream& streamOutDelegate(Log::Level logLevel, int code) {
            if(logLevel <= PRINTABLE_LOG_LEVEL) {
                if(prevLogLevel != logLevel) {
                    needToSetColor = true;
                    prevLogLevel = logLevel;
                }
                std::cout.rdbuf(originBuffer);
                std::string textColor;
                std::string logLevelString;
                switch(logLevel) {
                    case Log::ERROR :
                        textColor = Log::Style::TextColor::RED;
                        logLevelString = "ERRR";
                        break;
                    case Log::WARNING :
                        textColor = Log::Style::TextColor::YELLOW;
                        logLevelString = "WARN";
                        break;
                    case Log::INFORMATION :
                        textColor = Log::Style::TextColor::GREEN;
                        logLevelString = "INFO";
                        break;
                    case Log::DEBUG :
                        textColor = Log::Style::TextColor::BLUE;
                        logLevelString = "DEBG";
                        break;
                    case Log::VERBOSE :
                        textColor = Log::Style::TextColor::CYAN;
                        logLevelString = "VERB";
                        break;
                    default :
                        return std::cout;
                }
                if(this -> useLevelColor && needToSetColor) {
                    std::cout << textColor << std::flush;
                    needToSetColor = false;
                }
                std::cout <<  "[ " << logLevelString ;
                if(code > 0) {
                    std::cout << Log::format(" - %03d", code);
                }
                std::cout << " ] : ";
            } else std::cout.rdbuf(NULL);
            return std::cout;
        }

};

Logger* logger = nullptr;;

/* ----------------------------------------------------------------------------------------------- */

void Log::init(HardwareSerial outputSerial, bool useLevelColor) {
    if(logger != nullptr) delete logger;
    logger = new Logger(outputSerial, useLevelColor);
}

std::ostream& Log::e(int code) { return error(code); }
std::ostream& Log::w(int code) { return warning(code); }
std::ostream& Log::i(int code) { return information(code); }
std::ostream& Log::d(int code) { return debug(code); }
std::ostream& Log::v(int code) { return verbose(code); }

std::ostream& Log::error(int code) { return logger -> streamOutDelegate(ERROR, code); }
std::ostream& Log::warning(int code) { return logger -> streamOutDelegate(WARNING, code); }
std::ostream& Log::information(int code) { return logger -> streamOutDelegate(INFORMATION, code); }
std::ostream& Log::debug(int code) { return logger -> streamOutDelegate(DEBUG, code); }
std::ostream& Log::verbose(int code) { return logger -> streamOutDelegate(VERBOSE, code); }

std::string Log::format(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char smallBuffer[1024];
    int size = vsnprintf(smallBuffer, sizeof smallBuffer, format, args);
    va_end(args);
    if (size < sizeof smallBuffer) 
        return std::string(smallBuffer);
    char buffer[size  + 1];
    va_start(args, format);
    vsnprintf(buffer, sizeof buffer, format, args);
    va_end(args);
    return std::string(buffer);
}

// Log--Style
void Log::Style::resetAllStyle() {
    std::cout << Log::Style::RESET_ALL << std::endl;
    logger -> needToSetColor = true;
}
const std::string Log::Style::RESET_ALL = "\033[0m";

// Log--Style--Apply
const std::string Log::Style::Apply::BOLD      = "\033[1m";
const std::string Log::Style::Apply::ITALIC    = "\033[3m";
const std::string Log::Style::Apply::UNDERLINE = "\033[4m";
const std::string Log::Style::Apply::REVERSE   = "\033[7m";

// Log--Style--Remove
const std::string Log::Style::Remove::BOLD      = "\033[22m";
const std::string Log::Style::Remove::ITALIC    = "\033[23m";
const std::string Log::Style::Remove::UNDERLINE = "\033[24m";
const std::string Log::Style::Remove::REVERSE   = "\033[27m";

// Log--Style--TextColor
const std::string Log::Style::TextColor::BLACK   = "\033[30m";
const std::string Log::Style::TextColor::RED     = "\033[31m"; 
const std::string Log::Style::TextColor::GREEN   = "\033[32m";
const std::string Log::Style::TextColor::YELLOW  = "\033[33m";
const std::string Log::Style::TextColor::BLUE    = "\033[34m";
const std::string Log::Style::TextColor::MAGENTA = "\033[35m";
const std::string Log::Style::TextColor::CYAN    = "\033[36m";
const std::string Log::Style::TextColor::WHITE   = "\033[37m";
const std::string Log::Style::TextColor::NORMAL  = "\033[39m";

// Log--Style--BackgroundColor
const std::string Log::Style::BackgroundColor::BLACK   = "\033[40m";
const std::string Log::Style::BackgroundColor::RED     = "\033[41m"; 
const std::string Log::Style::BackgroundColor::GREEN   = "\033[42m";
const std::string Log::Style::BackgroundColor::YELLOW  = "\033[43m";
const std::string Log::Style::BackgroundColor::BLUE    = "\033[44m";
const std::string Log::Style::BackgroundColor::MAGENTA = "\033[45m";
const std::string Log::Style::BackgroundColor::CYAN    = "\033[46m";
const std::string Log::Style::BackgroundColor::WHITE   = "\033[47m";
const std::string Log::Style::BackgroundColor::NORMAL  = "\033[49m";