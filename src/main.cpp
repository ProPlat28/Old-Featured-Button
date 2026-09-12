#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;

class $modify(OldFeaturedHook, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        this->swapFeaturedButtonTexture(this);

        return true;
    }

    void swapFeaturedButtonTexture(CCNode* node) {
        if (!node) return;

        auto id = node->getID();
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

    void replaceButtonSprite(CCMenuItemSpriteExtra* btn) {
        auto oldImage = btn->getNormalImage();
        auto oldSize = oldImage ? oldImage->getContentSize() : CCSizeZero;

        auto newSprite = CCSprite::create("old_featured_btn.png"_spr);
        if (!newSprite) {
            log::warn("OldFeaturedIcon: failed to load replacement sprite");
            return;
        }

        if (oldSize.width > 0.f && oldSize.height > 0.f) {
            auto newSize = newSprite->getContentSize();
            float scale = std::min(oldSize.width / newSize.width, oldSize.height / newSize.height);
            newSprite->setScale(scale);
        }

        btn->setNormalImage(newSprite);
        btn->setContentSize(newSprite->getScaledContentSize());

        log::info("OldFeaturedIcon: replaced Featured button sprite");
    }
};