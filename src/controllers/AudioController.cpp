#include "controllers/AudioController.h"
#include "models/Project.h"

AudioController::AudioController(std::shared_ptr<Project> project, QObject *parent)
    : QObject(parent), m_project(project) {}
AudioController::~AudioController() {}
