#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>

#include "Includes.hpp" 

#include "GDCPListLayer.hpp"
#include "Request.hpp"

$on_mod(Loaded) {
	Request::loadEditors();
	Request::loadLevelNames();
	Request::loadLevelNamesPlat();
	Request::loadWeekly();
}

class $modify(MyLevelSearchLayer, LevelSearchLayer) {
	bool init(int p0) {
		if (!LevelSearchLayer::init(p0)) {
			return false;
		}

		auto challengeSprite = CircleButtonSprite::create(
			CCSprite::create("listDemon.png"_spr),
			CircleBaseColor::Green,
			CircleBaseSize::SmallAlt
		);

		auto challengeButton = CCMenuItemSpriteExtra::create(
			challengeSprite,
			this,
			menu_selector(MyLevelSearchLayer::onMyButton)
		);

		auto menu = this->getChildByID("other-filter-menu");
		menu->addChild(challengeButton);

		challengeButton->setID("gdcp-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onMyButton(CCObject*) {
		auto layer = GDCPListLayer::create(false);
		auto scene = CCScene::create();
		scene->addChild(layer);

		auto transition = CCTransitionFade::create(0.5f, scene);
		
		CCDirector::sharedDirector()->pushScene(transition);
	}
};