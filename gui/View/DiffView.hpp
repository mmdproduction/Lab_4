#pragma once
#include "DiffModel.hpp"
#include "DiffController.hpp"
#include "UI.hpp"

class DiffView {
    DiffModel&      model;
    DiffController& controller;

    float x, y, w, h;

    const glm::vec4 COLOR_BG    = glm::vec4(0.15f, 0.15f, 0.18f, 1.0f);
    const glm::vec4 COLOR_PANEL = glm::vec4(0.2f,  0.2f,  0.25f, 1.0f);
    const glm::vec4 COLOR_GREEN = glm::vec4(0.3f,  0.7f,  0.3f,  1.0f);
    const glm::vec4 COLOR_RED   = glm::vec4(0.6f,  0.3f,  0.3f,  1.0f);
    const glm::vec4 COLOR_BLUE  = glm::vec4(0.3f,  0.4f,  0.7f,  1.0f);

    
    UIInputText input_elem;
    UIInputText input_count;
    UIButton    btn_add;
    UIButton    btn_remove;
    UIButton    btn_clear;
    UIButton    btn_predict;

    UICollapsibleList list_auto;
    UIButton          btn_load_auto;

    UIList list_seed;
    UIList list_predictions;

public:
    DiffView(DiffModel& m, DiffController& c,
                   float x_, float y_, float w_, float h_)
        : model(m), controller(c), x(x_), y(y_), w(w_), h(h_),
       
          input_elem (x + 180,  y + h - 60,  260, 35, "Элемент ряда"),
          input_count(x + 180,  y + h - 110, 260, 35, "Кол-во предсказаний"),
          btn_add    (x + 80,  y + h - 160, 120, 35, COLOR_GREEN, "Добавить",  {1,1,1}),
          btn_remove (x + 250, y + h - 160, 120, 35, COLOR_RED,   "Удалить",   {1,1,1}),
          btn_clear  (x + 80,  y + h - 210, 120, 35, COLOR_RED,   "Очистить",  {1,1,1}),
          btn_predict(x + 250, y + h - 210, 120, 35, COLOR_GREEN, "Предсказать",{1,1,1}),
      
          list_auto  (x + 180,  y + h - 270, 260, "Авт. наборы"),
          btn_load_auto(x + 180, y + h - 380, 260, 35, COLOR_BLUE, "Загрузить", {1,1,1}),
      
          list_seed       (x + 520, y + h/2, 280, h - 40),
          list_predictions(x + 840, y + h/2, 280, h - 40)
    {
        list_seed.setShowScrollbar(true);
        list_predictions.setShowScrollbar(true);

        for (size_t i = 0; i < model.auto_cases.getLength(); ++i)
            list_auto.addItem(model.auto_cases.get(i).name);
    }

    void on_event(EventSystem& events) {
        input_elem.onEvent(events);
        input_count.onEvent(events);
        btn_add.onEvent(events);
        btn_remove.onEvent(events);
        btn_clear.onEvent(events);
        btn_predict.onEvent(events);
        list_auto.onEvent(events);
        btn_load_auto.onEvent(events);
        list_seed.onEvent(events);
        list_predictions.onEvent(events);

        if (btn_add.isPressed) {
            controller.on_add_element(input_elem.getText());
            input_elem.setText("");
            refresh_seed();
        }

        if (btn_remove.isPressed) {
            controller.on_remove_last();
            refresh_seed();
        }

        if (btn_clear.isPressed) {
            controller.on_clear_seed();
            refresh_seed();
            refresh_predictions();
        }

        if (btn_predict.isPressed) {
            controller.on_set_predict_count(input_count.getText());
            controller.on_predict();
            refresh_predictions();
        }

        if (btn_load_auto.isPressed) {
            int idx = list_auto.getSelectedItem();
            if (idx >= 0) {
                controller.on_load_auto_case(idx);
                refresh_seed();
                refresh_predictions();
            }
        }
    }

    void draw(Renderer& renderer, TextRenderer& textRenderer) {
        renderer.begin();
        textRenderer.begin();

        renderer.draw({x, y}, {w, h}, COLOR_BG);


        textRenderer.drawText("Ввод ряда",       x + 80,  y + h - 20, 1, glm::vec3(0.9f));
        textRenderer.drawText("Исходный ряд",     x + 520, y + h - 20, 1, glm::vec3(0.9f));
        textRenderer.drawText("Предсказания",     x + 840, y + h - 20, 1, glm::vec3(0.9f));

        btn_add.draw(renderer, textRenderer);
        btn_remove.draw(renderer, textRenderer);
        btn_clear.draw(renderer, textRenderer);
        btn_predict.draw(renderer, textRenderer);
        btn_load_auto.draw(renderer, textRenderer);

        renderer.end();
        textRenderer.end();

        input_elem.draw(renderer, textRenderer);
        input_count.draw(renderer, textRenderer);
        list_auto.draw(renderer, textRenderer);
        list_seed.draw(renderer, textRenderer);
        list_predictions.draw(renderer, textRenderer);
    }

    void update(float dt) {
        input_elem.update(dt);
        input_count.update(dt);
        btn_add.update(dt);
        btn_remove.update(dt);
        btn_clear.update(dt);
        btn_predict.update(dt);
        list_auto.update(dt);
        btn_load_auto.update(dt);
        list_seed.update(dt);
        list_predictions.update(dt);
    }

private:
    void refresh_seed() {
        list_seed.clearItems();
        for (const auto& elem: model.seed)
            list_seed.addItem(std::format("{:.2f}", elem));
    }

    void refresh_predictions() {
        list_predictions.clearItems();
        for (const auto& elem: model.predictions)
            list_predictions.addItem(std::format("{:.2f}", elem));
    }
};