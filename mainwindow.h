#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "UiHandler.hpp"
#include "DataHandler.hpp"
#include <QMainWindow>
#include "HighlightDelegate.hpp"

using std::vector;

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
    UiHandler mUiHandler;
    DataHandler mDataHandler;
    bool isHidden = false;

    void init();

    void onReloadTable();
    void onShowItem(QTableWidgetItem *item);
    void onMarkItem(QTableWidgetItem *item);
    void onFocusItem(QTableWidgetItem *item);
    void onRefreshLog();
    void onSetTagHighLight();
    void onSetMsgHighLight();

signals:
    void windowResized();
};
#endif // MAINWINDOW_H
