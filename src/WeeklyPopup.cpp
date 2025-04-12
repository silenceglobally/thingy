#include "WeeklyPopup.hpp"

#include "Cache.hpp"
#include "Request.hpp"

WeeklyCell* WeeklyCell::create() {
    WeeklyCell* ret = new WeeklyCell("", 360, 90);
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

WeeklyPopup* WeeklyPopup::create() {
    WeeklyPopup* ret = new WeeklyPopup();
    if (ret->initAnchored(420, 280)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

WeeklyPopup::~WeeklyPopup() {
    if (m_loadingCircle) m_loadingCircle->release();
}

bool WeeklyPopup::setup() {
    geode::addSideArt(m_mainLayer, SideArt::All, SideArtStyle::PopupGold);

    CCSprite* spr = CCSprite::create("weeklyTitle.png"_spr);
    spr->setScale(0.85f);
    spr->setPosition(m_size / 2 + ccp(0, 110));

    m_mainLayer->addChild(spr);

    m_cellContainer = CCNode::create();
    m_mainLayer->addChild(m_cellContainer, 10);

    CCScale9Sprite* spr2 = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    spr2->setPosition(m_size / 2 - ccp(0, 8));
    spr2->setContentSize({ 380, 116 });
    spr2->setColor({ 0,0,0 });
    spr2->setOpacity(75);
    m_mainLayer->addChild(spr2);

    spr2 = CCScale9Sprite::create("GJ_square02.png");
    spr2->setPosition(m_size / 2 - ccp(0, 8));
    spr2->setContentSize({ 380, 116 });

    m_cellContainer->addChild(spr2, 2);

    spr = CCSprite::createWithSpriteFrameName("gj_dailyCrown_001.png");
    spr->setPosition(m_size / 2 + ccp(0, 62));

    m_cellContainer->addChild(spr, 3);

    m_loadingCircle = LoadingCircle::create();
    m_loadingCircle->setPositionY(-8);
    m_loadingCircle->setParentLayer(this);
    m_loadingCircle->retain();

    std::thread([this] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Loader::get()->queueInMainThread([this] {
            m_loadingCircle->show();
        });
    }).detach();

    m_errorLabel = CCLabelBMFont::create("Something went wrong...", "goldFont.fnt");
    m_errorLabel->setScale(0.675f);
    m_errorLabel->setPosition(m_size / 2- ccp(0, 8));
    m_errorLabel->setVisible(false);

    m_mainLayer->addChild(m_errorLabel);

    m_timeLabel = CCLabelBMFont::create("New Weekly Challenge in: 3days 2h", "goldFont.fnt");
    m_timeLabel->setScale(0.575f);
    m_timeLabel->setPosition({m_size.width / 2, 40});
    m_mainLayer->addChild(m_timeLabel);

    m_discardButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png"),
        this,
        menu_selector(WeeklyPopup::onDiscard)
    );
    m_discardButton->setScale(0.725f);
    m_discardButton->setPosition({24, 187});
    m_discardButton->setVisible(false);

    m_buttonMenu->addChild(m_discardButton, 2);

    showLoading();

    loadLevel();

    time_t now = time(nullptr);
    tm* tm_now = localtime(&now);

    int days_until_sunday = (7 - tm_now->tm_wday) % 7;
    if (days_until_sunday == 0)
        days_until_sunday = 7;

    tm target_tm = *tm_now;
    target_tm.tm_hour = 0;
    target_tm.tm_min = 0;
    target_tm.tm_sec = 0;
    target_tm.tm_mday += days_until_sunday;
    mktime(&target_tm);

    time_t target_time = mktime(&target_tm);
    double diff_seconds = difftime(target_time, now);

    int hours = diff_seconds / 3600;
    int remaining = static_cast<int>(diff_seconds) % 3600;
    int minutes = remaining / 60;
    int seconds = remaining % 60;

    log::debug("Time until next Sunday: {} hours, {} minutes, {} seconds.", hours, minutes, seconds);

    return true;
}

void WeeklyPopup::onDiscard(CCObject*) {
    if (m_cell) m_cell->removeFromParentAndCleanup(true);
    m_cell = nullptr;

    showLoading();

    Cache::setLocalWeekly(Cache::getCurrentWeekly());
    Cache::setCachedWeekly(nullptr);

    loadLevel();
}

void WeeklyPopup::showLevel(GJGameLevel* level) {
    if (!level) {
        showError();
        return;
    }

    if (m_cell) m_cell->removeFromParentAndCleanup(true);

    m_cell = WeeklyCell::create();
    m_cell->loadFromLevel(level);
    m_cell->setPosition(m_size / 2 - ccp(180, 55));

    m_cellContainer->addChild(m_cell, 4);

    if (CCMenuItemSpriteExtra* btn = typeinfo_cast<CCMenuItemSpriteExtra*>(m_cell->getChildByIDRecursive("view-button"))) {
        CCSprite* spr = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        spr->setScale(0.7f);
        btn->setSprite(spr);
    }

    m_discardButton->setVisible(Cache::getCurrentWeekly() != Cache::getLocalWeekly());

    hideLoading();
    hideError();

    m_cellContainer->setVisible(true);
}

void WeeklyPopup::loadLevel() {
    if (GJGameLevel* level = Cache::getCachedWeekly())
        showLevel(level);
    else if (Cache::getLocalWeekly() == 0)
        Request::loadWeekly(true);
    else {
        GameLevelManager* glm = GameLevelManager::sharedState();
        glm->m_levelManagerDelegate = this;
        glm->getOnlineLevels(GJSearchObject::create(SearchType::Type19, std::to_string(Cache::getLocalWeekly())));
    }
}

void WeeklyPopup::loadLevelsFinished(cocos2d::CCArray* levels, char const*, int) {
    if (levels->count() < 1) {
        showError();
        return;
    }

    GJGameLevel* level = static_cast<GJGameLevel*>(levels->objectAtIndex(0));

    Cache::setCachedWeekly(level);
    showLevel(level);
}

void WeeklyPopup::loadLevelsFailed(char const*, int) {
    showError();
}

void WeeklyPopup::showError() {
    m_cellContainer->setVisible(false);

    m_errorLabel->setVisible(true);
    m_isError = true;

    hideLoading();

    m_timeLabel->setVisible(false);
    m_discardButton->setVisible(false);
}

void WeeklyPopup::hideError() {
    m_errorLabel->setVisible(false);
    m_isError = false;
    m_timeLabel->setVisible(true);
}

void WeeklyPopup::showLoading() {
    m_cellContainer->setVisible(false);
    m_loadingCircle->setVisible(true);
    m_isLoading = true;

    hideError();

    m_timeLabel->setVisible(false);
    m_discardButton->setVisible(false);
}

void WeeklyPopup::hideLoading() {
    m_loadingCircle->setVisible(false);
    m_isLoading = false;

    m_timeLabel->setVisible(true);
}