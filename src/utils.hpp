#include "includes.hpp"

class Utils {

public:

    static void replace(std::string& str, char c, std::string r) {
        size_t pos = 0;
        while ((pos = str.find(c, pos)) != std::string::npos) {
            str.replace(pos, 1, r);
            pos += 3;
        }
    }

    static GDCPListLayer* getLayer() {
        GDCPListLayer* layer = Cache::getLayer();

        if (!layer) {
            CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
            layer = scene->getChildByType<GDCPListLayer>(0);

            if (layer) Cache::setLayer(layer);
        }

        return layer;
    }

};