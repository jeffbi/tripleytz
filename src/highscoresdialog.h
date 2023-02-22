#ifndef HIGHSCORESDIALOG_H
#define HIGHSCORESDIALOG_H

#include <QDialog>

#include <vector>

//#include "config.h"
class Config;
class QTreeWidget;
class QPushButton;

///
/// \brief Controls a dialog box for displaying the high scores
///
class HighScoresDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HighScoresDialog(Config &config, QWidget *parent = nullptr);

private:
    void populate_tree();

private:
    QTreeWidget    *_tree;
    QPushButton    *_clear;
    QPushButton    *_ok;
    Config         &_config;

private slots:
    void clear_clicked();
};

#endif // HIGHSCORESDIALOG_H
