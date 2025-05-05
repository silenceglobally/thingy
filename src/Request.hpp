#pragma once

#include "Includes.hpp"
#include "GDCPListLayer.hpp"

class Request {

private:

    static std::string getLevelsString(int page);
    static std::string getPlatLevelsString(int page);

    static void loadPageLevels(int page);
    static void loadPagePlatLevels(int page);

public:

    static void loadLevelNames(bool shouldLoadLevels = false, int page = 0);
    static void loadPlatLevelNames(bool shouldLoadLevels = false, int page = 0);

    static void loadPage(int page);
    static void loadPlatPage(int page);

    static void loadEditors(bool shouldUpdateButtons = false);

    static void loadWeekly(bool shouldUpdate = false);
};
