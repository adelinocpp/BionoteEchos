#ifndef ANNOTATIONTIER_H
#define ANNOTATIONTIER_H

#include <QString>
#include <QVector>
#include <QObject>
#include <memory>

class AnnotationInterval;
class AnnotationPoint;

/**
 * @brief Tipo de camada de anotação
 */
enum class TierType {
    IntervalTier,  // Camada de intervalos (como no Praat)
    PointTier      // Camada de pontos (como no Praat)
};

/**
 * @brief Representa uma camada de anotação (tier) estilo TextGrid do Praat
 * 
 * Uma camada pode ser de dois tipos:
 * - IntervalTier: contém intervalos com início, fim e texto
 * - PointTier (TextTier): contém pontos no tempo com texto
 */
class AnnotationTier : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Construtor
     * @param name Nome da camada
     * @param type Tipo da camada (Interval ou Point)
     * @param xmin Tempo inicial da camada (segundos)
     * @param xmax Tempo final da camada (segundos)
     * @param parent Objeto pai Qt
     */
    explicit AnnotationTier(const QString &name, 
                           TierType type,
                           double xmin = 0.0,
                           double xmax = 0.0,
                           QObject *parent = nullptr);

    /**
     * @brief Destrutor
     */
    ~AnnotationTier();

    // Getters
    QString getName() const { return m_name; }
    TierType getType() const { return m_type; }
    double getXMin() const { return m_xmin; }
    double getXMax() const { return m_xmax; }
    
    int getIntervalCount() const;
    int getPointCount() const;
    
    std::shared_ptr<AnnotationInterval> getInterval(int index) const;
    std::shared_ptr<AnnotationPoint> getPoint(int index) const;
    
    const QVector<std::shared_ptr<AnnotationInterval>>& getIntervals() const { return m_intervals; }
    const QVector<std::shared_ptr<AnnotationPoint>>& getPoints() const { return m_points; }
    
    // Setters
    void setName(const QString &name);
    void setXMin(double xmin) { m_xmin = xmin; }
    void setXMax(double xmax) { m_xmax = xmax; }
    
    /**
     * @brief Adiciona um intervalo à camada (apenas para IntervalTier)
     * @param xmin Tempo inicial do intervalo
     * @param xmax Tempo final do intervalo
     * @param text Texto do intervalo
     * @return Ponteiro para o intervalo criado
     */
    std::shared_ptr<AnnotationInterval> addInterval(double xmin, double xmax, const QString &text = "");
    
    /**
     * @brief Adiciona um ponto à camada (apenas para PointTier)
     * @param time Tempo do ponto
     * @param text Texto do ponto
     * @return Ponteiro para o ponto criado
     */
    std::shared_ptr<AnnotationPoint> addPoint(double time, const QString &text = "");
    
    /**
     * @brief Remove um intervalo da camada
     * @param index Índice do intervalo
     * @return true se removido com sucesso
     */
    bool removeInterval(int index);
    
    /**
     * @brief Remove um ponto da camada
     * @param index Índice do ponto
     * @return true se removido com sucesso
     */
    bool removePoint(int index);
    
    /**
     * @brief Encontra o intervalo em uma posição temporal
     * @param time Tempo em segundos
     * @return Índice do intervalo (-1 se não encontrado)
     */
    int findIntervalAtTime(double time) const;
    
    /**
     * @brief Encontra o ponto mais próximo de uma posição temporal
     * @param time Tempo em segundos
     * @param tolerance Tolerância em segundos
     * @return Índice do ponto (-1 se não encontrado)
     */
    int findPointNearTime(double time, double tolerance = 0.05) const;
    
    /**
     * @brief Limpa todos os intervalos ou pontos da camada
     */
    void clear();
    
    /**
     * @brief Verifica se a camada está vazia
     */
    bool isEmpty() const;

signals:
    /**
     * @brief Sinal emitido quando um intervalo é adicionado
     */
    void intervalAdded(int index);
    
    /**
     * @brief Sinal emitido quando um intervalo é removido
     */
    void intervalRemoved(int index);
    
    /**
     * @brief Sinal emitido quando um intervalo é modificado
     */
    void intervalModified(int index);
    
    /**
     * @brief Sinal emitido quando um ponto é adicionado
     */
    void pointAdded(int index);
    
    /**
     * @brief Sinal emitido quando um ponto é removido
     */
    void pointRemoved(int index);
    
    /**
     * @brief Sinal emitido quando um ponto é modificado
     */
    void pointModified(int index);
    
    /**
     * @brief Sinal emitido quando o nome da camada é alterado
     */
    void nameChanged(const QString &newName);

private:
    QString m_name;
    TierType m_type;
    double m_xmin;
    double m_xmax;
    
    QVector<std::shared_ptr<AnnotationInterval>> m_intervals;
    QVector<std::shared_ptr<AnnotationPoint>> m_points;
    
    void sortIntervals();
    void sortPoints();
};

#endif // ANNOTATIONTIER_H

