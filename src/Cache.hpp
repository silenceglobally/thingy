#pragma once

#include <Geode/Geode.hpp>

#include "GDCPListLayer.hpp"

class Cache {

private:

    GDCPListLayer* m_layer = nullptr;

    GJGameLevel* m_cachedWeekly = nullptr;

    // Classic level-related
    std::vector<std::string> m_levelNames;
    std::unordered_map<int, CCArray*> m_cachedPages;

    // Platformer level-related
    std::vector<std::string> m_platLevelNames;
    std::unordered_map<int, CCArray*> m_cachedPlatPages;

    std::vector<EditorEntry> m_editorsList;
    std::string m_editors;

    int m_tempCount = 0;
    int m_currentWeekly = 0;

    static Cache& get() {
        static Cache instance;
        return instance;
    }

public:
    std::unordered_map<int, int> m_levelIds;
    std::unordered_map<int, int> m_platLevelIds;

    static void clearAllCache() {
        Cache& c = get();
        // Classic
        c.m_levelNames.clear();
        c.m_levelIds.clear();
        c.m_cachedPages.clear();

        // Platformer
        c.m_platLevelNames.clear();
        c.m_platLevelIds.clear();
        c.m_cachedPlatPages.clear();

        c.m_editors.clear();
        c.m_editorsList.clear();
        c.m_currentWeekly = 0;
        c.m_cachedWeekly = nullptr;
    }

    // --- Classic Level Functions ---

    static const std::vector<std::string>& getLevelNames() {
        return get().m_levelNames;
    }

    static int getLevelCount() {
        return get().m_levelNames.size();
    }

    static void setLevelNames(const std::vector<std::string>& names) {
        get().m_levelNames = names;
    }

    static std::unordered_map<int, int> getLevelIds() {
        return get().m_levelIds;
    }

    static int getLevelId(int index) {
        return get().m_levelIds.contains(index) ? get().m_levelIds.at(index) : 0;
    }

    static void setLevelId(int index, int id) {
        get().m_levelIds[index] = id;
    }

    static void setCachedPage(int page, CCArray* levels) {
        get().m_cachedPages[page] = levels;
    }

    static CCArray* getCachedPage(int page) {
        return get().m_cachedPages.contains(page) ? get().m_cachedPages.at(page) : nullptr;
    }

    // --- Platformer Level Functions ---

    static const std::vector<std::string>& getPlatLevelNames() {
        return get().m_platLevelNames;
    }

    static int getPlatLevelCount() {
        return get().m_platLevelNames.size();
    }

    static void setPlatLevelNames(const std::vector<std::string>& names) {
        get().m_platLevelNames = names;
    }

    static std::unordered_map<int, int> getPlatLevelIds() {
        return get().m_platLevelIds;
    }

    static int getPlatLevelId(int index) {
        return get().m_platLevelIds.contains(index) ? get().m_platLevelIds.at(index) : 0;
    }

    static void setPlatLevelId(int index, int id) {
        get().m_platLevelIds[index] = id;
    }

    static void setCachedPlatPage(int page, CCArray* levels) {
        get().m_cachedPlatPages[page] = levels;
    }

    static CCArray* getCachedPlatPage(int page) {
        return get().m_cachedPlatPages.contains(page) ? get().m_cachedPlatPages.at(page) : nullptr;
    }

    // --- Shared / Editor Functions ---

    static int getCount() {
        return get().m_tempCount;
    }

    static void addCount() {
        get().m_tempCount++;
    }

    static void setCount(int count) {
        get().m_tempCount = count;
    }

    static void setLayer(GDCPListLayer* layer) {
        get().m_layer = layer;
    }

    static GDCPListLayer* getLayer() {
        return get().m_layer;
    }

    static const std::string& getEditors() {
        return get().m_editors;
    }

    static void setEditors(const std::string& editors) {
        get().m_editors = editors;
    }

    static void setEditorsList(const std::vector<EditorEntry>& list) {
        get().m_editorsList = list;
    }

    static const std::vector<EditorEntry>& getEditorsList() {
        return get().m_editorsList;
    }

    // --- Weekly Caching ---

    static void setCurrentWeekly(int id) {
        get().m_currentWeekly = id;
    }

    static int getCurrentWeekly() {
        return get().m_currentWeekly;
    }

    static GJGameLevel* getCachedWeekly() {
        return get().m_cachedWeekly;
    }

    static void setCachedWeekly(GJGameLevel* level) {
        get().m_cachedWeekly = level;
    }

    static void setLocalWeekly(int id) {
        Mod::get()->setSavedValue("local_weekly", id);
    }

    static int getLocalWeekly() {
        return Mod::get()->getSavedValue<int64_t>("local_weekly");
    }

};
