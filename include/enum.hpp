#pragma once

#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

template<typename T>
class CCAny : public CCObject
{
public:
    CCAny(T v)
        : m_value(v) {}
    T getValue() const {return m_value;}

    static CCAny<T>* create(T v)
    {
        CCAny<T>* pRet = new CCAny(v);
        if (pRet)
        {
            pRet->autorelease();
        }
        return pRet;
    }

    /* override functions */
    virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visitObject(this); }

private:
    T m_value;
};



// enums

enum class AnchorVertical {Top, Center, Bottom};
enum class AnchorHorizontal {Left, Center, Right};

enum class Unit {
    /// Relative to the parent size. Value in range 0-100. Value higher or lower is allowed.
    Percent,
    /// Viewport. Value in range 0-1 with 0 is 0 and 1 is full viewport length. Value higher or lower is allowed.
    Viewport,
    /// OpenGL Point
    OpenGL,
    /// UIKit Point
    UIKit
};

enum class DirtyCategory {
    Size, // parent size updated, prompting its children to update
    Position, // unused until position property (like sticky, fixed, absolute, ...) is implemented
    Stinky
};