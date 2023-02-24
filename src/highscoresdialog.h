#ifndef HIGHSCORESDIALOG_H
#define HIGHSCORESDIALOG_H

/**************************************************************************
* Copyright (c) 2023 by Jeff Bienstadt                                    *
*                                                                         *
* This file is part of the tripleytz project.                             *
*                                                                         *
* tripleytz is free software: you can redistribute it and/or modify       *
* it under the terms of the GNU General Public License as published by    *
* the Free Software Foundation, either version 3 of the License, or       *
* (at your option) any later version.                                     *
*                                                                         *
* tripleytz is distributed in the hope that it will be useful, but        *
* WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU        *
* General Public License for more details.                                *
*                                                                         *
* You should have received a copy of the GNU General Public License along *
* with tripleytz. If not, see <https://www.gnu.org/licenses/>.            *
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
