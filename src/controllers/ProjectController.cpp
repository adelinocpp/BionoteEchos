#include "controllers/ProjectController.h"
#include "models/Project.h"

ProjectController::ProjectController(std::shared_ptr<Project> project, QObject *parent)
    : QObject(parent), m_project(project) {}
ProjectController::~ProjectController() {}
