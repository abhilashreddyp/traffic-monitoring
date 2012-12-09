#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QImage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void teste(QImage img);

signals:
    void closed();

protected:
    virtual void closeEvent(QCloseEvent* event);
    
private:
    Ui::MainWindow *ui;
};

#endif // MAIN_WINDOW_HPP
