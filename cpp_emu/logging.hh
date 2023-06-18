#pragma once

#include <iostream>
#include <string>

namespace logging
{
enum Level {
    NONE, INF, DBG, WRN, ERR, FUCK
};

const char* levelNames[] = {
    [Level::NONE] = "None",
    [Level::INF]  = "Info",
    [Level::DBG]  = "Debug",
    [Level::WRN]  = "Warn",
    [Level::ERR]  = "Error",
    [Level::FUCK] = "Fuck"

};

static auto currentLevel =  Level::FUCK;

void setLevel(Level level) { currentLevel = level; }


void log(Level level, char* fstring, ...)
{
    va_list args1;
    va_start(args1, fstring);
    va_list args2;
    va_copy(args2, args1);
    char buf[1 + vsnprintf(NULL, 0, fstring, args1)];
    va_end(args1);
    vsnprintf(buf, sizeof(buf), fstring, args2);
    va_end(args2);

    printf("\033[0m\033[1;34m[%s]\033[0m %s", levelNames[level], buf);
}


}
