#include "dragWindow.h"
#include <QApplication>

int main(int argc, char* argv[]){
    QApplication app(argc, argv);

    dragWindow Window;
    Window.show();

    return app.exec();

}
