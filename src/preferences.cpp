/* nobleNote, a note taking application
 * Copyright (C) 2012 Christian Metscher <hakaishi@web.de>,
                      Fabian Deuchler <Taiko000@gmail.com>

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.

 * nobleNote is licensed under the MIT, see `http://copyfree.org/licenses/mit/license.txt'.
 */

#include "preferences.h"
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QFileDialog>
#include <QDesktopServices>

Preferences::Preferences(QWidget *parent): QDialog(parent){
     setupUi(this);

     settings = new QSettings(this);

     dontQuit->setChecked(settings->value("dont_quit_on_close",false).toBool());
     convertNotes->setChecked(settings->value("convert_notes",true).toBool());
     showSource->setChecked(settings->value("show_source", false).toBool());
     kineticScrolling->setChecked(settings->value("kinetic_scrolling", false).toBool());
     sizeSpinHeight->setValue(settings->value("note_editor_default_size",QSize(335,250)).toSize().height());
     sizeSpinWidth->setValue(settings->value("note_editor_default_size",QSize(335,250)).toSize().width());


     connect(buttonBox, SIGNAL(accepted()), this, SLOT(saveSettings()));
     connect(browseButton, SIGNAL(clicked(bool)), this, SLOT(openDir()));
     connect(kineticScrolling,SIGNAL(toggled(bool)),this,SIGNAL(kineticScrollingEnabledChanged(bool)));
}

void Preferences::showEvent(QShowEvent* show_pref){
     pathLabel->setText(settings->value("root_path").toString());
     rootPath = settings->value("root_path").toString();
     originalRootPath = rootPath;

     QWidget::showEvent(show_pref);
}

void Preferences::saveSettings(){
     if(!settings->isWritable()){
       QMessageBox::warning(this,tr("Warning"),tr("Could not write settings!"));
     }

     if(rootPath != originalRootPath){
       settings->setValue("root_path",rootPath);
#ifdef Q_WS_X11
       settings->setValue("backup_dir_path",QDir::homePath() + "/.local/share/nobleNote/backups");
#else
       settings->setValue("backup_dir_path",QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/nobleNote/backups");
#endif
       pathChanged();
     }

     settings->setValue("dont_quit_on_close", dontQuit->isChecked());
     settings->setValue("convert_notes", convertNotes->isChecked());
     settings->setValue("note_editor_default_size", QSize(sizeSpinWidth->value(),sizeSpinHeight->value()));
     settings->setValue("show_source", showSource->isChecked());
     settings->setValue("kinetic_scrolling",kineticScrolling->isChecked());

     accept();
}

void Preferences::openDir(){
     QString path;
     path = QFileDialog::getExistingDirectory(this,
                  tr("Open Directory"), rootPath, QFileDialog::ShowDirsOnly
                  | QFileDialog::DontResolveSymlinks);
     QFileInfo file(path);
     if(!file.isWritable() && !path.isEmpty()){
         QMessageBox::warning(this,"No Write Access", QString("The path \"%1\" is not writable!").arg(file.filePath()));
       return;
     }
     if(!path.isEmpty()){
       rootPath = path;
       pathLabel->setText(rootPath);
     }
}
