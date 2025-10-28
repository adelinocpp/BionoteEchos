#ifndef TEXTGRIDIMPORTER_H
#define TEXTGRIDIMPORTER_H

#include <QString>
#include <QObject>

class TextGridImporter : public QObject
{
    Q_OBJECT
public:
    explicit TextGridImporter(QObject *parent = nullptr);
    ~TextGridImporter();
};

#endif // TEXTGRIDIMPORTER_H
