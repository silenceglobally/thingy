#include "ChallengeListLayer.hpp"
#include "GDCP.hpp"
#include <Geode/Geode.hpp>
#include <Geode/binding/CustomListView.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GJListLayer.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/binding/InfoAlertButton.hpp>
#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/binding/SetIDPopup.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/ranges.hpp>
#include <random>

using namespace geode::prelude;

ChallengeListLayer* ChallengeListLayer::create() {
    auto ret = new ChallengeListLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCScene* ChallengeListLayer::scene() {
    auto ret = CCScene::create();
    ret->addChild(ChallengeListLayer::create());
    return ret;
}

bool ChallengeListLayer::init() {
    if (!CCLayer::init()) return false;

    setID("ChallengeListLayer");
    auto winSize = CCDirector::get()->getWinSize();

    auto bg = CCSprite::create("GJ_gradientBG.png");
    bg->setAnchorPoint({ 0.0f, 0.0f });
    bg->setScaleX((winSize.width + 10.0f) / bg->getTextureRect().size.width);
    bg->setScaleY((winSize.height + 10.0f) / bg->getTextureRect().size.height);
    bg->setPosition({ -5.0f, -5.0f });
    bg->setColor({ 51, 51, 51 });
    bg->setID("background");
    addChild(bg);

    auto bottomLeftCorner = CCSprite::createWithSpriteFrameName("gauntletCorner_001.png");
    bottomLeftCorner->setPosition({ -1.0f, -1.0f });
    bottomLeftCorner->setAnchorPoint({ 0.0f, 0.0f });
    bottomLeftCorner->setID("left-corner");
    addChild(bottomLeftCorner);

    auto bottomRightCorner = CCSprite::createWithSpriteFrameName("gauntletCorner_001.png");
    bottomRightCorner->setPosition({ winSize.width + 1.0f, -1.0f });
    bottomRightCorner->setAnchorPoint({ 1.0f, 0.0f });
    bottomRightCorner->setFlipX(true);
    bottomRightCorner->setID("right-corner");
    addChild(bottomRightCorner);

    m_countLabel = CCLabelBMFont::create("", "goldFont.fnt");
    m_countLabel->setAnchorPoint({ 1.0f, 1.0f });
    m_countLabel->setScale(0.6f);
    m_countLabel->setPosition({ winSize.width - 7.0f, winSize.height - 3.0f });
    m_countLabel->setID("level-count-label");
    addChild(m_countLabel);

    m_list = GJListLayer::create(CustomListView::create(CCArray::create(), BoomListType::Level, 190.0f, 356.0f), "GDCP Challenge List", { 0, 0, 0, 180 }, 356.0f, 220.0f, 0);
    m_list->setZOrder(2);
    m_list->setPosition(winSize / 2 - m_list->getContentSize() / 2);
    m_list->setID("GJListLayer");
    addChild(m_list);

    addSearchBar();

    auto menu = CCMenu::create();
    menu->setPosition({ 0.0f, 0.0f });
    menu->setID("button-menu");
    addChild(menu);

    m_backButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(ChallengeListLayer::keyBackClickedWrapper));
    m_backButton->setPosition({ 25.0f, winSize.height - 25.0f });
    m_backButton->setID("back-button");
    menu->addChild(m_backButton);

    m_leftButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(ChallengeListLayer::pageLeft));
    m_leftButton->setPosition({ 24.0f, winSize.height / 2 });
    m_leftButton->setID("prev-page-button");
    menu->addChild(m_leftButton);

    auto rightBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    rightBtnSpr->setFlipX(true);
    m_rightButton = CCMenuItemSpriteExtra::create(rightBtnSpr, this, menu_selector(ChallengeListLayer::pageRight));
    m_rightButton->setPosition({ winSize.width - 24.0f, winSize.height / 2 });
    m_rightButton->setID("next-page-button");
    menu->addChild(m_rightButton);

    m_infoButton = InfoAlertButton::create("Staff Team", "<cy>NatNat\nRunaia\nSadCraft0o\nFrostyToes</c>\n<cj>rr3verb\nAllu\nTsa\nDragonDude\nAgathX</c>\n<cg>Deceptive</c>", 1.0f);
    m_infoButton->setPosition({ 30.0f, 30.0f });
    m_infoButton->setID("info-button");
    menu->addChild(m_infoButton, 2);

    auto refreshBtnSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    auto refreshButton = CCMenuItemSpriteExtra::create(refreshBtnSpr, this, menu_selector(ChallengeListLayer::refresh));
    refreshButton->setPosition({ winSize.width - refreshBtnSpr->getContentWidth() / 2 - 4.0f, refreshBtnSpr->getContentHeight() / 2 + 4.0f });
    refreshButton->setID("refresh-button");
    menu->addChild(refreshButton, 2);

    auto pageBtnSpr = CCSprite::create("GJ_button_02.png");
    pageBtnSpr->setScale(0.7f);
    m_pageLabel = CCLabelBMFont::create("1", "bigFont.fnt");
    m_pageLabel->setScale(0.8f);
    m_pageLabel->setPosition(pageBtnSpr->getContentSize() / 2);
    pageBtnSpr->addChild(m_pageLabel);
    m_pageButton = CCMenuItemSpriteExtra::create(pageBtnSpr, this, menu_selector(ChallengeListLayer::pageButton));
    m_pageButton->setPositionY(winSize.height - 39.5f);
    m_pageButton->setID("page-button");
    menu->addChild(m_pageButton);
    // Sprite by Cvolton
    m_randomButton = CCMenuItemSpriteExtra::create(CCSprite::create("BI_randomBtn_001.png"_spr), this, menu_selector(ChallengeListLayer::randomPage));
    m_randomButton->setPositionY(m_pageButton->getPositionY() - m_pageButton->getContentHeight() / 2 - m_randomButton->getContentHeight() / 2 - 5.0f);
    m_randomButton->setID("random-button");
    menu->addChild(m_randomButton);

    auto lastArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    lastArrow->setFlipX(true);
    auto otherLastArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    otherLastArrow->setPosition(lastArrow->getContentSize() / 2 + CCPoint { 20.0f, 0.0f });
    otherLastArrow->setFlipX(true);
    lastArrow->addChild(otherLastArrow);
    lastArrow->setScale(0.4f);
    m_lastButton = CCMenuItemSpriteExtra::create(lastArrow, this, menu_selector(ChallengeListLayer::lastPage));
    m_lastButton->setPositionY(m_randomButton->getPositionY() - m_randomButton->getContentHeight() / 2 - m_lastButton->getContentHeight() / 2 - 5.0f);
    m_lastButton->setID("last-button");
    menu->addChild(m_lastButton);

    auto x = winSize.width - m_randomButton->getContentWidth() / 2 - 3.0f;
    m_pageButton->setPositionX(x);
    m_randomButton->setPositionX(x);
    m_lastButton->setPositionX(x - 4.0f);

    auto firstArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    auto otherFirstArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    otherFirstArrow->setPosition(firstArrow->getContentSize() / 2 - CCPoint { 20.0f, 0.0f });
    firstArrow->addChild(otherFirstArrow);
    firstArrow->setScale(0.4f);
    m_firstButton = CCMenuItemSpriteExtra::create(firstArrow, this, menu_selector(ChallengeListLayer::firstPage));
    m_firstButton->setPosition({ 21.5f, m_lastButton->getPositionY() });
    m_firstButton->setID("first-button");
    menu->addChild(m_firstButton);

    m_loadingCircle = LoadingCircle::create();
    m_loadingCircle->setParentLayer(this);
    m_loadingCircle->retain();
    m_loadingCircle->show();
    m_loadingCircle->setID("loading-circle");

    showLoading();
    setKeypadEnabled(true);
    setKeyboardEnabled(true);

    GDCP::load(std::move(m_GDCPListener), std::move(m_GDCPOkListener), [this] { populateList(""); }, failure());

    return true;
}

std::function<void(int)> ChallengeListLayer::failure() {
    auto failedString = "Failed to load GDCP Challenge List. Please try again later.";
    return [this, failedString](int code) {
        FLAlertLayer::create(fmt::format("Load Failed ({})", code).c_str(), failedString, "OK")->show();
        m_loadingCircle->setVisible(false);
    };
}

void ChallengeListLayer::addSearchBar() {
    auto winSize = CCDirector::get()->getWinSize();

    m_searchBarMenu = CCMenu::create();
    m_searchBarMenu->setContentSize({ 356.0f, 30.0f });
    m_searchBarMenu->setPosition({ 0.0f, 190.0f });
    m_searchBarMenu->setID("search-bar-menu");
    m_list->addChild(m_searchBarMenu);

    auto searchBackground = CCLayerColor::create({ 194, 114, 62, 255 }, 356.0f, 30.0f);
    searchBackground->setID("search-bar-background");
    m_searchBarMenu->addChild(searchBackground);

    if (!m_query.empty()) {
        auto searchButton = CCMenuItemSpriteExtra::create(CCSprite::create("ID_findBtnOn_001.png"_spr), this, menu_selector(ChallengeListLayer::searchLevel));
        searchButton->setPosition({ 337.0f, 15.0f });
        searchButton->setID("search-button");
        m_searchBarMenu->addChild(searchButton);
    } else {
        auto searchButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_findBtn_001.png"), this, menu_selector(ChallengeListLayer::searchLevel));
        searchButton->setPosition({ 337.0f, 15.0f });
        searchButton->setID("search-button");
        m_searchBarMenu->addChild(searchButton);
    }

    m_searchBar = TextInput::create(413.3f, "Search Demons...");
    m_searchBar->setCommonFilter(CommonFilter::Any);
    m_searchBar->setPosition({ 165.0f, 15.0f });
    m_searchBar->setTextAlign(TextInputAlign::Left);
    m_searchBar->getInputNode()->setLabelPlaceholderScale(0.53f);
    m_searchBar->getInputNode()->setMaxLabelScale(0.53f);
    m_searchBar->setScale(0.75f);
    m_searchBar->setCallback([this](const std::string& text) { m_searchBarText = text; });
    m_searchBar->setID("search-bar");
    m_searchBarMenu->addChild(m_searchBar);
}

void ChallengeListLayer::showLoading() {
    m_pageLabel->setString(std::to_string(m_page + 1).c_str());
    m_loadingCircle->setVisible(true);
    m_list->m_listView->setVisible(false);
    m_searchBarMenu->setVisible(false);
    m_countLabel->setVisible(false);
    m_leftButton->setVisible(false);
    m_rightButton->setVisible(false);
    m_firstButton->setVisible(false);
    m_lastButton->setVisible(false);
    m_pageButton->setVisible(false);
    m_randomButton->setVisible(false);
}

void ChallengeListLayer::populateList(const std::string& query) {
    m_query = query;

    if (m_fullSearchResults.empty()) {
        loadLevelsFinished(CCArray::create(), "");
        m_countLabel->setString("");
    }
    else {
        auto glm = GameLevelManager::get();
        glm->m_levelManagerDelegate = this;
        auto searchResults = std::vector<std::string>(m_fullSearchResults.begin() + m_page * 10,
            m_fullSearchResults.begin() + std::min((int)m_fullSearchResults.size(), (m_page + 1) * 10));
        auto searchObject = GJSearchObject::create(SearchType::MapPackOnClick, string::join(searchResults, ","));
        std::string key = searchObject->getKey();
        if (auto storedLevels = glm->getStoredOnlineLevels(key.substr(std::max(0, (int)key.size() - 256)).c_str())) {
            loadLevelsFinished(storedLevels, key.c_str());
            setupPageInfo("", key.c_str());
        }
        else glm->getOnlineLevels(searchObject);
    }
}

void ChallengeListLayer::loadLevelsFinished(CCArray* levels, const char*) {
    auto winSize = CCDirector::get()->getWinSize();
    if (m_list->getParent() == this) removeChild(m_list);
    m_list = GJListLayer::create(CustomListView::create(levels, BoomListType::Level, 190.0f, 356.0f), "GDCP Challenge List", { 0, 0, 0, 180 }, 356.0f, 220.0f, 0);
    m_list->setZOrder(2);
    m_list->setPosition(winSize / 2 - m_list->getContentSize() / 2);
    m_list->setID("GJListLayer");
    addChild(m_list);
    addSearchBar();
    m_searchBar->setString(m_searchBarText);
    m_countLabel->setVisible(true);
    m_loadingCircle->setVisible(false);
    if (m_fullSearchResults.size() > 10) {
        auto maxPage = (m_fullSearchResults.size() - 1) / 10;
        m_leftButton->setVisible(m_page > 0);
        m_rightButton->setVisible(m_page < maxPage);
        m_firstButton->setVisible(m_page > 0);
        m_lastButton->setVisible(m_page < maxPage);
        m_pageButton->setVisible(true);
        m_randomButton->setVisible(true);
    }
}

void ChallengeListLayer::loadLevelsFailed(const char*) {
    m_searchBarMenu->setVisible(true);
    m_countLabel->setVisible(true);
    m_loadingCircle->setVisible(false);
    FLAlertLayer::create("Load Failed", "Failed to load levels. Please try again later.", "OK")->show();
}

void ChallengeListLayer::setupPageInfo(gd::string, const char*) {
    m_countLabel->setString(fmt::format("{} to {} of {}", m_page * 10 + 1,
        std::min((int)m_fullSearchResults.size(), (m_page + 1) * 10), m_fullSearchResults.size()).c_str());
    m_countLabel->limitLabelWidth(100.0f, 0.6f, 0.0f);
}

void ChallengeListLayer::search() {
    if (m_query != m_searchBarText) {
        showLoading();
        GDCP::load(std::move(m_GDCPListener), std::move(m_GDCPOkListener), [this] {
            m_page = 0;
            populateList(m_searchBarText);
        }, failure());
    }
}

void ChallengeListLayer::page(int page) {
    auto maxPage = (m_fullSearchResults.size() + 9) / 10;
    m_page = maxPage > 0 ? (maxPage + (page % maxPage)) % maxPage : 0;
    showLoading();
    populateList(m_query);
}

void ChallengeListLayer::keyDown(enumKeyCodes key) {
    switch (key) {
        case KEY_Left:
        case CONTROLLER_Left:
            if (m_leftButton->isVisible()) page(m_page - 1);
            break;
        case KEY_Right:
        case CONTROLLER_Right:
            if (m_rightButton->isVisible()) page(m_page + 1);
            break;
        case KEY_Enter:
            search();
            break;
        default:
            CCLayer::keyDown(key);
            break;
    }
}

void ChallengeListLayer::keyBackClicked() {
    CCDirector::get()->popSceneWithTransition(0.5f, kPopTransitionFade);
}

void ChallengeListLayer::setIDPopupClosed(SetIDPopup*, int page) {
    m_page = std::min(std::max(page - 1, 0), ((int)m_fullSearchResults.size() - 1) / 10);
    showLoading();
    populateList(m_query);
}

ChallengeListLayer::~ChallengeListLayer() {
    CC_SAFE_RELEASE(m_loadingCircle);
    auto glm = GameLevelManager::get();
    if (glm->m_levelManagerDelegate == this) glm->m_levelManagerDelegate = nullptr;
}

void ChallengeListLayer::keyBackClickedWrapper(CCObject*) {
    ChallengeListLayer::keyBackClicked();
}

void ChallengeListLayer::pageLeft(CCObject*) {
    ChallengeListLayer::page(m_page - 1);
}

void ChallengeListLayer::pageRight(CCObject*) {
    ChallengeListLayer::page(m_page + 1);
}

void ChallengeListLayer::refresh(CCObject*) {
    GDCP::load(std::move(m_GDCPListener), std::move(m_GDCPOkListener), [this] { populateList(m_query); }, failure());
    ChallengeListLayer::showLoading();
}

void ChallengeListLayer::pageButton(CCObject*) {
    auto popup = SetIDPopup::create(m_page + 1, 1, (m_fullSearchResults.size() + 9) / 10, "Go to Page", "Go", true, 1, 60.0f, false, false);
    popup->m_delegate = this;
    popup->show();
}

void ChallengeListLayer::randomPage(CCObject*) {
    static std::mt19937 mt(std::random_device{}());
    ChallengeListLayer::page(std::uniform_int_distribution<int>(0, (m_fullSearchResults.size() - 1) / 10)(mt));
}

void ChallengeListLayer::lastPage(CCObject*) {
    ChallengeListLayer::page((m_fullSearchResults.size() - 1) / 10);
}

void ChallengeListLayer::firstPage(CCObject*) {
    ChallengeListLayer::page(0);
}

void ChallengeListLayer::searchLevel(CCObject*) {
    search();
}