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
    std::vector<std::string> m_levelNamesPlat;
    std::unordered_map<int, CCArray*> m_cachedPagesPlat;

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
    std::unordered_map<int, int> m_levelIdsPlat;

    static void clearAllCache() {
        Cache& c = get();
        // Classic
        c.m_levelNames.clear();
        c.m_levelIds.clear();
        c.m_cachedPages.clear();

        // Platformer
        c.m_levelNamesPlat.clear();
        c.m_levelIdsPlat.clear();
        c.m_cachedPagesPlat.clear();

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

    static const std::vector<std::string>& getLevelNamesPlat() {
        return get().m_levelNamesPlat;
    }

    static int getLevelCountPlat() {
        return get().m_levelNamesPlat.size();
    }

    static void setLevelNamesPlat(const std::vector<std::string>& names) {
        get().m_levelNamesPlat = names;
    }

    static std::unordered_map<int, int> getLevelIdsPlat() {
        return get().m_levelIdsPlat;
    }

    static int getLevelIdPlat(int index) {
        return get().m_levelIdsPlat.contains(index) ? get().m_levelIdsPlat.at(index) : 0;
    }

    static void setLevelIdPlat(int index, int id) {
        get().m_levelIdsPlat[index] = id;
    }

    static void setCachedPagePlat(int page, CCArray* levels) {
        get().m_cachedPagesPlat[page] = levels;
    }

    static CCArray* getCachedPagePlat(int page) {
        return get().m_cachedPagesPlat.contains(page) ? get().m_cachedPagesPlat.at(page) : nullptr;
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
