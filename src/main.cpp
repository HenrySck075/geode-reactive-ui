#include <Geode/Geode.hpp>
#include "../include/enum.hpp"
#include "../include/style.hpp"
#include "../include/Point4.hpp"

using namespace geode::prelude;

#include <Geode/modify/CCEGLView.hpp>
class $modify(CCEGLView) {
    void onGLFWWindowSizeFunCallback(GLFWwindow* win, int w, int h) {
        onGLFWWindowSizeFunCallback(win, w, h);
        auto d = CCDirector::sharedDirector();
        if (d == nullptr) return;
        auto s = d->getRunningScene();
        if (s == nullptr) return;
        s->setContentSize(d->convertToGL(ccp(w,h)));
    }
};
#include <Geode/modify/CCNode.hpp>
class $modify(CCNode) {
    // @param value Value in specified unit
    // @param unit The unit in question
    // @returns The value in OpenGL unit
    float processUnit(float value, Unit unit, bool width) {
        switch (unit) {
            case Unit::OpenGL: return value;
            case Unit::UIKit: 
                auto d = CCDirector::sharedDirector();
                // any value works
                return value * (d->getWinSize().width / d->getOpenGLView()->getDisplaySize().width); 
            case Unit::Viewport:
                return value * (width ? CCDirector::sharedDirector()->getWinSize().width : CCDirector::sharedDirector()->getWinSize().height);
            case Unit::Percent:
                return value / 100 * (width ? m_pParent->getContentSize().width : m_pParent->getContentSize().height);
        }
    }

    void updateStyle(DirtyCategory dirtyType, Style* style, bool self) {
        auto anchor = style->anchor;
        
        // styling guideline
        if (dirtyType == DirtyCategory::Size) {
            if (self) goto selfUpdate;
            switch (anchor->horizontal->getObject()) {
                case AnchorHorizontal::Left:
                    this->setPositionX(
                        m_pParent->getContentSize().width 
                        - 
                        processUnit(
                            style->offset->getObject().x,
                            style->offset_unit->horizontal->getObject(),
                            true
                        )
                    );
                    break;
                case AnchorHorizontal::Center:
                    this->setPositionX(
                        m_pParent->getContentSize().width / 2
                        -
                        processUnit(
                            style->offset->getObject().x,
                            style->offset_unit->horizontal->getObject(),
                            true
                        )
                    );
                    break;
                case AnchorHorizontal::Right:
                    this->setPositionX(
                        processUnit(
                            style->offset->getObject().x,
                            style->offset_unit->horizontal->getObject(),
                            true
                        )
                    );
                    break;
            }
            switch (anchor->vertical->getObject()) {
                case AnchorVertical::Top:
                    this->setPositionY(
                        m_pParent->getContentSize().height
                        - 
                        processUnit(
                            style->offset->getObject().y,
                            style->offset_unit->vertical->getObject(),
                            false
                        )
                    );
                    break;
                case AnchorVertical::Center:
                    this->setPositionY(
                        m_pParent->getContentSize().height / 2
                        - 
                        processUnit(
                            style->offset->getObject().y,
                            style->offset_unit->vertical->getObject(),
                            false
                        )
                    );
                    break;
                case AnchorVertical::Bottom:
                    this->setPositionY(
                        processUnit(
                            style->offset->getObject().y,
                            style->offset_unit->vertical->getObject(),
                            false
                        )
                    );
                    break;
            }
            selfUpdate:
            setContentSize(CCSize(
                processUnit(
                    style->size->getObject().width,
                    style->size_unit->horizontal->getObject(),
                    true
                ),
                processUnit(
                    style->size->getObject().height,
                    style->size_unit->vertical->getObject(),
                    false
                )
            ));
        }

        this->setUserObject("reactive_dirty", nullptr);
        this->setUserObject("reactive_dirty_self", nullptr);
    }

    void update() {

        CCAny<DirtyCategory>* dirtyTypePtr;
        // if no parent, quit
        // i mean, this update function wont even run if isRunning is false, is it?
        if (m_pParent == nullptr) return; 

        bool fromSelf = false;
        dirtyTypePtr = static_cast<CCAny<DirtyCategory>*>(m_pParent->getUserObject("reactive_dirty"));
        // if parent's dirty attr is not set, check if the child 
        // have the dirty_self attr.
        //
        // this is done to ensure that Style just setting dirty_self wont cause
        // other unrelated nodes in the parent's child list to update, 
        // which some mod might have its heavy stuff placed in setContentSize
        // for no reason at all
        if (dirtyTypePtr == nullptr) {
            dirtyTypePtr = static_cast<CCAny<DirtyCategory>*>(this->getUserObject("reactive_dirty_self"));
            // if still not the quit
            if (dirtyTypePtr == nullptr) return;
            fromSelf = true;
        }
        auto dirtyType = dirtyTypePtr->getValue();
        auto style = static_cast<Style*>(m_pParent->getUserObject("reactive_style"));
        if (style == nullptr) return;
        
        updateStyle(dirtyType, style, fromSelf);
    }
    void setContentSize(CCSize const& size) {
		CCNode::setContentSize(size);
        this->setUserObject("reactive_dirty", CCAny<DirtyCategory>::create(DirtyCategory::Size));
	}
};