#pragma once

#include "Includes.hpp"

class GDCPListLayer : public cocos2d::CCLayer, public LevelManagerDelegate, public SetIDPopupDelegate {

private:

    GJListLayer* m_list = nullptr;

    CustomListView* m_customListView = nullptr;

    CCLabelBMFont* m_errorMessage = nullptr;
    CCLabelBMFont* m_pageLabel = nullptr;
    CCLabelBMFont* m_pageButtonLabel = nullptr;

    LoadingCircle* m_loadingCircle = nullptr;

    CCMenuItemSpriteExtra* m_nextButton = nullptr;
    CCMenuItemSpriteExtra* m_prevButton = nullptr;
    CCMenuItemSpriteExtra* m_refreshButton = nullptr;

    InfoAlertButton* m_infoButton = nullptr;

    int m_currentPage = 0;
    
    bool m_isLoading = true;
    bool m_isError = false;
    bool m_didRefresh = false;

    CCLabelBMFont* m_pageCounter = nullptr;

    std::chrono::steady_clock::time_point m_lastRefresh = std::chrono::steady_clock::now();

    ~GDCPListLayer();

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

    void updatePageLabels();

    int getLastPage();

    virtual void loadLevelsFinished(cocos2d::CCArray* levels, char const*, int) override;
    virtual void loadLevelsFailed(char const*, int) override;
    virtual void setIDPopupClosed(SetIDPopup* popup, int id) override;

    void onGoToPage(CCObject*);
    void onLastPage(CCObject*);

public:

    static GDCPListLayer* create();

    void loadPage(const std::string& str);
    
    void showError();

    void updateButtons();

};