/*
 ** config.h
 **
 ** This file is part of mke.
 **
 ** Copyright (C) 2013 Jonas Kulla <Nyocurio@gmail.com>
 **
 ** mke is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** mke is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with mke.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "json5pp.hpp"

#include <set>
#include <string>
#include <vector>

struct Config {
    // Used for sending the JSON data to Ruby as System::CONFIG
    json5pp::value raw;
    
    int rgssVersion;
    
    bool debugMode;
    bool winConsole;
    bool preferMetalRenderer;
    bool printFPS;
    
    bool winResizable;
    bool fullscreen;
    bool fixedAspectRatio;
    bool smoothScaling;
    bool vsync;
    
    int defScreenW;
    int defScreenH;
    std::string windowTitle;
    
    int fixedFramerate;
    bool frameSkip;
    bool syncToRefreshrate;
    
    bool solidFonts;
    
    bool subImageFix;
    bool enableBlitting;
    int maxTextureSize;
    
    struct {
        bool active;
        bool lastMileScaling;
    } integerScaling;
    
    std::string gameFolder;
    bool manualFolderSelect;
    
    bool anyAltToggleFS;
    bool enableReset;
    bool enableSettings;
    bool allowSymlinks;
    bool pathCache;
    
    std::string dataPathOrg;
    std::string dataPathApp;
    
    std::string iconPath;
    std::string execName;
    std::string titleLanguage;
    
    struct {
        std::string soundFont;
        bool chorus;
        bool reverb;
    } midi;
    
    struct {
        int sourceCount;
    } SE;
    
    bool useScriptNames;
    
    std::string customScript;
    
    std::vector<std::string> launchArgs;
    std::vector<std::string> preloadScripts;
    std::vector<std::string> rtps;
    
    std::vector<std::string> fontSubs;
    
    std::vector<std::string> rubyLoadpaths;
    
    /* Editor flags */
    struct {
        bool debug;
        bool battleTest;
    } editor;
    
    /* Game INI contents */
    struct {
        std::string scripts;
        std::string title;
    } game;
    
    // JIT Options
    struct {
        bool enabled;
        int verboseLevel;
        int maxCache;
        int minCalls;
    } jit;
    
    // Keybinding action name mappings
    struct {
        std::string a;
        std::string b;
        std::string c;
        
        std::string x;
        std::string y;
        std::string z;
        
        std::string l;
        std::string r;
    } kbActionNames;
    
    std::string userConfPath;
    
    /* Internal */
    std::string customDataPath;
    
    Config();
    
    void read(int argc, char *argv[]);
    void readGameINI();
};

#endif // CONFIG_H
