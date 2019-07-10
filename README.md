# Game2048

这个C++项目实现了一个简单的Game2048游戏。里面有许多不错的设计。

## 单例模式

Controller是使用单例模式实现的。

Controller.h
class Controller {
    static Controller * controller; // 单例

    // 构造方法和拷贝构造方法私有
    Controller();
    Controller(const Controller& controller);
    ~Controler();
public: 
    static Controller& getInstance();
    static void destroy();

    // controller的内存管理方式是：
    //      1. 通过getInstance()创建单例
    //      2. 通过destroy()销毁单例
}

Controller.cpp
#include "Controller.h"

Controller * Controller::controller = NULL;

Controller Controller::getInstance() {
    if (controller == NULL) {
        controller = new Controller();
    }
    return *controller;
}

void Controller::destroy() {
    delete controller;
    controller = NULL; // 这一步也很关键！
}


Controller::Controller() {
    // initialization
} 

Controller::~Controller() {
    // clear
}
