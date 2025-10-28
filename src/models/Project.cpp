#include "models/Project.h"
#include "models/AudioFile.h"
#include "models/AnnotationTier.h"
#include "models/AnnotationInterval.h"
#include "models/AnnotationPoint.h"
#include <QFileInfo>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

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

bool Project::saveProject(const QString &filePath)
{
    QString savePath = filePath.isEmpty() ? m_projectPath : filePath;
    
    if (savePath.isEmpty()) {
        return false;
    }
    
    // Gerar JSON
    QString jsonData = toJson();
    
    // Salvar em arquivo
    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    file.write(jsonData.toUtf8());
    file.close();
    
    // Atualizar caminho e estado
    if (!filePath.isEmpty()) {
        setProjectPath(savePath);
    }
    setModified(false);
    
    return true;
}

bool Project::loadProject(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    QString jsonData = file.readAll();
    file.close();
    
    // Limpar projeto atual
    clear();
    
    // Carregar do JSON
    if (!fromJson(jsonData)) {
        return false;
    }
    
    // Atualizar caminho
    setProjectPath(filePath);
    setModified(false);
    
    return true;
}

QString Project::toJson() const
{
    QJsonObject root;
    
    // Informações do projeto
    root["version"] = "1.0";
    root["projectName"] = getProjectName();
    root["projectPath"] = m_projectPath;
    
    // Arquivos de áudio
    QJsonArray audioFilesArray;
    for (const auto &audioFile : m_audioFiles) {
        QJsonObject audioObj;
        audioObj["filePath"] = audioFile->getFilePath();
        audioObj["sampleRate"] = audioFile->getSampleRate();
        audioObj["numChannels"] = audioFile->getNumChannels();
        audioObj["duration"] = audioFile->getDuration();
        audioObj["codec"] = audioFile->getCodec();
        audioFilesArray.append(audioObj);
    }
    root["audioFiles"] = audioFilesArray;
    
    // Camadas de anotação
    QJsonArray tiersArray;
    for (const auto &tier : m_tiers) {
        QJsonObject tierObj;
        tierObj["name"] = tier->getName();
        tierObj["type"] = (tier->getType() == TierType::IntervalTier) ? "interval" : "point";
        
        // Intervalos ou pontos
        if (tier->getType() == TierType::IntervalTier) {
            QJsonArray intervalsArray;
            for (int i = 0; i < tier->getIntervalCount(); ++i) {
                auto interval = tier->getInterval(i);
                QJsonObject intervalObj;
                intervalObj["startTime"] = interval->getXMin();
                intervalObj["endTime"] = interval->getXMax();
                intervalObj["label"] = interval->getText();
                intervalsArray.append(intervalObj);
            }
            tierObj["intervals"] = intervalsArray;
        } else {
            QJsonArray pointsArray;
            for (int i = 0; i < tier->getPointCount(); ++i) {
                auto point = tier->getPoint(i);
                QJsonObject pointObj;
                pointObj["time"] = point->getTime();
                pointObj["label"] = point->getText();
                pointsArray.append(pointObj);
            }
            tierObj["points"] = pointsArray;
        }
        
        tiersArray.append(tierObj);
    }
    root["tiers"] = tiersArray;
    
    QJsonDocument doc(root);
    return doc.toJson(QJsonDocument::Indented);
}

bool Project::fromJson(const QString &json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        return false;
    }
    
    QJsonObject root = doc.object();
    
    // Verificar versão
    QString version = root["version"].toString();
    if (version.isEmpty()) {
        return false;
    }
    
    // Carregar arquivos de áudio
    QJsonArray audioFilesArray = root["audioFiles"].toArray();
    for (const QJsonValue &value : audioFilesArray) {
        QJsonObject audioObj = value.toObject();
        QString filePath = audioObj["filePath"].toString();
        
        // Verificar se arquivo existe
        if (!QFile::exists(filePath)) {
            // Tentar caminho relativo ao projeto
            QFileInfo projectInfo(m_projectPath);
            QString relativePath = projectInfo.dir().absoluteFilePath(filePath);
            if (QFile::exists(relativePath)) {
                filePath = relativePath;
            } else {
                // Arquivo não encontrado - pular
                continue;
            }
        }
        
        // Criar AudioFile (será carregado depois)
        // Por enquanto, apenas adicionar referência
        // TODO: Implementar carregamento lazy ou assíncrono
    }
    
    // Carregar camadas de anotação
    QJsonArray tiersArray = root["tiers"].toArray();
    for (const QJsonValue &value : tiersArray) {
        QJsonObject tierObj = value.toObject();
        QString name = tierObj["name"].toString();
        QString type = tierObj["type"].toString();
        
        TierType tierType = (type == "interval") ? TierType::IntervalTier : TierType::PointTier;
        auto tier = std::make_shared<AnnotationTier>(name, tierType);
        
        if (type == "interval") {
            QJsonArray intervalsArray = tierObj["intervals"].toArray();
            for (const QJsonValue &intervalValue : intervalsArray) {
                QJsonObject intervalObj = intervalValue.toObject();
                double startTime = intervalObj["startTime"].toDouble();
                double endTime = intervalObj["endTime"].toDouble();
                QString label = intervalObj["label"].toString();
                
                tier->addInterval(startTime, endTime, label);
            }
        } else {
            QJsonArray pointsArray = tierObj["points"].toArray();
            for (const QJsonValue &pointValue : pointsArray) {
                QJsonObject pointObj = pointValue.toObject();
                double time = pointObj["time"].toDouble();
                QString label = pointObj["label"].toString();
                
                tier->addPoint(time, label);
            }
        }
        
        addTier(tier);
    }
    
    return true;
}

