#ifndef ANNOTATIONCONTROLLER_H
#define ANNOTATIONCONTROLLER_H

#include <QObject>
#include <memory>

class Project;

class AnnotationController : public QObject
{
    Q_OBJECT
public:
    explicit AnnotationController(std::shared_ptr<Project> project, QObject *parent = nullptr);
    ~AnnotationController();
    
private:
    std::shared_ptr<Project> m_project;
};

#endif // ANNOTATIONCONTROLLER_H
