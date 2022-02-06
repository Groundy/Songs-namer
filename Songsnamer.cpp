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
	QString separator = ui.separatorField->toPlainText();
	fileNames.sort();
	for each (QString fileName in fileNames){
		ui.songsInFolderView->addItem(fileName);

		QStringList fileNameParts = fileName.split(separator);
		bool fileNameHasProperFormat = fileNameParts.size() == 2;
		QString artistName = fileNameParts[0];
		bool artistNameIsCommon = artistName[0] != '_';
		bool artistIsNotOnListYet = ui.artistsAlreadyUsedList->findItems(artistName, Qt::MatchExactly).size() == 0;
		bool addArtist = fileNameHasProperFormat && artistNameIsCommon && artistIsNotOnListYet;
		if (addArtist)
			ui.artistsAlreadyUsedList->addItem(artistName);
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
	currentSong = 0;
	ui.songsInFolderView->clear();
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

	QString separator = ui.separatorField->toPlainText();
	bool separatorFoundInFileName = text.indexOf(separator) != -1;
	QString artistField = "", titleField = text;
	if(separatorFoundInFileName) {
		QStringList fileNameParts = text.split(separator);
		if (fileNameParts.size() == 2) {
			titleField = fileNameParts[1];
			artistField = fileNameParts[0];
		}
	}
	ui.titleText->setPlainText(titleField);
	ui.artistText->setPlainText(artistField);
}

void Songsnamer::changeName(){
	QString artistName = ui.artistText->toPlainText();
	QString title = ui.titleText->toPlainText();
	auto itemsOnListForArtist = ui.artistsAlreadyUsedList->findItems(artistName, Qt::MatchExactly);
	bool artistIsNotOnTheList = itemsOnListForArtist.size() == 0;
	if (artistIsNotOnTheList && artistName[0] != '_')
		ui.artistsAlreadyUsedList->addItem(artistName);
	
	QString separator = ui.separatorField->toPlainText();
	QString orgName = ui.songsInFolderView->currentItem()->text();
	QString orgPath = pathToFolder + "//" + orgName;
	QString newName = artistName + separator + title + ".mp3";
	QString newPath = pathToFolder + "//" + newName;
	QFile file(orgPath);
	file.rename(newPath);
	ui.songsInFolderView->currentItem()->setText(newName);
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

