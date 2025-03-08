#include "GDCP.hpp"
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

#define GDCP_LIST_URL "https://raw.githubusercontent.com/DeceptiveGD/Geometry-Dash-Challenge-Progression/refs/heads/main/data/_list.json"

void isOk(const std::string& url, EventListener<web::WebTask>&& listenerRef, bool head, const std::function<void(bool, int)>& callback) {
    auto&& listener = std::move(listenerRef);
    listener.bind([callback](web::WebTask::Event* e) {
        if (auto res = e->getValue()) callback(res->ok(), res->code());
    });
    listenerRef.setFilter(head ? web::WebRequest().send("HEAD", url) : web::WebRequest().downloadRange({ 0, 0 }).get(url));
}

void GDCP::load(
    EventListener<web::WebTask>&& listenerRef, EventListener<web::WebTask>&& okListener,
    const std::function<void()>& success, const std::function<void(int)>& failure
) {
    auto&& listener = std::move(listenerRef);
    listener.bind([failure, success](web::WebTask::Event* e) {
        if (auto res = e->getValue()) {
            if (!res->ok()) return failure(res->code());

            PEMONLIST_LOADED = true;
            PEMONLIST.clear();
            auto json = res->json().unwrapOr(matjson::Value());
            if (!json.isObject() || !json.contains("data") || !json["data"].isArray()) return success();

            PEMONLIST = ranges::map<std::vector<GDCPListDemon>>(ranges::filter(json["data"].asArray().unwrap(), [](const matjson::Value& level) {
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
            success();
        }
    });

    isOk(GDCP_LIST_URL, std::move(okListener), false, [&listener, failure](bool ok, int code) {
        if (ok) listener.setFilter(web::WebRequest().get(GDCP_LIST_URL));
        else failure(code);
    });
}
