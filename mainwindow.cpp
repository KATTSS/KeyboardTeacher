#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTimer"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    statusBar = new QHBoxLayout(this);
    languages = new QComboBox(this);
    languages->addItem("de");
    languages->addItem("fr");
    languages->addItem("ara");
    languages->addItem("by");
    languages->addItem("il");
    languages->addItem("ru");
    languages->addItem("us");
    languages->setFocusPolicy(Qt::NoFocus);

    accuracy = new QLabel("Accuracy: 100%", this);
    speed = new QLabel ("Speed in sym/sec: ", this);
    time = new QLabel ("Time: ", this);
    statusBar->addWidget(languages);
    statusBar->addWidget((accuracy));
    statusBar->addWidget(speed);
    statusBar->addWidget(time);
    mainLayout->addLayout(statusBar);

    text = new QTextEdit(this);
    text->setReadOnly(true);
    text->setStyleSheet("font-size: 25px; color: gray");
    text->setText("Choose a language");
    mainLayout->addWidget(text);

    keyboardScene = new QGraphicsScene(this);
    keyboardView = new QGraphicsView(keyboardScene, this);
    keyboardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    keyboardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    keyboardView->show();
    mainLayout->addWidget(keyboardView);

    connect(languages, &QComboBox::currentTextChanged,this, &MainWindow::update);

    textGetter = new RandomText("/home/katya/work/453501/ОАиП/LR5/Words.txt");
       createKeyboard("us");
    cursor=0;

    start = QTime::currentTime();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateSpeed()));
    connect(timer, SIGNAL(updateT()), this, SLOT(updateTime()));
    timer->start(10000);
}

MainWindow::~MainWindow()
{
    xkb_state_unref(state);
    xkb_keymap_unref(keymap);
    xkb_context_unref(ctx);
    delete ui;
}

void MainWindow::updateSpeed()
{
    speed->setText("Speed in sym/sec: "+ QString::number(symbols/10.0));
    updateTime();
    updateAccuracy();
    symbols=0;
}

void MainWindow::updateTime()
{
    time->setText("Time(sec): "+ QString::number(start.secsTo(QTime::currentTime())));

}

void MainWindow::updateAccuracy()
{
    accuracy->setText("Accuracy: " + QString::number(100*(cursor)/allsymbols)+"%");
}

void MainWindow::createKeyboard(QString language)
{
    buttons.clear();
    keyboardScene->clear();
    listChar.clear();
    workWithXKB(language);

    const int keyWidth = 50;
    const int keyHeight = 50;
    if (listChar.isEmpty()) {
             return;
    }

    for (int i=0; i<12; ++i) {
        //qDebug() << listChar[i] << "1";
        buttons.push_back(new Key(keyboardScene, listChar[i], QRect(0 + i * 55,0,keyWidth, keyHeight), i));
        keyboardScene->addItem(buttons.back());
    }
    for (int i=0; i<11; ++i) {
        //qDebug() << listChar[i+13] << "2";
        buttons.push_back(new Key(keyboardScene, listChar[i+13], QRect(25 + i * 55,55,keyWidth, keyHeight), i));
        keyboardScene->addItem(buttons.back());
    }
    for (int i=0; i<10; ++i) {
        //qDebug() << listChar[i+26] << "3";
        buttons.push_back(new Key(keyboardScene, listChar[i+26], QRect(50 + i * 55,110,keyWidth, keyHeight), i));
        keyboardScene->addItem(buttons.back());
    }
}

int MainWindow::getLanguageIndex()
{
    return languages->currentIndex();
}

void MainWindow::updateText()
{
    text->clear();
    text->setText(textGetter->getRandomText(getLanguageIndex()));
    QTextCharFormat format;
    format.setForeground(Qt::gray);
    QTextCursor textCursor = text->textCursor();
    textCursor.select(QTextCursor::Document);
    textCursor.mergeCharFormat(format);
    textCursor.clearSelection();
    text->setTextCursor(textCursor);

}

void MainWindow::update()
{

    cursor=0;
    allsymbols=0;
    getAppropriateText();
    updateText();
    emit updateT();
    start=QTime::currentTime();
    accuracy->setText("Accuracy: 100%");
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    grabKeyboard();
    ++symbols;
    ++allsymbols;
\
    int code = event->nativeScanCode();
    QChar actual;
    char buffer[32];
    int size = xkb_state_key_get_utf8(state, code, buffer, sizeof(buffer));
    if (size > 0) {
        actual = QString::fromUtf8(buffer, size).at(0).toLower();
    } else {
        actual = ' ';
    }

    QChar expected = text->document()->characterAt(cursor).toLower();

    qDebug() << "Expected:" << expected << "Actual:" << actual;

    QTextCharFormat format;
    QTextCursor textCursor;
    if (expected == actual) {
        cursor++;
        textCursor = text->textCursor();
        textCursor.setPosition(cursor-1);
        textCursor.setPosition(cursor, QTextCursor::KeepAnchor);
        format.setForeground(Qt::black);
        textCursor.mergeCharFormat(format);
       // text->setTextCursor(textCursor);
        textCursor.setPosition(cursor);
        textCursor.setPosition(cursor + 1, QTextCursor::KeepAnchor);
        text->setTextCursor(textCursor);
        qDebug() << "Correct input, cursor moved to:" << cursor;
    } else {

        textCursor = text->textCursor();
        textCursor.setPosition(cursor);
        textCursor.setPosition(cursor + 1, QTextCursor::KeepAnchor);
        text->setTextCursor(textCursor);
        qDebug() << "Incorrect input, waiting for next attempt";
    }

    if (cursor >= text->document()->characterCount() - 1) {
        qDebug() << "Text completed!";
        cursor = 0;
        updateText();
    }
}

void MainWindow::workWithXKB(QString language)
{
    ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!ctx) qDebug() << "error with xkb context in createKeyboard";

    struct xkb_rule_names names = {
        .rules = NULL,
        .model = "pc105",
        .layout = language.toStdString().c_str(),
        .variant = NULL,
        .options = ""
    };

    keymap = xkb_keymap_new_from_names(ctx, &names,
                                       XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (!keymap) {qDebug() << "error with xkb keymap in createKeyboard";
        xkb_context_unref(ctx);
        return;
    }

    state = xkb_state_new(keymap);
    if (!state) { qInfo() << "Error";
        xkb_keymap_unref(keymap);
        xkb_context_unref(ctx);
        return;
    }

    for(xkb_keycode_t i = 24; i < 256; i++)
    {
        xkb_keysym_t keysym = xkb_state_key_get_one_sym(state, i);
        char buffer[32];
        int size = xkb_keysym_to_utf8(keysym, buffer, sizeof(buffer));
        if(size > 0)
        {
            listChar.push_back(QString::fromUtf8(buffer).at(0).toUpper());
        }
    }
}

void MainWindow::getAppropriateText()
{
    QString language = languages->currentText();
    createKeyboard(language);
}
