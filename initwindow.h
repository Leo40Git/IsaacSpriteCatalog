#ifndef INITWINDOW_H
#define INITWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class InitWindow;
}

class InitWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit InitWindow(QWidget *parent = nullptr);
    ~InitWindow();

    void showEvent(QShowEvent *event) override;

private:
    Ui::InitWindow *ui;
    QTimer *timer;

    void log(const QString &text);

private slots:
    void doInit();
};

#endif // INITWINDOW_H
