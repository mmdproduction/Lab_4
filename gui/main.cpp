#include<locale>
#include"GL/glew.h"

#include"GLFW/glfw3.h"
#include"glm/glm.hpp"

#include"Sequence.hpp"
#include"TestView.hpp"
#include"TestModel.hpp"
#include"TestController.hpp"
#include"TabsPanel.hpp"
#include"DiffPanel.hpp"
#include<iostream>


int main(){

    setlocale(LC_ALL, ".UTF8");
    if (!Window::windowInit()) return -1;
    
    Window window(1080, 720, "Test GUI");


    window.makeContextCurrent();
    window.setFramebufferSizeCallback();

    if(glewInit()) return -1;
    Renderer render = Renderer(window.getWidth(), window.getHeight(), "shaders/vert.glsl", "shaders/frag.glsl");
    TextRenderer textRender = TextRenderer(window.getWidth(), window.getHeight(), "shaders/text_vert.glsl", "shaders/text_frag.glsl");

    if(!textRender.loadFont("C:/Windows/Fonts/arial.ttf", 24)){ //TODO добавить свой шрифт в папку assets
        std::cerr << "ERROR::FONT::BUILD_FAIL";
    }


    EventSystem events;

    events.setKeyCallback(window.getWindow());
    events.setCharCallback(window.getWindow());
    events.setMouseButtonCallback(window.getWindow());
    events.setCursorPosCallback(window.getWindow());
    events.setScrollCallback(window.getWindow());


    TestModel testModel;
    TestController testCtrl(testModel);
    TestView testView(testModel, 0, 0, window.getWidth(), window.getHeight() - 40);

    TabsPanel tabsPanel = TabsPanel(0, window.getHeight() - 40, window.getWidth(), 40);
    DiffPanel dfPanel = DiffPanel(0, 0, window.getWidth(), window.getHeight() - 40);


    testModel.onTestFinished = [&](const TestResultData& data) {
    std::string text = std::string(data.passed ? "[OK]   " : "[FAIL] ") + data.name;
    testView.addResult(text, data.passed);
    };
    testView.onRunAll = [&]{ testView.clearList(); testCtrl.onRunAll(); };
    testView.onClear  = [&]{ testCtrl.onClear(testView);};

    tabsPanel.addTab("Разност. схемы");
    tabsPanel.addTab("Тесты");

    float lastTime = glfwGetTime();
    while(!window.isWindowShouldClose()){

        events.update();
        tabsPanel.onEvent(events);

        if(tabsPanel.getActive() == 0){
            dfPanel.onEvent(events);
        }
        if(tabsPanel.getActive() == 1){
            testView.onEvent(events);
        }

        double now = glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;

        glClear(GL_COLOR_BUFFER_BIT);


        tabsPanel.draw(render, textRender);
        if(tabsPanel.getActive() == 0){
            dfPanel.draw(render, textRender);
        }

        if(tabsPanel.getActive() == 1){
            testView.draw(render, textRender);
        }

        
        
        window.swapBuffers();
        if(tabsPanel.getActive() == 0){
            dfPanel.update(dt);
        }
        if(tabsPanel.getActive() == 1){
            testCtrl.update(testView);
            testView.update(dt);
        }
       
        tabsPanel.update(dt);

    }
    glfwTerminate();
    return 0;
}


