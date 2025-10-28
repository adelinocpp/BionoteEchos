#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>
#include <memory>

class Project;

class AudioController : public QObject
{
    Q_OBJECT
public:
    explicit AudioController(std::shared_ptr<Project> project, QObject *parent = nullptr);
    ~AudioController();
    
private:
    std::shared_ptr<Project> m_project;
};

#endif // AUDIOCONTROLLER_H
