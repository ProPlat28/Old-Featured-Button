#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;

class $modify(CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init())
            return false;

        findFeatured(this);
        return true;
    }

    void findFeatured(CCNode* node) {
        auto id = node->getID();

        if (id.find("featured") != std::string::npos) {
            auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(node);

            if (!button)
                button = typeinfo_cast<CCMenuItemSpriteExtra*>(node->getParent());

            if (button) {
                auto sprite = CCSprite::create("old_featured_btn.png"_spr);

                if (sprite) {
                    auto size = button->getContentSize();
                    auto spriteSize = sprite->getContentSize();

                    if (spriteSize.width > 0 && spriteSize.height > 0) {
                        float scale = std::min(
                            size.width / spriteSize.width,
                            size.height / spriteSize.height
                        );

                        sprite->setScale(scale);
                    }

                    button->setNormalImage(sprite);
                }
            }
        }

        auto children = node->getChildren();

        if (!children)
            return;

        for (unsigned int i = 0; i < children->count(); i++) {
            findFeatured(
                static_cast<CCNode*>(children->objectAtIndex(i))
            );
        }
    }
};
