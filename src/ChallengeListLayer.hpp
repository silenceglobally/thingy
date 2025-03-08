#include <Geode/Geode.hpp>
#include <Geode/binding/SetIDPopupDelegate.hpp>
#include <Geode/binding/LevelManagerDelegate.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class ChallengeListLayer : public cocos2d::CCLayer, SetIDPopupDelegate, LevelManagerDelegate {
public:
    static ChallengeListLayer* create();
    static cocos2d::CCScene* scene();

    void search();
    void page(int);
    void keyDown(cocos2d::enumKeyCodes) override;
    void keyBackClicked() override;

    ~ChallengeListLayer() override;

    geode::EventListener<geode::utils::web::WebTask> m_GDCPListener;
    geode::EventListener<geode::utils::web::WebTask> m_GDCPOkListener;
    GJListLayer* m_list;
    cocos2d::CCLabelBMFont* m_listLabel;
    LoadingCircle* m_loadingCircle;
    cocos2d::CCMenu* m_searchBarMenu;
    geode::TextInput* m_searchBar;
    cocos2d::CCLabelBMFont* m_countLabel;
    cocos2d::CCLabelBMFont* m_pageLabel;
    InfoAlertButton* m_infoButton;
    CCMenuItemSpriteExtra* m_backButton;
    CCMenuItemSpriteExtra* m_leftButton;
    CCMenuItemSpriteExtra* m_rightButton;
    CCMenuItemSpriteExtra* m_pageButton;
    CCMenuItemSpriteExtra* m_randomButton;
    CCMenuItemSpriteExtra* m_firstButton;
    CCMenuItemSpriteExtra* m_lastButton;
    int m_page = 0;
    std::string m_query = "";
    std::string m_searchBarText = "";
    std::vector<std::string> m_fullSearchResults;
    bool init() override;
    std::function<void(int)> failure();
    void addSearchBar();
    void showLoading();
    void populateList(const std::string& query);
    void loadLevelsFinished(cocos2d::CCArray*, const char*) override;
    void loadLevelsFailed(const char*) override;
    void loadLevelsFinished(cocos2d::CCArray* levels, const char* key, int) override {
        loadLevelsFinished(levels, key);
    }
    void loadLevelsFailed(const char* key, int) override {
        loadLevelsFailed(key);
    }
    void setupPageInfo(gd::string, const char*) override;
    void setIDPopupClosed(SetIDPopup*, int) override;
    void keyBackClickedWrapper(CCObject*);

    void pageLeft(CCObject*);
    void pageRight(CCObject*);

    void refresh(CCObject*);
    
    void pageButton(CCObject*);

    void randomPage(CCObject*);
    void lastPage(CCObject*);
    void firstPage(CCObject*);

    void searchLevel(CCObject*);
};