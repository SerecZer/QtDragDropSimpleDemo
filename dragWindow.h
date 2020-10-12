#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>

class dragWindow : public QWidget
{
    Q_OBJECT
public:
    explicit dragWindow(QWidget *parent = nullptr);

private:
    void dragEnterEvent(QDragEnterEvent *event) override;
    //void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

};

#endif // WINDOW_H
