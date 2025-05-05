#pragma once

#include "Includes.hpp"

class GDCPListLayer : public cocos2d::CCLayer, public LevelManagerDelegate, public SetIDPopupDelegate {

private:

    GJListLayer* m_list = nullptr;

    CustomListView* m_customListView = nullptr;

    CCLabelBMFont* m_errorLabel = nullptr;
    CCLabelBMFont* m_pageLabel = nullptr;
    CCLabelBMFont* m_pageButtonLabel = nullptr;

    LoadingCircle* m_loadingCircle = nullptr;

    CCMenuItemSpriteExtra* m_nextButton = nullptr;
    CCMenuItemSpriteExtra* m_prevButton = nullptr;
    CCMenuItemSpriteExtra* m_refreshButton = nullptr;
    CCMenuItemSpriteExtra* m_infoButton = nullptr;

    int m_currentPage = 0;
    
    bool m_isLoading = true;
    bool m_isError = false;
    bool m_didRefresh = false;
    bool m_isPlatformer = false;

    CCLabelBMFont* m_pageCounter = nullptr;

    std::chrono::steady_clock::time_point m_lastRefresh = std::chrono::steady_clock::now();

    ~GDCPListLayer();

    bool init(bool platformer);

    void keyBackClicked() override;

    void goToPage(int page);
    void showPage(CCArray* levels);

    void onBack(CCObject*);
    void onInfo(CCObject*);
    void onNext(CCObject*);
    void onPrev(CCObject*);
    void onRefresh(CCObject*);
    void onGoToPage(CCObject*);
    void onLastPage(CCObject*);
    void onWeekly(CCObject*);

    void hideError();

    void showLoading();
    void hideLoading();

    void updatePageLabels();

    int getLastPage();

    virtual void loadLevelsFinished(cocos2d::CCArray* levels, char const*, int) override;
    virtual void loadLevelsFailed(char const*, int) override;
    virtual void setIDPopupClosed(SetIDPopup* popup, int id) override;

public:

    static GDCPListLayer* create(bool platformer);

    void loadPage(const std::string& str);
    
    void showError();

    void updateButtons();

    void onPlatformer(CCObject*);

    void onClassic(CCObject*);

};