#pragma once
#include <Geode/utils/web.hpp>

struct GDCPListDemon {
    int id;
    std::string name;
    int position;
};

struct GDCPDemonPack {
    std::string name;
    double points;
    std::vector<int> levels;
};

class GDCP {
public:
    inline static std::vector<GDCPListDemon> PEMONLIST = {};
    inline static bool PEMONLIST_LOADED = false;

    static void load(
        geode::EventListener<geode::utils::web::WebTask>&&,
        geode::EventListener<geode::utils::web::WebTask>&&,
        const std::function<void()>&,
        const std::function<void(int)>&
    );
};
