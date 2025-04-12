#include "Utils.hpp"
#include "Cache.hpp"
#include "GDCPListLayer.hpp"

void Utils::replace(std::string& str, char c, std::string r) {
    size_t pos = 0;
    while ((pos = str.find(c, pos)) != std::string::npos) {
        str.replace(pos, 1, r);
        pos += 3;
    }
}

GDCPListLayer* Utils::getLayer() {
    GDCPListLayer* layer = Cache::getLayer();

    if (!layer) {
        CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
        layer = scene->getChildByType<GDCPListLayer>(0);

        if (layer) Cache::setLayer(layer);
    }

    return layer;
}

int Utils::getTopForLevelId(int id) {
    for (const auto& pair : Cache::getLevelIds())
        if (pair.second == id) return pair.first + 1;

    return 0;
}

std::string Utils::getRemainingTime() {
    time_t now = time(nullptr);
    tm* tm_now = localtime(&now);

    int days_until_sunday = (7 - tm_now->tm_wday) % 7;
    if (days_until_sunday == 0)
        days_until_sunday = 7;

    tm target_tm = *tm_now;
    target_tm.tm_hour = 0;
    target_tm.tm_min = 0;
    target_tm.tm_sec = 0;
    target_tm.tm_mday += days_until_sunday;
    mktime(&target_tm);

    time_t target_time = mktime(&target_tm);
    double diff_seconds = difftime(target_time, now);

    int hours = diff_seconds / 3600;
    int remaining = static_cast<int>(diff_seconds) % 3600;
    int minutes = remaining / 60;
    int seconds = remaining % 60;

    std::string ret;

    if (hours >= 24) {
        int days = hours / 24;
        ret = fmt::format("{}day{} {}h", days, days > 1 ? "s" : "", hours % 24);
    } else if (hours > 0)
        ret = fmt::format("{}h {}min", hours, minutes);
    else if (minutes > 0)
        ret = fmt::format("{}m {}sec", minutes, seconds);
    else
        ret = fmt::format("{}sec", seconds);
    
    return ret;
}