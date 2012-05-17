#ifndef XMLNOTE_H
#define XMLNOTE_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextFrame>

class XmlNote : protected QXmlStreamWriter /*, protected QXmlStreamReader*/
{
public:
    XmlNote();
    XmlNote(QString* outputString);

    void setOutputDevice(QIODevice * device)    { QXmlStreamWriter::setDevice(device);}
    QIODevice * outputDevice() const            { return QXmlStreamWriter::device();}
//    void setInputDevice(QIODevice * device)     { QXmlStreamReader::setDevice(device);}
//    QIODevice * inputDevice() const             { return QXmlStreamReader::device();}

    // obtain this via     QTextFrame* frame = textEdit->document()->rootFrame();
    void setFrame(QTextFrame * frame)       {   frame_ = frame;}
    QTextFrame * frame() const              { return frame_;}

    void setNoteTitle(const QString& title)     { title_ = title;}
    const QString& noteTitle() const            { return title_;}

    void write(); // write the content's of frame to the specified device/outputString

    QString title_;
    QString * outputString_;
     QTextFrame * frame_;
};

#endif // XMLNOTE_H
