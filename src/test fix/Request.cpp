#include <Geode/utils/web.hpp>

#include "Cache.hpp"
#include "Request.hpp"
#include "Utils.hpp"

void Request::loadPage(int page) {
    if (Cache::getLevelNames().empty())
        return loadLevelNames(true, page);

    loadPageLevels(page);
}

void Request::loadLevelNames(bool shouldLoadLevels, int page) {

    if (!Cache::getLevelNames().empty() && shouldLoadLevels)
        return loadPageLevels(page);

    auto req = web::WebRequest();

    req.header("Content-Type", "application/json");

    req.get(fmt::format("{}{}.json", listLink, "_list")).listen([shouldLoadLevels, page] (web::WebResponse* e) {
        auto res = e->json();
        GDCPListLayer* layer = Utils::getLayer();

        if (res.isErr()) {
            if (layer) layer->showError();
            
            return log::info("1. Failed to load level names: {}", res.unwrapErr());
        }

        auto json = res.unwrap().asArray();

        if (json.isErr()) {
            if (layer) layer->showError();
            
            return log::info("2. Failed to load level names: {}", json.unwrapErr());
        }

        std::vector<std::string> names;
        std::vector<std::string> realnames;

        for (auto& value : json.unwrap()) {
            std::string name = value.asString().unwrapOr("");
            if (name.empty()) continue;
            names.push_back(name);
        }

        Cache::setLevelNames(names);

        if (shouldLoadLevels)
            loadPageLevels(page);
        
    });
}

void Request::loadPageLevels(int page) {
    std::vector<std::string> names = Cache::getLevelNames();

    Cache::setCount(0);

    for (int i = page * levelsPerPage; i <= page * levelsPerPage + levelsPerPage - 1; i++)
        if (i >= names.size()) Cache::addCount();
    
    for (int i = page * levelsPerPage; i <= page * levelsPerPage + levelsPerPage - 1; i++) {
        if (i >= names.size()) continue;

        if (Cache::getLevelId(i) != 0) {
            Cache::addCount();

            if (Cache::getCount() >= levelsPerPage)
                if (GDCPListLayer* layer = Utils::getLayer())
                    layer->loadPage(getLevelsString(page));

            continue;
        }

        auto req = web::WebRequest();
        req.header("Content-Type", "application/json");

        std::string name = names[i];

        Utils::replace(name, ' ', "%20");

        req.get(fmt::format("{}{}.json", listLink, name)).listen([i, page](web::WebResponse* e) {
            auto res = e->json();
            GDCPListLayer* layer = Utils::getLayer();

            if (res.isErr()) {
                if (layer) layer->showError();

                return log::info("1. Failed to load page levels: {}", res.unwrapErr());
            }

            auto json = res.unwrap();

            int id = json["id"].asInt().unwrapOr(0);

            if (id == 0) {
                if (layer) layer->showError();

                return log::info("2. Failed to load page levels: {}", res.unwrapErr());
            }

            Cache::addCount();
            Cache::setLevelId(i, id);

            if (Cache::getCount() >= levelsPerPage && layer)
                layer->loadPage(getLevelsString(page));
            
        });
    }
}

std::string Request::getLevelsString(int page) {
    std::string str;
    for (int i = page * levelsPerPage; i <= page * levelsPerPage + levelsPerPage - 1; i++) {
        if (i >= Cache::getLevelNames().size()) continue;

        int id = Cache::getLevelId(i);

        if (id == 0) {
            log::info("3. Failed to load page levels: Failed to find level");
            return "";
        }
        
        str += std::to_string(id) + (i == page * levelsPerPage + levelsPerPage - 1 ? "" : ",");
    }

    return str;
}

void Request::loadEditors(bool shouldUpdateButtons) {
    if (!Cache::getEditors().empty()) return;
    
    auto req = web::WebRequest();
    req.header("Content-Type", "application/json");

    req.get(fmt::format("{}{}.json", listLink, "_editors")).listen([shouldUpdateButtons](web::WebResponse* e) {
        auto res = e->json();

        if (res.isErr())
            return log::info("1. Failed to load editors: {}", res.unwrapErr());

        auto arr = res.unwrap().asArray();

        if (arr.isErr())
            return log::info("2. Failed to load editors: {}", arr.unwrapErr());

        std::unordered_map<Role, std::vector<std::string>> names;
        std::vector<EditorEntry> editorEntries;

        for (const matjson::Value& value : arr.unwrap()) {
            std::string roleStr = value["role"].asString().unwrapOr("");
            std::string name = value["name"].asString().unwrapOr("");
            int accountID = value["accountID"].asInt().unwrap();

            if (roleStr.empty() || name.empty()) continue;

            Role role = Role::Other;
            if (roleMap.contains(roleStr))
                role = roleMap.at(roleStr);

            names[role].push_back(name);
            editorEntries.push_back({ name, role, accountID });
        }

        std::string editors;

        for (const auto& [role, roleNames] : names) {
            editors += fmt::format("<c{}>", roleColors.at(role));
            for (const auto& name : roleNames) {
                editors += name;
                if (name != roleNames.back()) editors += "\n";
            }
            editors += "</c>\n";
        }

        Cache::setEditors(editors);
        Cache::setEditorsList(editorEntries);

        if (shouldUpdateButtons)
            if (GDCPListLayer* layer = Utils::getLayer())
                layer->updateButtons();
    });
}