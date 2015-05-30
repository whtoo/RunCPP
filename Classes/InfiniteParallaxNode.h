#ifndef INFINITEPARALLAXNODE_H
#define INFINITEPARALLAXNODE_H

#include "cocos2d.h"
USING_NS_CC;

class InfiniteParallaxNode : public ParallaxNode
{
public:
    static InfiniteParallaxNode* create();
    void updatePosition();
};

#endif // INFINITEPARALLAXNODE_H
