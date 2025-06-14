#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP
#include "Log.hpp"
#include "./ui_mainwindow.h"
#include "FileLogHelper.hpp"
#include <vector>
#include "HighlightDelegate.hpp"

using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui {
class UiHandler;
}
QT_END_NAMESPACE
class UiHandler
{
private:
    int LINE_HEIGTH = 30;
    HighlightDelegate *mTagHLDelegate;
    HighlightDelegate *mMsgHLDelegate;
    void setBackgroundRow(QTableWidget *table, QBrush color, int row);
    void setForegroundRow(QTableWidget *table, QBrush color, int row);
public:
    void setTagHighLight(Ui::MainWindow *ui, const QString &tag);
    void setMsgHighLight(Ui::MainWindow *ui, const QString &msg);
    void loadLogs(Ui::MainWindow *ui, vector<Log> logs);
    void updateLogShow(Ui::MainWindow *ui, QTableWidgetItem *item);
    void markLog(Ui::MainWindow *ui, FileLogHelper &fileLogHelper, QTableWidgetItem *item);
    void focusLog(Ui::MainWindow *ui, QTableWidgetItem *item);
    void initUi(Ui::MainWindow *ui);
};

#endif // UIHANDLER_HPP
