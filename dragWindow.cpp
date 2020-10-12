#include "dragWindow.h"
#include <QApplication>
#include <QtWidgets>
#include <QFrame>
#include <QLabel>
#include <QDrag>

static QString hotSpotMimeDataKey() {return QStringLiteral("application/x-hotspot");}

dragWindow::dragWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Drag and Drop Demo");
    setFixedSize(800,800);
    setAcceptDrops(true);

    //create some labels to serve as nodes
    QLabel *label1 = new QLabel(this);
    label1->setText("Node 1");
    label1->setFrameShape(QFrame::Panel);
    label1->setFrameShadow(QFrame::Raised);

    QLabel *label2 = new QLabel(this);
    label2->setText("Node 2");
    label2->setFrameShape(QFrame::Panel);
    label2->setFrameShadow(QFrame::Raised);

    QLabel *label3 = new QLabel(this);
    label3->setText("Node 3");
    label3->setFrameShape(QFrame::Panel);
    label3->setFrameShadow(QFrame::Raised);

    QLabel *label4 = new QLabel(this);
    label4->setText("Node 4");
    label4->setFrameShape(QFrame::Panel);
    label4->setFrameShadow(QFrame::Raised);

    //some initial positions of the nodes
    label1->move(10,10);
    label2->move(590,10);
    label3->move(10,590);
    label4->move(590,590);

    //color the window for contrast
    //QPalette newPalette = palette();
    //newPalette.setColor(QPalette::Window, Qt::white);
    //setPalette(newPalette);

}

void dragWindow::dragEnterEvent(QDragEnterEvent *event){
    if(event->mimeData()->hasText()){
        if(event->source() == this){
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else{
            event->acceptProposedAction();
        }
    }
    else{
        event->ignore();
    }
}

//void dragWindow::dragMoveEvent(QDragMoveEvent *event){}

void dragWindow::dropEvent(QDropEvent *event){
    if (event->mimeData()->hasText()) {
            const QMimeData *mime = event->mimeData();
            //QStringList pieces = mime->text().split(QRegularExpression(QStringLiteral("\\s+")), QString::SplitBehavior::SkipEmptyParts);
            QPoint position = event->pos();
            QPoint hotSpot;

            QByteArrayList hotSpotPos = mime->data(hotSpotMimeDataKey()).split(' ');
            if (hotSpotPos.size() == 2) {
                hotSpot.setX(hotSpotPos.first().toInt());
                hotSpot.setY(hotSpotPos.last().toInt());
            }

            //for (const QString &piece : pieces) {
                QLabel *newLabel = new QLabel(this);
                newLabel->setText(mime->text());
                newLabel->setFrameShape(QFrame::Panel);
                newLabel->setFrameShadow(QFrame::Raised);
                newLabel->move(position - hotSpot);
                newLabel->show();
                newLabel->setAttribute(Qt::WA_DeleteOnClose);

                position += QPoint(newLabel->width(), 0);
            //}

            if (event->source() == this) {
                event->setDropAction(Qt::MoveAction);
                event->accept();
            } else {
                event->acceptProposedAction();
            }
        } else {
            event->ignore();
        }
        for (QWidget *widget : findChildren<QWidget *>()) {
            if (!widget->isVisible())
                widget->deleteLater();
        }
}

void dragWindow::mousePressEvent(QMouseEvent *event){
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if(!child){return;}

    QPoint hotSpot = event->pos() - child->pos();
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(child->text());
    mimeData->setData(hotSpotMimeDataKey(), QByteArray::number(hotSpot.x()) + ' ' + QByteArray::number(hotSpot.y()));

    qreal dpr = window()->windowHandle()->devicePixelRatio();
    QPixmap pixmap(child->size()*dpr);
    pixmap.setDevicePixelRatio(dpr);
    child->render(&pixmap);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(hotSpot);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

    if(dropAction == Qt::MoveAction){
        child->close();
    }

}

