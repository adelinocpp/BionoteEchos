#ifndef PROJECTCONTROLLER_H
#define PROJECTCONTROLLER_H

#include <QObject>
#include <memory>

class Project;

class ProjectController : public QObject
{
    Q_OBJECT
public:
    explicit ProjectController(std::shared_ptr<Project> project, QObject *parent = nullptr);
    ~ProjectController();
    
private:
    std::shared_ptr<Project> m_project;
};

#endif // PROJECTCONTROLLER_H
