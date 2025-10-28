#ifndef ANNOTATIONINTERVAL_H
#define ANNOTATIONINTERVAL_H

#include <QString>
#include <QObject>

/**
 * @brief Representa um intervalo de anotação em uma IntervalTier
 * 
 * Um intervalo possui:
 * - Tempo inicial (xmin)
 * - Tempo final (xmax)
 * - Texto da anotação
 */
class AnnotationInterval : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Construtor
     * @param xmin Tempo inicial do intervalo (segundos)
     * @param xmax Tempo final do intervalo (segundos)
     * @param text Texto da anotação
     * @param parent Objeto pai Qt
     */
    explicit AnnotationInterval(double xmin = 0.0,
                                double xmax = 0.0,
                                const QString &text = "",
                                QObject *parent = nullptr);

    /**
     * @brief Destrutor
     */
    ~AnnotationInterval();

    // Getters
    double getXMin() const { return m_xmin; }
    double getXMax() const { return m_xmax; }
    QString getText() const { return m_text; }
    double getDuration() const { return m_xmax - m_xmin; }
    
    // Setters
    void setXMin(double xmin);
    void setXMax(double xmax);
    void setText(const QString &text);
    
    /**
     * @brief Define os limites do intervalo
     * @param xmin Tempo inicial
     * @param xmax Tempo final
     */
    void setBounds(double xmin, double xmax);
    
    /**
     * @brief Verifica se um tempo está dentro do intervalo
     * @param time Tempo em segundos
     * @return true se o tempo está dentro do intervalo
     */
    bool contains(double time) const;
    
    /**
     * @brief Verifica se o intervalo é válido (xmax > xmin)
     */
    bool isValid() const { return m_xmax > m_xmin; }

signals:
    /**
     * @brief Sinal emitido quando os limites do intervalo mudam
     */
    void boundsChanged();
    
    /**
     * @brief Sinal emitido quando o texto muda
     */
    void textChanged(const QString &newText);

private:
    double m_xmin;
    double m_xmax;
    QString m_text;
};

#endif // ANNOTATIONINTERVAL_H

