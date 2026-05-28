#pragma once
#include "DiffModel.hpp"
#include "DiffController.hpp"
#include "DiffView.hpp"

class DiffPanel {
    DiffModel      model;
    DiffController controller;
    DiffView       view;
 
public:
    DiffPanel(float x, float y, float w, float h)
        : model(),
          controller(model),
          view(model, controller, x, y, w, h) {}
 
    void onEvent(EventSystem& events) { view.on_event(events); }
    void draw(Renderer& r, TextRenderer& tr) { view.draw(r, tr); }
    void update(float dt) { view.update(dt); }
};