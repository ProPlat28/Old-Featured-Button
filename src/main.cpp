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

    void findFeaturedButton(CCNode* node) {
    auto id = std::string(node->getID());

    if (id.find("featured") != std::string::npos) {
        auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(node);

        if (!button)
            button = typeinfo_cast<CCMenuItemSpriteExtra*>(node->getParent());

        if (button)
            changeFeaturedButton(button);
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
