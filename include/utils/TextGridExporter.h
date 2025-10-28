#ifndef TEXTGRIDEXPORTER_H
#define TEXTGRIDEXPORTER_H

#include <QString>
#include <QObject>

class TextGridExporter : public QObject
{
    Q_OBJECT
public:
    explicit TextGridExporter(QObject *parent = nullptr);
    ~TextGridExporter();
};

#endif // TEXTGRIDEXPORTER_H
