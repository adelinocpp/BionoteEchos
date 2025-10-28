#include "controllers/AnnotationController.h"
#include "models/Project.h"

AnnotationController::AnnotationController(std::shared_ptr<Project> project, QObject *parent)
    : QObject(parent), m_project(project) {}
AnnotationController::~AnnotationController() {}
