#include "findfilemodel.h"
#include <QFileIconProvider>
#include <QDirIterator>
#include <QTextStream>
#include <QUrl>
#include <QTextDocumentFragment>


FindFileModel::FindFileModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

QString FindFileModel::fileName(const QModelIndex &index) const
{
    return QFileInfo(itemFromIndex(index)->data(Qt::UserRole + 1).toString()).fileName();
}

QString FindFileModel::filePath(const QModelIndex &index) const
{
    return QFileInfo(itemFromIndex(index)->data(Qt::UserRole + 1).toString()).filePath();
}

bool FindFileModel::remove(const QModelIndex &index)
{
    bool b = QFile::remove(itemFromIndex(index)->data(Qt::UserRole + 1).toString());
    if(b)
         this->removeRow(index.row(),index.parent());
    return b;
}

QFileInfo FindFileModel::fileInfo(const QModelIndex &index) const
{
    return QFileInfo(itemFromIndex(index)->data(Qt::UserRole + 1).toString());
}

void FindFileModel::appendFile(QString filePath)
{
    QFileInfo info(filePath);
    if(info.path().isEmpty() || info.path() == ".")
    {
        qWarning("FindFileModel::appendFile failed: filePath must contain the full path including the file name");
        return;
    }

    QString filePathTrunc = info.filePath();

    while(filePathTrunc.count("/") > 1)
      filePathTrunc.remove(0,filePathTrunc.indexOf("/")+1);

    QStandardItem * fileItem = new QStandardItem(filePathTrunc);
    fileItem->setIcon(QFileIconProvider().icon(info));
    fileItem->setData(filePath,Qt::UserRole + 1); // store as user data
    appendRow(fileItem);
}

bool FindFileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
     QStandardItemModel::setData(index,value,role);
     return QFile::rename(filePath(index),fileInfo(index).path() + "/" + value.toString());
}


/*static*/ QStringList  FindFileModel::find(const QString &searchName, const QString &searchText,const QString& path)
{
    if(searchName.isEmpty() && searchText.isEmpty())
        return QStringList();

    QStringList files;
    QStringList results; // found files

    QDirIterator it(path, QDirIterator::Subdirectories);
    while(it.hasNext())
      files << it.next();

    if(!searchText.isEmpty())
      files = findFiles(files, searchText,path);

    while(!files.isEmpty()){
      QString file = files.takeFirst();
      QFileInfo info(file);

      // if no fileName given, search for content only, if filenName given, search for files that contain that name
      if(searchName.isEmpty() || (!searchName.isEmpty() && !info.isDir() && info.baseName().contains(searchName,Qt::CaseInsensitive)))
        results.append(file);
    }
    return results;
}

QStringList FindFileModel::mimeTypes() const
{
    return QStringList(QString("text/uri-list"));
}

QMimeData *FindFileModel::mimeData(const QModelIndexList &indexes) const
{
    QList<QUrl> urls;
    for(QModelIndexList::ConstIterator it = indexes.constBegin(); it != indexes.constEnd(); ++it)
    {
        urls+=QUrl::fromLocalFile(this->filePath(*it));
    }
    QMimeData * mimeData = new QMimeData();
    mimeData->setUrls(urls);
    return mimeData;
}

/*static*/ QStringList FindFileModel::findFiles(const QStringList &files, const QString &text, const QString &path){
//     QProgressDialog progressDialog(this);
//     progressDialog.setCancelButtonText(tr("&Cancel"));
//     progressDialog.setRange(0, files.size());
//     progressDialog.setWindowTitle(tr("Find Files"));

     QStringList foundFiles;

     for(int i = 0; i < files.size(); ++i){
//        progressDialog.setValue(i);
//        progressDialog.setLabelText(tr("Searching file number %1 of %2...")
//          .arg(i).arg(files.size()));
//        qApp->processEvents();

//        if(progressDialog.wasCanceled())
//          break;

        QFile file(QDir(path).absoluteFilePath(files[i]));

        if(file.open(QIODevice::ReadOnly)){
          QTextStream in(&file);
          QTextDocumentFragment doc = QTextDocumentFragment::fromHtml(in.readAll());
          QString noteText = doc.toPlainText();
          if(noteText.contains(text))
            foundFiles << files[i];
        }
        if(file.isOpen())
          file.close();
     }
     return foundFiles;
}
