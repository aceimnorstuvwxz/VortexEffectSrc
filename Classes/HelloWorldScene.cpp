#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "VortexLayer.h"

USING_NS_CC;


Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	VortexLayer*	layer = VortexLayer::create();
	layer->setTargetLayer(HelloWorld::create());
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto sprite = Sprite::create("Sea.jpg");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize / 2) + origin);


    // add the sprite as a child to this layer
    this->addChild(sprite);
    
	//创建鱼所在的层
	m_FishLayer = FishLayer::create();
	m_FishLayer->retain();
	this->addChild(m_FishLayer);

    return true;
}

