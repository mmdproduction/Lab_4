#pragma once
#include "TestModel.hpp"
#include "UI.hpp"

class TestView {
    TestModel& model;

    float x, y, w, h;

    float btnW = 160, btnH = 35;
    float pad = 20; 

    float listW = 0;
    float listH = 0;  
    float listBottomY = 0; 
    float topY = 0;


    const glm::vec4 COLOR_PASS = glm::vec4(0.2f, 0.8f, 0.3f, 1.0f);
    const glm::vec4 COLOR_FAIL = glm::vec4(0.9f, 0.2f, 0.2f, 1.0f);
    const glm::vec4 COLOR_BG   = glm::vec4(0.15f, 0.15f, 0.18f, 1.0f);
    const glm::vec4 COLOR_PANEL = glm::vec4(0.2f, 0.2f, 0.25f, 1.0f);

    UIButton  btnRunAll, btnClear;
    UIList    resultList;


public:
    TestView(TestModel& m,float x_,float y_,float w_,float h_): model(m), x(x_), y(y_), w(w_), h(h_), topY(y + h - 10), listW(w - 40), listBottomY(y + 20), listH( h - 80), btnRunAll(x + pad + btnW / 2.0f,               
        topY - btnH / 2.0f,                      
        btnW, btnH, 
        {0.3f, 0.7f, 0.3f, 1.0f}, 
        "Запустить все", 
        {1, 1, 1}),
        btnClear(x + w - pad - btnW / 2.0f,    
        topY - btnH / 2.0f, 
        btnW, btnH, 
        {0.6f, 0.3f, 0.3f, 1.0f}, 
        "Очистить", 
        {1, 1, 1}), resultList(x + 20 + listW / 2.0f,                  
        listBottomY + listH / 2.0f,
        listW, listH) {

            resultList.setShowScrollbar(true);
        }

    std::function<void()> onRunAll;
    std::function<void()> onClear;

    void onEvent(EventSystem& events) {
        btnRunAll.onEvent(events);
        btnClear.onEvent(events);
        resultList.onEvent(events);
        if (btnRunAll.isPressed && onRunAll){refreshList(); onRunAll();}
        if (btnClear.isPressed && onRunAll) onClear();
    }

    void draw(Renderer& renderer, TextRenderer& textRenderer) {

        

        renderer.begin();
        textRenderer.begin();

        
        renderer.draw({x, y}, {w, h}, COLOR_BG);
        float titleY = y + h - 35;
        textRenderer.drawText("Модульные тесты", x + 350, titleY, 1, glm::vec3(0.9f));

        btnRunAll.draw(renderer, textRenderer);
        btnClear.draw(renderer, textRenderer);

        renderer.end();
        textRenderer.end();
        
        resultList.draw(renderer, textRenderer);
        
    }

    void refreshList() {
        std::lock_guard lock(model.pendingMutex);
        clearList();
        
        for (size_t i = 0; i < model.pendingResults.getLength(); ++i) {
            const auto& r = model.pendingResults.get(i);
            std::string text = std::string(r.passed ? "[OK] " : "[FAIL] ") + r.name;
            resultList.addItem(text); 
        }
    }

    void clearList(){
        size_t count = resultList.getItemCount();
        for (size_t i = count; i > 0; --i) {
            resultList.removeItem(i - 1);
        }
    }

    void update(float dt) {
        btnRunAll.update(dt);
        btnClear.update(dt);    
    }

    void addResult(const std::string& text, bool passed) {
    resultList.addItem(text);
}
};