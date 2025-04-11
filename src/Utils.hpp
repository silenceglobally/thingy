#include "Includes.hpp"
#include "GDCPListLayer.hpp"

class Utils {
public:
    static void replace(std::string& str, char c, std::string r);

    static GDCPListLayer* getLayer();

    static int getTopForLevelId(int id);
};