//
//  system.h
//  Player
//
//  Created by ゾロアーク on 11/22/20.
//

#ifndef system_h
#define system_h

#include <string>

#define MKE_PLATFORM_WINDOWS 0
// #define MKE_PLATFORM_MACOS 1
#define MKE_PLATFORM_LINUX 2

#ifdef __WIN32__
#define MKE_PLATFORM MKE_PLATFORM_WINDOWS
#elif defined __linux__
#define MKE_PLATFORM MKE_PLATFORM_LINUX
#else
#error "Can't identify platform."
#endif

namespace systemImpl {
enum WineHostType {
    Windows,
    Linux
};
std::string getSystemLanguage();
std::string getUserName();
int getScalingFactor();

bool isWine();
WineHostType getRealHostType();
}

namespace mke_sys = systemImpl;

#endif /* system_h */
