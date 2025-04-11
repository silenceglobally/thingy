#include "Geode/cocos/textures/CCTexture2D.h"
#include "Includes.hpp"
#include "DoubleArrow.h"

#include "GDCPListLayer.hpp"
#include "Request.hpp"
#include "Cache.hpp"

GDCPListLayer::~GDCPListLayer() {
    Cache::setLayer(nullptr);
}

GDCPListLayer* GDCPListLayer::create() {
    GDCPListLayer* ret = new GDCPListLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void GDCPListLayer::keyBackClicked() {
    GameLevelManager::sharedState()->m_levelManagerDelegate = nullptr;
    CCDirector::get()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

bool GDCPListLayer::init() {

    Cache::setLayer(this);

    this->setKeypadEnabled(true);
    cocos::handleTouchPriority(this, true);

    CCSprite* backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    backgroundSprite->setID("background");
    
    cocos2d::CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    cocos2d::CCSize size = backgroundSprite->getContentSize();
    backgroundSprite->setScaleX(winSize.width / size.width);
    backgroundSprite->setScaleY(winSize.height / size.height);
    
    backgroundSprite->setAnchorPoint({0, 0});
    backgroundSprite->setColor({100, 100, 100});
    backgroundSprite->setZOrder(-1);
    addChild(backgroundSprite); 

    CCMenu* menu = CCMenu::create();
    menu->setPosition({0, 0});
    menu->setAnchorPoint({ 0.f, 0.f });
    addChild(menu);

    CCSprite* spr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    CCMenuItemSpriteExtra* button = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(GDCPListLayer::onBack)
    );
    button->setPosition({25, winSize.height - 25});

    menu->addChild(button);

    CCSprite* bottomLeft = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    bottomLeft->setPosition({-1,-1});
    bottomLeft->setAnchorPoint({0,0});
    addChild(bottomLeft, -1);

    CCSprite* bottomRight = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    bottomRight->setPosition({winSize.width + 1, -1});
    bottomRight->setAnchorPoint({1,0});
    bottomRight->setFlipX(true);
    addChild(bottomRight, -1);
    
    m_loadingCircle = LoadingCircle::create();
    m_loadingCircle->setParentLayer(this);
    m_loadingCircle->retain();
    m_loadingCircle->show();

    m_errorMessage = CCLabelBMFont::create("Unable to load page", "bigFont.fnt");
    m_errorMessage->setPosition(winSize / 2);
    m_errorMessage->setVisible(false);
    m_errorMessage->setOpacity(125);
    m_errorMessage->setScale(0.73f);
    addChild(m_errorMessage, 2);

    spr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    m_refreshButton = CCMenuItemSpriteExtra::create(spr,
        this,
        menu_selector(GDCPListLayer::onRefresh)
    );
    m_refreshButton->setCascadeOpacityEnabled(true);
    m_refreshButton->setPosition({winSize.width - 27, 27});

    menu->addChild(m_refreshButton);

    spr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    spr->setFlipX(true);

    m_nextButton = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(GDCPListLayer::onNext)
    );
    m_nextButton->setCascadeOpacityEnabled(true);
    m_nextButton->setPosition({winSize.width - 24.f, winSize.height / 2.f});

    menu->addChild(m_nextButton);

    spr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");

    m_prevButton = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(GDCPListLayer::onPrev)
    );
    m_prevButton->setCascadeOpacityEnabled(true);
    m_prevButton->setPosition({24.f, winSize.height / 2.f});

    menu->addChild(m_prevButton);
    
    std::string editors = Cache::getEditors();

    m_infoButton = InfoAlertButton::create("Staff Team", editors, 1.0f);
    m_infoButton->setPosition({ 30.0f, 30.0f });
    menu->addChild(m_infoButton);

    m_pageLabel = CCLabelBMFont::create("", "goldFont.fnt");
    m_pageLabel->setPosition({winSize.width - 2.f, winSize.height - 2.f});
    m_pageLabel->setAnchorPoint({1.f, 1.f});
    m_pageLabel->setScale(0.56f);
    addChild(m_pageLabel);

    if (editors.empty()) m_infoButton->setVisible(false);

    m_list = GJListLayer::create(nullptr, "GDCP Challenge List", {191, 114, 62, 255}, 356.f, 220.f, 0);
    m_list->setPosition(winSize / 2 - m_list->getScaledContentSize() / 2 - CCPoint(0,5));
    addChild(m_list);

    showLoading();
    
    GDCPListLayer::goToPage(m_currentPage);

    CCLabelBMFont* m_pageCounter = CCLabelBMFont::create("1", "bigFont.fnt");

    m_goToPageButton = CCMenuItemSpriteExtra::create(
        EditorButtonSprite::create(m_pageCounter, EditorBaseColor::Cyan, EditorBaseSize::Normal),
        this,
        menu_selector(GDCPListLayer::onGoToPage)
    );
    m_goToPageButton->setScale(0.75f);

    auto lastPageButton = CCMenuItemSpriteExtra::create(
        DoubleArrow::create(true, "GJ_arrow_03_001.png"),
        this,
        menu_selector(GDCPListLayer::onLastPage)
    );

    
    float labelBottom = m_pageLabel->getPositionY() - m_pageLabel->getContentSize().height;
    float nextTop = m_nextButton->getPositionY() + m_nextButton->getContentSize().height;
    
    if (nextTop < labelBottom) std::swap(labelBottom, nextTop);
    
    float menuHeight = nextTop - labelBottom; // labelBottom + m_pageLabel->getContentSize().height
    
    auto rightMenu = CCMenu::create();
    rightMenu->setContentSize({ m_goToPageButton->getContentSize().width, menuHeight });
    rightMenu->setPosition(winSize.width - m_goToPageButton->getContentSize().width * 1.25f, m_pageLabel->getPositionY() - m_pageLabel->getContentSize().height); //  - rightMenu->getContentSize().height
    rightMenu->setLayout(SimpleAxisLayout::create(Axis::Column)->setGap(5.f));
    rightMenu->setAnchorPoint({ 0.f, 1.f });
    
    rightMenu->addChild(m_goToPageButton);
    rightMenu->addChild(lastPageButton);
    this->addChild(rightMenu);    

    rightMenu->updateLayout();
    updatePageLabel();
    updateGoToPage();

    return true;
}

int GDCPListLayer::getLastPage() {
    return (Cache::getLevelCount() + levelsPerPage) / levelsPerPage;
}

void GDCPListLayer::onBack(CCObject*) {
    keyBackClicked();
}

void GDCPListLayer::onRefresh(CCObject*) {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - m_lastRefresh).count();

    if (elapsed < (3 + (std::rand() % 3)) && !m_isError && m_didRefresh) return;

    m_lastRefresh = currentTime;
    m_didRefresh = true;

    Cache::clearAllCache();

    Request::loadEditors(true);

    goToPage(m_currentPage);

    hideError();
    showLoading();
    updateButtons();
}

void GDCPListLayer::goToPage(int page) {
    if (CCArray* cachedPage = Cache::getCachedPage(page))
        showPage(cachedPage);
    else
        Request::loadPage(page);
}

void GDCPListLayer::showPage(cocos2d::CCArray* levels) {
    if (levels->count() == 0)
        return showError();

    m_customListView = CustomListView::create(levels, BoomListType::Level, 220.0, 356.0);
    m_list->addChild(m_customListView);

    hideLoading();
    hideError();
    updateButtons();
}

void GDCPListLayer::loadPage(const std::string& str) {
    if (str.empty())
        return showError();

    GameLevelManager* glm = GameLevelManager::sharedState();
    glm->m_levelManagerDelegate = this;
    glm->getOnlineLevels(GJSearchObject::create(SearchType::Type19, str));
}

void GDCPListLayer::loadLevelsFinished(cocos2d::CCArray* levels, char const*, int) {
    Cache::setCachedPage(m_currentPage, levels);
    showPage(levels);
}

void GDCPListLayer::loadLevelsFailed(char const*, int) {
    showError();
}

void GDCPListLayer::onNext(CCObject*) {
    m_currentPage++;

    showLoading();

    goToPage(m_currentPage);

    updateButtons();
    updatePageLabel();
    updateGoToPage();
}

void GDCPListLayer::onPrev(CCObject*) {
    m_currentPage--;

    if (m_currentPage < 0) m_currentPage = 0;

    showLoading();

    goToPage(m_currentPage);

    updateButtons();
    updatePageLabel();
    updateGoToPage();
}

void GDCPListLayer::updatePageLabel() {
    int pageMax = m_currentPage * levelsPerPage + levelsPerPage;
    m_pageLabel->setString(fmt::format(
        "{} to {} of {}",
        m_currentPage * levelsPerPage + 1,
        pageMax > Cache::getLevelCount() ? Cache::getLevelCount() : pageMax,
        Cache::getLevelCount()
    ).c_str());
}

void GDCPListLayer::updateButtons() {
    bool enableArrows = !m_isLoading;

    m_nextButton->setEnabled(enableArrows && m_currentPage != getLastPage() - 1);
    m_nextButton->setOpacity(enableArrows && m_currentPage != getLastPage() - 1 ? 255 : 125);

    m_prevButton->setEnabled(enableArrows && m_currentPage != 0);
    m_prevButton->setOpacity(enableArrows && m_currentPage != 0 ? 255 : 125);

    m_refreshButton->setEnabled(!m_isLoading);
    m_refreshButton->setOpacity(!m_isLoading ? 255 : 200);

    std::string editors = Cache::getEditors();
    m_infoButton->m_description = editors;

    m_infoButton->setEnabled(!editors.empty());
    m_infoButton->setOpacity(!editors.empty() ? 255 : 125);
}

void GDCPListLayer::showError() {
    m_errorMessage->setVisible(true);
    m_isError = true;

    hideLoading();
    updateButtons();
}

void GDCPListLayer::hideError() {
    m_errorMessage->setVisible(false);
    m_isError = false;

    updateButtons();
}

void GDCPListLayer::showLoading() {
    if (m_customListView) {
        m_customListView->removeFromParentAndCleanup(true);
        m_customListView = nullptr;
    }

    m_loadingCircle->setVisible(true);
    m_isLoading = true;

    hideError();
    updateButtons();
}

void GDCPListLayer::hideLoading() {
    m_loadingCircle->setVisible(false);
    m_isLoading = false;

    updateButtons();
}

void GDCPListLayer::onGoToPage(CCObject* sender) {
    auto popup = SetIDPopup::create(m_currentPage + 1, 1, getLastPage(), "Go to Page", "Search", true, 0, 0.f, true, true);
    popup->show();

    popup->m_delegate = this;
}

void GDCPListLayer::setIDPopupClosed(SetIDPopup* popup, int id) {
    if (id < 0 || id > getLastPage()) return;
    
    showLoading();
    m_currentPage = id - 1;
    goToPage(m_currentPage);
    updateButtons();
    updatePageLabel();
    updateGoToPage();
}

void GDCPListLayer::onLastPage(CCObject* sender) {
    showLoading();
    m_currentPage = getLastPage() - 1;
    goToPage(m_currentPage);
    updateButtons();
    updatePageLabel();
    updateGoToPage();
}

void GDCPListLayer::updateGoToPage() {
    if (m_goToPageButton) {
        if (auto label = m_goToPageButton->getChildByType<EditorButtonSprite*>(0)->getChildByType<CCLabelBMFont*>(0)) {
            label->setString(std::to_string(m_currentPage + 1).c_str());
        } else {
            log::warn("No label of type CCLabelBMFont found in m_goToPageButton.");
        }
    } else {
        log::warn("m_goToPageButton is null!");
    }
}