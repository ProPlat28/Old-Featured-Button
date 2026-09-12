#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <string>
#include <algorithm>

using namespace geode::prelude;

class $modify(OldFeaturedHook, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        this->swapFeaturedButtonTexture(this);

        return true;
    }

    void swapFeaturedButtonTexture(CCNode* node) {
        if (!node) return;

        std::string const id = node->getID();

        if (id.find("featured") != std::string::npos) {
            if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
                this->replaceButtonSprite(btn);
            }
            else if (auto parentBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(node->getParent())) {
                this->replaceButtonSprite(parentBtn);
            }
        }

        auto children = node->getChildren();
        if (!children) return;

        for (int i = 0; i < children->count(); i++) {
            this->swapFeaturedButtonTexture(static_cast<CCNode*>(children->objectAtIndex(i)));
        }
    }

    CCSize findSiblingButtonSize(CCMenuItemSpriteExtra* btn) {
        auto parent = btn->getParent();
        if (!parent) return CCSizeZero;

        auto siblings = parent->getChildren();
        if (!siblings) return CCSizeZero;

        for (int i = 0; i < siblings->count(); i++) {
            auto sibling = typeinfo_cast<CCMenuItemSpriteExtra*>(siblings->objectAtIndex(i));
            if (sibling && sibling != btn) {
                auto size = sibling->getContentSize();
                if (size.width > 0.f && size.height > 0.f) {
                    return size;
                }
            }
        }

        return CCSizeZero;
    }

    void replaceButtonSprite(CCMenuItemSpriteExtra* btn) {
        CCSize targetSize = this->findSiblingButtonSize(btn);
        if (targetSize.width <= 0.f || targetSize.height <= 0.f) {
            targetSize = btn->getContentSize();
        }

        auto newSprite = CCSprite::create("old_featured_btn.png"_spr);
        if (!newSprite) {
            log::warn("OldFeaturedIcon: failed to load replacement sprite");
            return;
        }

        auto rawSize = newSprite->getContentSize();
        if (rawSize.width > 0.f && rawSize.height > 0.f &&
            targetSize.width > 0.f && targetSize.height > 0.f) {
            float scale = std::min(
                targetSize.width / rawSize.width,
                targetSize.height / rawSize.height
            );
            newSprite->setScale(scale);
        }

        btn->setNormalImage(newSprite);

        if (targetSize.width > 0.f && targetSize.height > 0.f) {
            btn->setContentSize(targetSize);
        }

        log::info("OldFeaturedIcon: replaced Featured button sprite");
    }
};
