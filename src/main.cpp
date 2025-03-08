#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include "ChallengeListLayer.hpp"

using namespace geode::prelude;

class $modify(MyLevelSearchLayer, LevelSearchLayer) {
	bool init(int p0) {
		if (!LevelSearchLayer::init(p0)) {
			return false;
		}

		log::debug("Hello from my LevelSearchLayer::init hook! This layer has {} children.", this->getChildrenCount());

		auto challengeSprite = CircleButtonSprite::create(
			CCSprite::create("list-demon.png"_spr),
			CircleBaseColor::Green,
			CircleBaseSize::SmallAlt
		);

		auto challengeBtn = CCMenuItemSpriteExtra::create(
			challengeSprite,
			this,
			menu_selector(MyLevelSearchLayer::onChallengeList)
		);

		auto menu = this->getChildByID("other-filter-menu");
		if (!menu) {
			log::error("Failed to retrieve 'other-filter-menu'. Button not added.");
			return true;
		}
		menu->addChild(challengeBtn);

		challengeBtn->setID("challenge-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onChallengeList(CCObject*) {
		auto scenePrev = CCTransitionFade::create(0.5f, ChallengeListLayer::scene());
        CCDirector::sharedDirector()->pushScene(scenePrev);
	}
};