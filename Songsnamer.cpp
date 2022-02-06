#include "Songsnamer.h"
#include <QDebug>

Songsnamer::Songsnamer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void Songsnamer::fillListWithMp3Files(QString pathToFolder){
	QDir dir(pathToFolder);
	QStringList fileNames = dir.entryList(QStringList() << "*.mp3");
	fileNames.sort();
	for each (QString fileName in fileNames){
		ui.songsInFolderView->addItem(fileName);
	}
	currentSong = 0;
	songCount = fileNames.count() - 1;
	refreshSongNumberNumber();
}

void Songsnamer::refreshSongNumberNumber(){
	ui.songlabel->setText(QString::number(currentSong) + "/" + QString::number(songCount));
}

void Songsnamer::selectFolderButtonPressed(){
	QFileDialog fileDialog(this);
	fileDialog.setFileMode(QFileDialog::Directory);
	bool ok = fileDialog.exec();
	if (ok) {
		QString selectedFolderPath = fileDialog.selectedFiles()[0];
		if (selectedFolderPath.length() > 0){
			ui.pathLabel->setText(selectedFolderPath);
			fillListWithMp3Files(selectedFolderPath);
			pathToFolder = selectedFolderPath;
		}
	}
}

void Songsnamer::itemOnSongListClicked(){
	currentSong = ui.songsInFolderView->currentRow();
	QString text = ui.songsInFolderView->item(currentSong)->text();
	refreshSongNumberNumber();
	//qDebug(text.toStdString().c_str());
	text = text.left(text.length() - 4);
	ui.titleText->setPlainText(text);
	ui.artistText->setPlainText("");
}

void Songsnamer::changeName(){
	QString artistName = ui.artistText->toPlainText();
	QString title = ui.titleText->toPlainText();
	auto itemsOnListForArtist = ui.artistsAlreadyUsedList->findItems(artistName, Qt::MatchExactly);
	bool artistIsNotOnTheList = itemsOnListForArtist.size() == 0;
	if (artistIsNotOnTheList)
		ui.artistsAlreadyUsedList->addItem(artistName);
	
	QString separator = ui.separatorField->toPlainText();
	QString orgName = ui.songsInFolderView->currentItem()->text();
	QString orgPath = pathToFolder + "//" + orgName;
	QString newName = artistName + separator + title + ".mp3";
	QString newPath = pathToFolder + "//" + newName;
	QFile file(orgPath);
	file.rename(newPath);
}

void Songsnamer::restartArtistNames(){
	ui.artistsAlreadyUsedList->clear();
	ui.artistsAlreadyUsedList->addItem("Other");
}

void Songsnamer::artistNameOnListClicked(){
	QString artistName = ui.artistsAlreadyUsedList->currentItem()->text();
	ui.artistText->setPlainText(artistName);
}

void Songsnamer::previousButtonPressed() {
	if (currentSong == 0)
		return;
	currentSong--;
	ui.songsInFolderView->setCurrentItem(ui.songsInFolderView->item(currentSong));
	itemOnSongListClicked();
}

void Songsnamer::nextButtonPressed(){
	if (currentSong == songCount)
		return;
	currentSong++;
	ui.songsInFolderView->setCurrentItem(ui.songsInFolderView->item(currentSong));
	itemOnSongListClicked();
}

