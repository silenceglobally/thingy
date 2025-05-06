#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/CommentCell.hpp>

#include "Cache.hpp"

using namespace geode::prelude;

static bool isEditor(int accountID) {
    for (const auto& editor : Cache::getEditorsList()) {
        if (editor.accountID == accountID) {
            return true;
        }
    }
    return false;
}

class $modify(ProfilePage) {
    void loadPageFromUserInfo(GJUserScore* a2) {
        ProfilePage::loadPageFromUserInfo(a2);

        if (!m_mainLayer || !a2) return;
        auto layer = m_mainLayer;

        auto username_menu = typeinfo_cast<CCMenu*>(layer->getChildByIDRecursive("username-menu"));
        if (!username_menu) return;

        if (isEditor(a2->m_accountID)) {
            auto yourBadge = CCSprite::create("modBadge.png"_spr);
            yourBadge->setID("moderator-badge"_spr);

            username_menu->addChild(yourBadge);
            username_menu->updateLayout();
        }
    }
};

class $modify(CommentCell) {
    void loadFromComment(GJComment* p0) {
        CommentCell::loadFromComment(p0);

        if (!m_mainLayer || !p0) return;
        auto layer = m_mainLayer;

        auto username_menu = typeinfo_cast<CCMenu*>(layer->getChildByIDRecursive("username-menu"));
        if (!username_menu) return;

        if (isEditor(p0->m_accountID)) {
            auto yourBadge = CCSprite::create("modBadge.png"_spr);
            yourBadge->setID("moderator-badge"_spr);

            username_menu->addChild(yourBadge);
            username_menu->updateLayout();
        }
    }
};
