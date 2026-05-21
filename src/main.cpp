#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <filesystem>

using namespace geode::prelude;

static CCTexture2D* getCustomTexture() {
    std::string path = "/storage/emulated/0/GDPictures/img.png";
    if (!std::filesystem::exists(path)) {
        return nullptr;
    }
    CCTextureCache::get()->removeTextureForKey(path.c_str());
    auto tex = CCTextureCache::get()->addImage(path.c_str(), false);
    if (tex) {
        tex->setAntiAliasTexParameters();
    }
    return tex;
}

struct MyEditorUI : geode::Modify<MyEditorUI, EditorUI> {

    bool init(LevelEditorLayer* lel) {
        if (!EditorUI::init(lel)) {
            return false;
        }

        auto editMenu = this->getChildByID("edit-tab-menu");
        if (!editMenu) {
            return true;
        }

        auto label = CCLabelBMFont::create("IMG", "bigFont.fnt");
        label->setScale(0.4f);

        auto bg = CCScale9Sprite::create("GJ_button_01.png", {0, 0, 40, 40});
        bg->setContentSize({40, 40});
        label->setPosition({20, 20});
        bg->addChild(label);

        auto btn = CCMenuItemSpriteExtra::create(
            bg,
            this,
            menu_selector(MyEditorUI::onImportImage)
        );
        btn->setID("import-img-btn");
        editMenu->addChild(btn);
        editMenu->updateLayout();

        return true;
    }

    void onImportImage(CCObject* sender) {
        auto tex = getCustomTexture();

        if (!tex) {
            FLAlertLayer::create(
                "Помилка",
                "Поклади img.png в /storage/emulated/0/GDPictures/",
                "OK"
            )->show();
            return;
        }

        auto selected = this->getSelectedObjects();
        if (!selected || selected->count() == 0) {
            FLAlertLayer::create(
                "Підказка",
                "Вибери блок в редакторі потім натисни IMG!",
                "OK"
            )->show();
            return;
        }

        for (unsigned int i = 0; i < selected->count(); i++) {
            auto obj = typeinfo_cast<GameObject*>(selected->objectAtIndex(i));
            if (!obj) {
                continue;
            }

            auto spr = typeinfo_cast<CCSprite*>(obj->getChildByTag(0));
            if (!spr) {
                spr = typeinfo_cast<CCSprite*>(obj);
            }

            if (spr) {
                spr->setTexture(tex);
                float w = (float)tex->getPixelsWide();
                float h = (float)tex->getPixelsHigh();
                spr->setTextureRect({0, 0, w, h});
            }
        }
    }
};
