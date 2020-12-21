#pragma once
#ifndef SERIAL_LOGGER_HPP
#define SERIAL_LOGGER_HPP

#include <iostream>
#include <stdio.h>
#include <HardwareSerial.h>

namespace Log {

    enum Level { ERROR, WARNING, INFORMATION, DEBUG, VERBOSE };

    std::ostream& e(int code = -1);
    std::ostream& w(int code = -1);
    std::ostream& i(int code = -1);
    std::ostream& d(int code = -1);
    std::ostream& v(int code = -1);

    std::ostream& error(int code = -1);
    std::ostream& warning(int code = -1);
    std::ostream& information(int code = -1);
    std::ostream& debug(int code = -1);
    std::ostream& verbose(int code = -1);
    
    std::string format(const char* format, ...);

    void init(HardwareSerial outputSerial, bool useLevelColor = true);

    template<typename _CharT, typename _Traits>
    inline std::basic_ostream<_CharT, _Traits>&
    endl(std::basic_ostream<_CharT, _Traits>& __os)
    { return flush(__os.put(__os.widen('\n'))); }

    class Style {
        public :

            static void resetAllStyle();
            static const std::string RESET_ALL;

            class Apply {
                public :
                    static const std::string BOLD;
                    static const std::string ITALIC;
                    static const std::string UNDERLINE;
                    static const std::string REVERSE;
            };

            class Remove {
                public :
                    static const std::string BOLD;
                    static const std::string ITALIC;
                    static const std::string UNDERLINE;
                    static const std::string REVERSE;
            };

            class TextColor {
                public :
                    static const std::string BLACK;
                    static const std::string RED;
                    static const std::string GREEN;
                    static const std::string YELLOW;
                    static const std::string BLUE;
                    static const std::string MAGENTA;
                    static const std::string CYAN;
                    static const std::string WHITE;
                    static const std::string NORMAL;
            };
            class BackgroundColor {
                public :
                    static const std::string BLACK;
                    static const std::string RED;
                    static const std::string GREEN;
                    static const std::string YELLOW;
                    static const std::string BLUE;
                    static const std::string MAGENTA;
                    static const std::string CYAN;
                    static const std::string WHITE;
                    static const std::string NORMAL;
            };
    };
};

#endif