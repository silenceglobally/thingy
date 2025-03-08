#include "GDCP.hpp"
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

#define GDCP_LIST_URL "https://raw.githubusercontent.com/DeceptiveGD/Geometry-Dash-Challenge-Progression/refs/heads/main/data/_list.json"
#define GDCP_BASE_URL "https://raw.githubusercontent.com/DeceptiveGD/Geometry-Dash-Challenge-Progression/refs/heads/main/data/"

void isOk(const std::string& url, EventListener<web::WebTask>&& listenerRef, bool head, const std::function<void(bool, int)>& callback) {
    auto&& listener = std::move(listenerRef);
    listener.bind([callback](web::WebTask::Event* e) {
        if (auto res = e->getValue()) callback(res->ok(), res->code());
    });

    listenerRef.setFilter(web::WebRequest().get(url));
}

void GDCP::load(
    EventListener<web::WebTask>&& listenerRef, EventListener<web::WebTask>&& okListener,
    const std::function<void()>& success, const std::function<void(int)>& failure
) {
    std::string levelUrl = GDCP_LIST_URL;

    log::debug("Starting GDCP load with URL: {}", levelUrl);

    auto&& listener = std::move(listenerRef);
    listener.bind([failure, success, &listener, &levelUrl, &okListener](web::WebTask::Event* e) {
        if (auto res = e->getValue()) {
            log::debug("Received response for GDCP load, status: {}, code: {}", res->ok(), res->code());

            if (!res->ok()) return failure(res->code());

            GDCP_LOADED = true;
            GDCP_LIST.clear();
            auto json = res->json().unwrapOr(matjson::Value());

            if (!json.isObject() || !json.contains("data") || !json["data"].isArray()) {
                log::debug("Invalid JSON structure or missing 'data' array, loading failed");
                return success();
            }

            GDCP_LIST = ranges::map<std::vector<GDCPListDemon>>(ranges::filter(json["data"].asArray().unwrap(), [](const matjson::Value& level) {
                return level.contains("level_id") && level["level_id"].isNumber() &&
                    level.contains("name") && level["name"].isString() &&
                    level.contains("placement") && level["placement"].isNumber();
            }), [](const matjson::Value& level) {
                return GDCPListDemon {
                    (int)level["level_id"].asInt().unwrap(),
                    level["name"].asString().unwrap(),
                    (int)level["placement"].asInt().unwrap()
                };
            });

            log::debug("Successfully loaded GDCP_LIST with {} demons", GDCP_LIST.size());
            success();
        }
    });

    for (int i = 0; i < GDCP_LIST.size(); ++i) {
        const auto& level = GDCP_LIST[i];
        std::string levelName = level.name;

        log::debug("Processing level: {}, index: {}", levelName, i);

        log::debug("Level {} ({}) - ID: {}, Placement: {}", i, levelName, level.id, level.position);

        isOk(levelUrl, std::move(okListener), false, [i, levelName, &listener, &levelUrl, &okListener, failure](bool ok, int code) {
            if (ok) {
                log::debug("Level URL is valid, proceeding with GET request for level: {}", levelName);

                listener.setFilter(web::WebRequest().get(levelUrl));
            } else {
                log::debug("Failed to get level URL: {}, error code: {}", levelName, code);
                failure(code);
            }
        });
    }
}

void GDCP::loadDemonDetails(const matjson::Value& levelJson, int position) {
    if (!levelJson.isObject()) return;

    int id = levelJson["id"].asInt().unwrap();
    std::string name = levelJson["name"].asString().unwrap();

    log::debug("Loading demon details: id = {}, name = {}, position = {}", id, name, position);

    GDCPListDemon demon{
        id,
        name,
        position
    };

    GDCP_LIST.push_back(demon);

    log::debug("Demon added to GDCP_LIST: {}", name);
}
