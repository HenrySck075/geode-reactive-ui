#pragma once

#include <Geode/Geode.hpp>
#include <henrysck075.cocosutils/include/Reactive.hpp>
#include "enum.hpp"
using namespace geode::prelude;

class Style : public CCObject {
private:
    // the node applied to
    CCNode* chocola;

    enum class StyleUnitType {
        Offset, 
        Size
    };
    class Anchor : public CCObject {
    private:
        Style* style;
    public:
        Reactive<AnchorHorizontal>* horizontal;
        Reactive<AnchorVertical>* vertical;
        bool init(Style* s, AnchorHorizontal h, AnchorVertical v) {
            style = s;
            horizontal = std::remove_pointer_t<decltype(horizontal)>::create(h);
            vertical = std::remove_pointer_t<decltype(vertical)>::create(v);
        };
        static Anchor* create(Style* s, AnchorHorizontal h, AnchorVertical v) {
            Anchor* pRet = new Anchor();
            if (pRet && pRet->init(s,h,v))
            {
                pRet->autorelease();
            } else {
                delete pRet;
            }
            return pRet;
        }
    };
    class StyleUnit : public CCObject {
    private:
        Style* style;
        StyleUnitType type;
    public:
        Reactive<Unit>* horizontal;
        Reactive<Unit>* vertical;
        bool init(Style* s, Unit h, Unit v, StyleUnitType t) {
            style = s;
            horizontal = Reactive<Unit>::create(h);
            vertical = Reactive<Unit>::create(v);
            type = t;
        };
        static StyleUnit* create(Style* s, Unit h, Unit v, StyleUnitType t) {
            StyleUnit* pRet = new StyleUnit();
            if (pRet && pRet->init(s,h,v,t))
            {
                pRet->autorelease();
            } else {
                delete pRet;
            }
            return pRet;
        }
    };
    void alert(DirtyCategory neko) {
        chocola->setUserObject("reactive_dirty_self", CCAny<DirtyCategory>::create(neko));
    }

    void sizeListener(Reactive<CCSize>* obj) {
        alert(DirtyCategory::Size);
    }
public:
    bool init(CCNode* node) {
        anchor = std::remove_pointer_t<decltype(anchor)>::create(this, AnchorHorizontal::Left, AnchorVertical::Bottom);
        offset = Reactive<CCPoint>::create(node->getPosition());
        offset_unit = StyleUnit::create(this, Unit::OpenGL, Unit::OpenGL, StyleUnitType::Offset);
        size = Reactive<CCSize>::create(node->getContentSize());
        size_unit = StyleUnit::create(this, Unit::OpenGL, Unit::OpenGL, StyleUnitType::Size);

        offset_unit->retain();
        size_unit->retain();
        size->retain();
        offset->retain();
        anchor->retain();

        chocola = node;
        return true;
    }
    static Style* create(CCNode* node) {
        Style* pRet = new Style();
        if (pRet && pRet->init(node))
        {
            pRet->autorelease();
        } else {
            delete pRet;
        }
        return pRet;
    }

    // position anchor
    Anchor* anchor;
    // offset from anchor
    Reactive<CCPoint>* offset;
    StyleUnit* offset_unit;
    // size
    Reactive<CCSize>* size;
    StyleUnit* size_unit;

    ~Style() {
        anchor->release();
        offset->release();
        offset_unit->release();
        size->release();
        size_unit->release();
    }
    friend class StyleUnit;
    friend class Anchor;
};