#include "models/Project.h"
#include "models/AudioFile.h"
#include "models/AnnotationTier.h"
#include <QFileInfo>

Project::Project(QObject *parent)
    : QObject(parent)
    , m_modified(false)
{
}

Project::~Project()
{
}

QString Project::getProjectName() const
{
    if (m_projectPath.isEmpty()) {
        return tr("Untitled Project");
    }
    
    QFileInfo fileInfo(m_projectPath);
    return fileInfo.baseName();
}

std::shared_ptr<AudioFile> Project::getAudioFile(int index) const
{
    if (index >= 0 && index < m_audioFiles.size()) {
        return m_audioFiles[index];
    }
    return nullptr;
}

std::shared_ptr<AnnotationTier> Project::getTier(int index) const
{
    if (index >= 0 && index < m_tiers.size()) {
        return m_tiers[index];
    }
    return nullptr;
}

void Project::setProjectPath(const QString &path)
{
    if (m_projectPath != path) {
        m_projectPath = path;
        emit projectPathChanged(path);
    }
}

void Project::setModified(bool modified)
{
    if (m_modified != modified) {
        m_modified = modified;
        if (modified) {
            emit projectModified();
        }
    }
}

int Project::addAudioFile(std::shared_ptr<AudioFile> audioFile)
{
    if (!audioFile) {
        return -1;
    }
    
    m_audioFiles.append(audioFile);
    int index = m_audioFiles.size() - 1;
    
    setModified(true);
    emit audioFileAdded(index);
    
    return index;
}

bool Project::removeAudioFile(int index)
{
    if (index >= 0 && index < m_audioFiles.size()) {
        m_audioFiles.removeAt(index);
        setModified(true);
        emit audioFileRemoved(index);
        return true;
    }
    return false;
}

int Project::addTier(std::shared_ptr<AnnotationTier> tier)
{
    if (!tier) {
        return -1;
    }
    
    m_tiers.append(tier);
    int index = m_tiers.size() - 1;
    
    setModified(true);
    emit tierAdded(index);
    
    return index;
}

bool Project::removeTier(int index)
{
    if (index >= 0 && index < m_tiers.size()) {
        m_tiers.removeAt(index);
        setModified(true);
        emit tierRemoved(index);
        return true;
    }
    return false;
}

int Project::findAudioFile(const QString &filePath) const
{
    for (int i = 0; i < m_audioFiles.size(); ++i) {
        if (m_audioFiles[i]->getFilePath() == filePath) {
            return i;
        }
    }
    return -1;
}

int Project::findTier(const QString &name) const
{
    for (int i = 0; i < m_tiers.size(); ++i) {
        if (m_tiers[i]->getName() == name) {
            return i;
        }
    }
    return -1;
}

void Project::clear()
{
    m_audioFiles.clear();
    m_tiers.clear();
    m_projectPath.clear();
    m_modified = false;
    
    emit projectCleared();
}

bool Project::isEmpty() const
{
    return m_audioFiles.isEmpty() && m_tiers.isEmpty();
}

void Project::newProject()
{
    clear();
}

