#ifndef HTMLNOTEWRITER_H
#define HTMLNOTEWRITER_H

#include <QDateTime>
#include <QUuid>
#include <QTextDocument>
#include <QIODevice>
#include <QFile>
#include <QCoreApplication>

/**
  * a class writing formatted text in html files
  * the resulting files can be read by any web browser
  *
  *
  */

class HtmlNoteWriter
{
    Q_DECLARE_TR_FUNCTIONS(HtmlNoteWriter)
public:
    HtmlNoteWriter(const QString &filePath);

    void write(); // write the content's of document to the specified file

    void setDocument(QTextDocument* document){ document_ = document; }
    QTextDocument* document() const { return document_; }

    // set the note title
    void setTitle( const QString& title){ title_ = title; }
    const QString& title() const { return title_; }

    // uuid is written into the document as a meta element
    void setUuid(QUuid uuid) { uuid_ = uuid;}
    QUuid uuid() const {return uuid_;}

    // does nothing at the moment
    // set last change date, if not set, the current date is used
    void setLastChange(const QDateTime& dt)     { lastChange_ = dt;}
    const QDateTime& lastChange() const         { return lastChange_;}

    // does nothing at the moment
    // set last metadata change date, if not set, the current date is used
    void setLastMetadataChange(const QDateTime& dt)     { lastMetadataChange_ = dt;}
    const QDateTime& lastMetadataChange() const         { return lastMetadataChange_;}

    // does nothing at the moment
    // set create date, if not set, the current date is used
    void setCreateDate(const QDateTime& dt)     { createDate_ = dt;}
    const QDateTime& createDate() const         { return createDate_;}

    // writes the given tomboy xml note as a html note. if creatFolder is true, the file will be created
    // inside a folder. The name of the folder is determined by the <tag> element
    // outputPath is a directory
    static void writeXml(const QString &xmlFilePath, const QString & outputPath, bool createFolder = false);

    // TODO clear statement?

private:
    QString title_;
    QTextDocument * document_;
    QUuid uuid_;
    QDateTime lastChange_;
    QDateTime lastMetadataChange_;
    QDateTime createDate_;

    QString filePath_;
};

#endif // HTMLNOTEWRITER_H
