#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
using namespace geode::prelude;

#define IMAGE_PATH "/storage/emulated/0/GDPictures/img.png"

CCTexture2D* getCustomTexture() {
    std::string path = IMAGE_PATH;

    if (!std::filesystem::exists(path)) return nullptr;

    CCTextureCache::get()->removeTextureForKey(path.c_str());
    auto texture = CCTextureCache::get()->addImage(path.c_str(), false);

    if (texture) {
        texture->setAntiAliasTexParameters();
    }

    return texture;
}

$modify(MyEditorUI, EditorUI) {
    bool init(LevelEditorLayer* lel) {
        if (!EditorUI::init(lel)) return false;

        auto editMenu = this->getChildByID("edit-tab-menu");
        if (!editMenu) return true;

        auto label = CCLabelBMFont::create("IMG", "bigFont.fnt");
        label->setScale(0.4f);

        auto bg = CCScale9Sprite::create(
            "GJ_button_01.png", {0, 0, 40, 40}
        );
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

    void onImportImage(CCObject*) {
        auto texture = getCustomTexture();

        if (!texture) {
            FLAlertLayer::create(
                "Помилка",
                "Файл не знайдено!\nПоклади <cy>img.png</cy> в\n<cg>/storage/emulated/0/GDPictures/</cg>",
                "OK"
            )->show();
            return;
        }

        auto selected = this->getSelectedObjects();

        if (!selected || selected->count() == 0) {
            FLAlertLayer::create(
                "Підказка",
                "Спочатку <cy>вибери блок</cy> в редакторі,\nпотім натисни IMG!",
                "OK"
            )->show();
            return;
        }

        for (int i = 0; i < selected->count(); i++) {
            auto obj = typeinfo_cast<GameObject*>(
                selected->objectAtIndex(i)
            );
            if (!obj) continue;

            float scaleX = obj->getScaleX();
            float scaleY = obj->getScaleY();

            auto mainSprite = typeinfo_cast<CCSprite*>(
                obj->getChildByTag(0)
            );
            if (!mainSprite) {
                mainSprite = typeinfo_cast<CCSprite*>(obj);
            }

            if (mainSprite) {
                mainSprite->setTexture(texture);

                float texW = texture->getPixelsWide() / scaleX;
                float texH = texture->getPixelsHigh() / scaleY;

                mainSprite->setTextureRect({0, 0, texW, texH});
            }
        }
    }
}
