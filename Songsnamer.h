#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_Songsnamer.h"
#include <stdio.h>
#include <qdebug.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qlistwidget.h>

class Songsnamer : public QMainWindow
{
    Q_OBJECT

public:
    Songsnamer(QWidget *parent = Q_NULLPTR);
    void fillListWithMp3Files(QString pathToFolder);
    void refreshSongNumberNumber();
public slots:
    void previousButtonPressed();
    void nextButtonPressed();
    void selectFolderButtonPressed();
    void itemOnSongListClicked();
    void changeName();
    void restartArtistNames();
    void artistNameOnListClicked();

private:
    Ui::SongsnamerClass ui;
    int songCount = 0;
    int currentSong = 0;
    QString pathToFolder;
};
