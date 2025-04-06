#pragma once

#include <Geode/Geode.hpp>

#include "layer.hpp"

class Cache {

private:

    Layer* m_layer = nullptr;

    std::vector<std::string> m_levelNames;
    std::unordered_map<int, int> m_levelIds;

    std::string m_editors;
    std::unordered_map<int, CCArray*> m_cachedPages;

    int m_tempCount = 0;

    static Cache& get() {
        static Cache instance;
        return instance;    
    }

public:

    static void clearAllCache() {
        Cache& c = get();
        c.m_levelNames.clear();
        c.m_levelIds.clear();
        c.m_cachedPages.clear();
        c.m_editors.clear();
    }

    static const std::vector<std::string>& getLevelNames() {
        return get().m_levelNames;
    }

    static int getLevelCount() {
        return get().m_levelNames.size();
    }

    static void setLevelNames(const std::vector<std::string>& names) {
        get().m_levelNames = names;
    }

    static int getLevelId(int index) {
        return get().m_levelIds.contains(index) ? get().m_levelIds.at(index) : 0;
    }

    static void setLevelId(int index, int id) {
        get().m_levelIds[index] = id;
    }

    static int getCount() {
        return get().m_tempCount;
    }

    static void addCount() {
        get().m_tempCount++;
    }

    static void setCount(int count) {
        get().m_tempCount = count;
    }

    static void setLayer(Layer* layer) {
        get().m_layer = layer;
    }

    static Layer* getLayer() {
        return get().m_layer;
    }

    static const std::string& getEditors() {
        return get().m_editors;
    }

    static void setEditors(const std::string& editors) {
        get().m_editors = editors;
    }

    static void setCachedPage(int page, CCArray* levels) {
        get().m_cachedPages[page] = levels;
    }

    static CCArray* getCachedPage(int page) {
        return get().m_cachedPages.contains(page) ? get().m_cachedPages.at(page) : nullptr;
    }

};
