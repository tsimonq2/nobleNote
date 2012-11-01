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

#ifndef BACKUP_H
#define BACKUP_H

#include "ui_backup.h"
#include "htmlnotereader.h"
#include "abstractnotereader.h"
#include "progressreceiver.h"
#include <QFileInfo>
#include <QTextDocument>
#include <QProgressDialog>
#include <QFutureWatcher>
#include <QUuid>

class ProgressReceiver;

class Backup : public QDialog, public Ui::Backup {
     Q_OBJECT
 
     public:
      Backup(QWidget *parent = 0);

     private:
      QTextDocument *document;
      QPushButton   *deleteOldButton;
      QStringList   noteFiles;
      QFutureWatcher<QString> *future1;
      QFutureWatcher<void> *future2;
      ProgressReceiver *progressReceiver1, *progressReceiver2;
      QProgressDialog  *progressDialog1, *progressDialog2;
      QStringList noteUuidList;
      QList<QFileInfo> backupFiles; //don't ever use a local stack variable
      QHash<QString,QStringList> *backupDataHash;

      struct GetUuid
      {
           ProgressReceiver *p;

           typedef QString result_type;

           QString operator()(QString file)
           {
                p->postProgressEvent();
                return HtmlNoteReader::uuid(file);
           }
      };

      struct SetupBackup
      {
           ProgressReceiver *p;
           QHash<QString,QStringList> *hash;

           void operator()(const QFileInfo &file)
           {

                QTextDocument *d = new QTextDocument;
                AbstractNoteReader *reader = new HtmlNoteReader(file.absoluteFilePath(),d);
                QStringList data;
                data << reader->title() << file.absoluteFilePath() << d->toHtml();
                hash->insert(file.absoluteFilePath(),data);

                p->postProgressEvent();
           }
      };

      GetUuid getUuid;
      SetupBackup setupBackup;

     private slots:
      void showPreview();
      void restoreBackup();
      void deleteBackup();
      void getNoteUuidList();
      void getNotes();
      void setupBackups();
      void setupChildren();
};

#endif //BACKUP_H
