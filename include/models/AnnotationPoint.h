#ifndef ANNOTATIONPOINT_H
#define ANNOTATIONPOINT_H

#include <QString>
#include <QObject>

/**
 * @brief Representa um ponto de anotação em uma PointTier (TextTier)
 * 
 * Um ponto possui:
 * - Tempo (number/time)
 * - Texto da anotação (mark)
 */
class AnnotationPoint : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Construtor
     * @param time Tempo do ponto (segundos)
     * @param text Texto da anotação (mark)
     * @param parent Objeto pai Qt
     */
    explicit AnnotationPoint(double time = 0.0,
                            const QString &text = "",
                            QObject *parent = nullptr);

    /**
     * @brief Destrutor
     */
    ~AnnotationPoint();

    // Getters
    double getTime() const { return m_time; }
    QString getText() const { return m_text; }
    
    // Setters
    void setTime(double time);
    void setText(const QString &text);
    
    /**
     * @brief Calcula a distância temporal até outro tempo
     * @param time Tempo em segundos
     * @return Distância absoluta em segundos
     */
    double distanceTo(double time) const;

signals:
    /**
     * @brief Sinal emitido quando o tempo do ponto muda
     */
    void timeChanged(double newTime);
    
    /**
     * @brief Sinal emitido quando o texto muda
     */
    void textChanged(const QString &newText);

private:
    double m_time;
    QString m_text;
};

#endif // ANNOTATIONPOINT_H

