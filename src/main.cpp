#include <Geode/modify/MenuLayer.hpp>

#include "includes.hpp" 

#include "layer.hpp"
#include "request.hpp"

$on_mod(Loaded) {
	Request::loadEditors();
	Request::loadLevelNames();
}

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(MyMenuLayer::onMyButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);

		myButton->setID("my-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onMyButton(CCObject*) {
		auto layer = Layer::create();
		auto scene = CCScene::create();
		scene->addChild(layer);

		auto transition = CCTransitionFade::create(0.f, scene);
		
		CCDirector::sharedDirector()->pushScene(transition);
	}
};