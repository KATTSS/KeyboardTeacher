#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QDebug>
#include "key.h"
#include <xkbcommon/xkbcommon.h>
#include <string.h>
#include "randomtext.h"
#include <QLineEdit>
#include <QKeyEvent>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QVBoxLayout *mainLayout;

    QHBoxLayout *statusBar;
    QComboBox *languages;
    QLabel *accuracy;
    QLabel *speed;

    QTextEdit *text;

    QLineEdit* checkline;

    QGraphicsView *keyboardView;
    QGraphicsScene *keyboardScene;

    QList<Key*> buttons;
    QList<QChar> listChar;

    int cursor;

    RandomText *textGetter;

    struct xkb_context *ctx = nullptr;
    struct xkb_keymap *keymap = nullptr;
    struct xkb_state *state = nullptr;

    void createKeyboard(QString language);
    void getAppropriateText();
    int getLanguageIndex();
    void updateText();

    void update();

    void keyPressEvent(QKeyEvent *event) override;
    void workWithXKB(QString language);

};
#endif // MAINWINDOW_H
