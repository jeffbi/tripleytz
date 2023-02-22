#ifndef HIGHSCORESDIALOG_H
#define HIGHSCORESDIALOG_H

/**************************************************************************
* Copyright (c) 2023 by Jeff Bienstadt                                    *
*                                                                         *
* This file is part of the tripleytz project.                             *
*                                                                         *
* Released under the MIT license. See the file LICENSE for more details.  *
**************************************************************************/

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
