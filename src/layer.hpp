#pragma once

#include "includes.hpp"

class Layer : public cocos2d::CCLayer, public LevelManagerDelegate {

private:

    GJListLayer* m_list = nullptr;

    CustomListView* m_customListView = nullptr;

    CCLabelBMFont* m_errorMessage = nullptr;
    CCLabelBMFont* m_pageLabel = nullptr;

    LoadingCircle* m_loadingCircle = nullptr;

    CCMenuItemSpriteExtra* m_nextButton = nullptr;
    CCMenuItemSpriteExtra* m_prevButton = nullptr;
    CCMenuItemSpriteExtra* m_refreshButton = nullptr;

    InfoAlertButton* m_infoButton = nullptr;

    int m_currentPage = 0;
    
    bool m_isLoading = true;
    bool m_isError = false;
    bool m_didRefresh = false;

    std::chrono::steady_clock::time_point m_lastRefresh = std::chrono::steady_clock::now();

    ~Layer();

    bool init() override;

    void keyBackClicked() override;

    void goToPage(int page);
    void showPage(CCArray* levels);

    void onBack(CCObject*);
    void onNext(CCObject*);
    void onPrev(CCObject*);
    void onRefresh(CCObject*);

    void hideError();

    void showLoading();
    void hideLoading();

    void updatePageLabel();

    int getLastPage();

    virtual void loadLevelsFinished(cocos2d::CCArray* levels, char const*, int) override;
    virtual void loadLevelsFailed(char const*, int) override;

public:

    static Layer* create();

    void loadPage(const std::string& str);
    
    void showError();

    void updateButtons();

};