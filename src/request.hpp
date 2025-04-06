#include "includes.hpp" 

#include "layer.hpp"

class Request {

private:
        
    static std::string getLevelsString(int page);

    static void loadPageLevels(int page);
    
public:

    static void loadLevelNames(bool shouldLoadLevels = false, int page = 0);

    static void loadPage(int page);
    
    static void loadEditors(bool shouldUpdateButtons = false);

};