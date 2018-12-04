#include "SchedulerTest.h"

Scene *SchedulerTest::createScene() {
	auto scene = SchedulerTest::create();
	return scene;
}
bool SchedulerTest::init() {
	if (!Scene::init()) {
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();

	//当前测试标签描述
	auto test_label = Label::createWithSystemFont("About Node's Update Test", "", 30);
	test_label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - test_label->getContentSize().height));
	this->addChild(test_label);

	/***************************************************************
	****						节点一
	****************************************************************/
	LayerColor *block1 = LayerColor::create(Color4B::RED, 70, 70);
	this->addChild(block1);
	block1->ignoreAnchorPointForPosition(false);
	block1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	block1->setPosition(visibleSize.width / 4, visibleSize.height * 3 / 4 - 50);
	//设置名字以便获取
	block1->setName("block1");
	//附加在节点一上的名字标签
	auto label1 = Label::createWithSystemFont("block1", "", 22);
	block1->addChild(label1);
	label1->setPosition(block1->getContentSize() / 2);

	//节点一状态标签,对于状态，需要动态改变它，所以需要给它设置个名字
	auto state_label1 = Label::createWithSystemFont("Block1 State:", "", 25);
	state_label1->setPosition(Vec2(
		state_label1->getContentSize().width / 2,
		block1->getPositionY() + block1->getContentSize().height)
	);
	this->addChild(state_label1);
	auto state1 = Label::createWithSystemFont("Stop", "", 25);
	state1->setPosition(Vec2(
		visibleSize.width / 2,
		state_label1->getPositionY())
	);
	this->addChild(state1);
	//设置名字以便获取
	state1->setName("state1");

	/***************************************************************
	****						节点二
	****************************************************************/
	LayerColor *block2 = LayerColor::create(Color4B::RED, 70, 70);
	this->addChild(block2);
	block2->ignoreAnchorPointForPosition(false);
	block2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	block2->setPosition(Vec2(block1->getPositionX(), block1->getPositionY() - 160));
	//设置名字以便获取
	block2->setName("block2");
	//附加在节点二上的名字标签
	auto label2 = Label::createWithSystemFont("block2", "", 22);
	block2->addChild(label2);
	label2->setPosition(block2->getContentSize() / 2);


	//节点二状态标签
	auto state_label2 = Label::createWithSystemFont("Block2 State:", "", 25);
	state_label2->setPosition(Vec2(
		state_label2->getContentSize().width / 2,
		block2->getPositionY() + block2->getContentSize().height)
	);
	this->addChild(state_label2);
	auto state2 = Label::createWithSystemFont("Stop", "", 25);
	state2->setPosition(Vec2(
		visibleSize.width / 2,
		state_label2->getPositionY())
	);
	this->addChild(state2);

	state2->setName("state2");
	/***************************************************************
	****						菜单条目
	****************************************************************/

	/*****
	**节点的Start菜单条目，用于节点开启调度器
	******/
	MenuItemLabel* Start_menuItem1 = MenuItemLabel::create(Label::createWithSystemFont("Start", "", 25), [&](Ref* sender) {

		//修改Block1状态
		auto state = dynamic_cast<Label*>(this->getChildByName("state1"));
		state->setString("Running...");
		//开启当前节点的update
		this->scheduleUpdate();
		//设置当前点击目标不可用
		auto start_item = dynamic_cast<MenuItemLabel*>(sender);
		start_item->setEnabled(false);
		//获取Remove并设置为可用点击
		auto menu = dynamic_cast<Menu*>(this->getChildByName("menu"));
		auto remove_item = dynamic_cast<MenuItemLabel*>(menu->getChildByName("remove_item1"));
		remove_item->setEnabled(true);
	});
	Start_menuItem1->setPosition(Vec2(
		Start_menuItem1->getContentSize().width / 2 - visibleSize.width / 2,
		Start_menuItem1->getContentSize().height - 10)
	);
	Start_menuItem1->setName("start_item1");

	auto Start_menuItem2 = MenuItemLabel::create(Label::createWithSystemFont("Start", "", 25), [&](Ref *sender) {
		if (!isScheduled(CC_SCHEDULE_SELECTOR(SchedulerTest::Own_Update))) {

			//设置当前点击目标不可用
			auto start_item = dynamic_cast<MenuItemLabel*>(sender);
			start_item->setEnabled(false);
			/*****************************************
			**开启自定义节点的update：在3秒之后开启，每1秒调用一次，一直开启
			**参数相关：
			**param1  CC_SCHEDULE_SELECTOR(SchedulerTest::Own_Update)：被调度器回调的函数
			**param2  1.0f:每隔一秒调用一次
			**param3  重复调用
			**param4  第一次开启调度器延迟时间，这里为4秒
			******************************************/
			this->schedule(CC_SCHEDULE_SELECTOR(SchedulerTest::Own_Update), 1.0f, CC_REPEAT_FOREVER, 4.0f);
			//开启另一个调度器，准备开启
			this->schedule(CC_SCHEDULE_SELECTOR(SchedulerTest::ReadyFunc), 1.0f, 3, 0);
		}

	});
	Start_menuItem2->setPosition(Vec2(Start_menuItem2->getContentSize().width / 2 - visibleSize.width / 2, -150));
	Start_menuItem2->setName("start_item2");

	/*****
	**节点的Remove菜单条目，用于移除节点调度器
	******/
	MenuItemLabel *Remove_menuItem1 = MenuItemLabel::create(Label::createWithSystemFont("Remove", "", 25), [&](Ref *sender) {
		//修改Block1状态
		auto state = dynamic_cast<Label*>(this->getChildByName("state1"));
		state->setString("Removed");
		//关闭update
		this->unscheduleUpdate();
		//设置当前目标不可用
		auto pause_item = dynamic_cast<MenuItemLabel*>(sender);
		pause_item->setEnabled(false);
		//获取Start并设置为可点击
		auto menu = dynamic_cast<Menu*>(this->getChildByName("menu"));
		auto start_item = dynamic_cast<MenuItemLabel*>(menu->getChildByName("start_item1"));
		start_item->setEnabled(true);
	});

	Remove_menuItem1->setPosition(Vec2(
		Start_menuItem1->getPositionX() + Start_menuItem1->getContentSize().width * 2,
		Start_menuItem1->getPositionY()));
	Remove_menuItem1->setEnabled(false);
	Remove_menuItem1->setName("remove_item1");

	auto Remove_menuItem2 = MenuItemLabel::create(Label::createWithSystemFont("Remove", "", 25), [&](Ref* sender) {

		if (isScheduled(CC_SCHEDULE_SELECTOR(SchedulerTest::Own_Update))) {
			//修改Block2状态
			auto state = dynamic_cast<Label*>(this->getChildByName("state2"));
			state->setString("Removed");

			this->unschedule(CC_SCHEDULE_SELECTOR(SchedulerTest::Own_Update));

			//设置当前目标不可用
			auto remove_item = dynamic_cast<MenuItemLabel*>(sender);
			remove_item->setEnabled(false);
			//获取Start并设置为可点击
			auto menu = dynamic_cast<Menu*>(this->getChildByName("menu"));
			auto start_item = dynamic_cast<MenuItemLabel*>(menu->getChildByName("start_item2"));
			start_item->setEnabled(true);
		}

	});
	Remove_menuItem2->setPosition(Vec2(
		Start_menuItem2->getPositionX() + Start_menuItem2->getContentSize().width * 2,
		Start_menuItem2->getPositionY()));
	Remove_menuItem2->setEnabled(false);
	Remove_menuItem2->setName("remove_item2");

	/*****
	**节点的Pause菜单条目，用于暂停当前节点的所有调度器
	******/
	auto Pause_menuItem = MenuItemLabel::create(Label::createWithSystemFont("PauseCurrentTarget", "", 25), [&](Ref* sender) {
		//暂停当前节点中的所有调度器
		this->getScheduler()->pauseTarget(this);

		//设置当前目标不可用
		auto remove_item = dynamic_cast<MenuItemLabel*>(sender);
		remove_item->setEnabled(false);

		//获取Resume并设置为可点击
		auto menu = dynamic_cast<Menu*>(this->getChildByName("menu"));
		auto resume_item = dynamic_cast<MenuItemLabel*>(menu->getChildByName("resume_item"));
		resume_item->setEnabled(true);

		//修改Block1状态
		auto state1 = dynamic_cast<Label*>(this->getChildByName("state1"));
		if (state1->getString() == "Running...") {
			state1->setString("Pause");
		}

		//修改Block2状态
		auto state2 = dynamic_cast<Label*>(this->getChildByName("state2"));
		if (isScheduled(CC_SCHEDULE_SELECTOR(SchedulerTest::Own_Update)) || isScheduled(CC_SCHEDULE_SELECTOR(SchedulerTest::ReadyFunc))) {
			state2->setString("Pause");
		}


	});
	Pause_menuItem->setPosition(Vec2(0, Pause_menuItem->getContentSize().height * 3 - visibleSize.height / 2));
	Pause_menuItem->setName("pause_item");

	/*****
	**节点的Resume菜单条目，用于恢复当前节点的所有调度器
	******/
	auto Resume_menuItem = MenuItemLabel::create(Label::createWithSystemFont("ResumeCurrentTarget", "", 25), [&](Ref* sender) {
		//恢复当前节点的所有调度器
		this->getScheduler()->resumeTarget(this);

		//设置当前目标不可用
		auto remove_item = dynamic_cast<MenuItemLabel*>(sender);
		remove_item->setEnabled(false);

		//获取Pause并设置为可点击
		auto menu = dynamic_cast<Menu*>(this->getChildByName("menu"));
		auto pause_item = dynamic_cast<MenuItemLabel*>(menu->getChildByName("pause_item"));
		pause_item->setEnabled(true);

		//修改Block1状态
		auto state1 = dynamic_cast<Label*>(this->getChildByName("state1"));
		if (state1->getString() == "Pause") {
			state1->setString("Running...");
		}

		//修改Block2状态
		auto state2 = dynamic_cast<Label*>(this->getChildByName("state2"));
		if (isScheduled(CC_SCHEDULE_SELECTOR(SchedulerTest::Own_Update))) {
			state2->setString("Running...");
		}


	});
	Resume_menuItem->setPosition(Vec2(
		Pause_menuItem->getPositionX(),
		Pause_menuItem->getPositionY() - Resume_menuItem->getContentSize().height * 2));
	Resume_menuItem->setEnabled(false);
	Resume_menuItem->setName("resume_item");

	//菜单
	auto menu = Menu::create(Start_menuItem1, Start_menuItem2, Remove_menuItem1, Remove_menuItem2, Pause_menuItem, Resume_menuItem, NULL);
	menu->setName("menu");

	this->addChild(menu);



	return true;
}
//节点的自我更新函数
void SchedulerTest::update(float dt) {

	//CCLOG("THE NODE'S UPDATE : %f...",dt);
	//判断节点的方向
	static int dir1 = 1;

	//通过名字获取当前节点的子节点
	auto block1 = dynamic_cast<LayerColor*>(this->getChildByName("block1"));
	Vec2 pos = Vec2(block1->getPositionX() + 3 * dir1, block1->getPositionY());
	if (pos.x > visibleSize.width - block1->getContentSize().width / 2 || pos.x < block1->getContentSize().width / 2) {
		dir1 = -dir1;
	}

	block1->setPosition(pos);
}

//自己的自我更新函数
void SchedulerTest::Own_Update(float dt) {
	//log("This is My Own Update : %f...",dt);
	static int dir2 = 1;

	//通过名字获取当前节点的子节点
	auto block2 = dynamic_cast<LayerColor*>(this->getChildByName("block2"));
	Vec2 pos = Vec2(block2->getPositionX() + 3 * dir2, block2->getPositionY());
	if (pos.x > visibleSize.width - block2->getContentSize().width / 2 || pos.x < block2->getContentSize().width / 2) {
		dir2 = -dir2;
	}

	block2->setPosition(pos);

}

//准备状态的函数
void SchedulerTest::ReadyFunc(float dt) {
	static int i = 4;
	i--;
	//修改Block2状态
	auto state = dynamic_cast<Label*>(this->getChildByName("state2"));
	state->setString(String::createWithFormat("Ready to Start:%d second", i)->getCString());

	if (i == 0) {
		i = 4;
		//修改Block2状态
		auto state = dynamic_cast<Label*>(this->getChildByName("state2"));
		state->setString("Running...");
		//获取Remove.
		auto menu = dynamic_cast<Menu*>(this->getChildByName("menu"));
		auto remove_item = dynamic_cast<MenuItemLabel*>(menu->getChildByName("remove_item2"));
		remove_item->setEnabled(true);

	}
}