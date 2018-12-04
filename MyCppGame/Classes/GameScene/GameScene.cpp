#include "GameScene.h"

Scene* GameScene::createScene()
{
	return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	// add "GameScene" splash screen"
	auto sprite = Sprite::create("GameScene.jpg");
	if (sprite == nullptr)
	{
		problemLoading("'GameScene.jpg'");
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}

	//initialize gameLogic 
	gameLogic = GameLogic::create(this, visibleSize);
	this->addChild(gameLogic , 1);

	//add Sprites
	initMenuClose();
	initLabels();

	menuLablel();

	this->scheduleUpdate();
	return true;
}

void GameScene::update(float dt)
{
	Scene::update(dt);

	auto menu = dynamic_cast<Menu*> (this->getChildByName("menu"));
	auto remove = dynamic_cast<MenuItem*>(menu->getChildByName("remove"));

	if (gameLogic->robotNumber() <= 0)
		remove->setEnabled(false);
	else remove->setEnabled(true);

	auto score = dynamic_cast<Label*> (this->getChildByName("score"));
	score->setString(StringUtils::format("Score: %d", gameLogic->getScore() ));
}

void GameScene::initLabels()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Van snake", "fonts/Marker Felt.ttf", 24);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	auto label_score = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 18);
	if (label_score == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		label_score->setPosition(label->getPosition() + Vec2(0, -label_score->getContentSize().height));
		this->addChild(label_score, 1);
	}
	label_score->setName("score");
}

void GameScene::initMenuClose()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameScene::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}



void GameScene::menuLablel()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
/*
	auto major_add = MenuItemLabel::create(Label::createWithSystemFont("add Body", "", 25),
		[=](Ref* pSender) {	gameLogic.object->addBody();}	);

	auto major_remove = MenuItemLabel::create(Label::createWithSystemFont("remove Body", "", 25),
		[=](Ref* pSender) {	gameLogic.object->removeBody(); });


	auto reset_menuItem = MenuItemLabel::create(Label::createWithSystemFont("Reset", "", 25),
		[=](Ref* pSender) {	gameLogic.object->setPosition(visibleSize / 2);	gameLogic.object->reset();	});


	reset_menuItem->setPosition(0 ,-visibleSize.height/2 + reset_menuItem->getContentSize().height/2);
	major_remove->setPosition(reset_menuItem->getPosition() + Vec2(0, major_remove->getContentSize().height));
	major_add->setPosition( major_remove->getPosition() + Vec2(0, major_add->getContentSize().height));

	major_add->setName("add");
	major_remove->setName("remove");
	reset_menuItem->setName("reset");

	auto menu = Menu::create(major_add, major_remove, reset_menuItem, NULL);
	menu->setName("menu");
	this->addChild(menu, 1);*/

	auto major_add = MenuItemLabel::create(Label::createWithSystemFont("add Robot", "", 25),
		[=](Ref* pSender) {	gameLogic->addRobot(); });
	major_add->setPosition(Vec2(visibleSize.width , -visibleSize.height)/2 + Vec2(-major_add->getContentSize().width, major_add->getContentSize().height));
	major_add->setName("add");

	auto major_remove = MenuItemLabel::create(Label::createWithSystemFont("remove Robot", "", 25),
		[=](Ref* pSender) {	gameLogic->removeRobot(); });
	major_remove->setPosition(major_add->getPosition() + Vec2(0, major_remove->getContentSize().height));
	major_remove->setName("remove");

	auto menu = Menu::create(major_add, major_remove , NULL);
	menu->setName("menu");
	this->addChild(menu, 1);
}

