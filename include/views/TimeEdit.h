#ifndef TIMEEDIT_H
#define TIMEEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>

/**
 * @brief Widget para edição de tempo no formato HH:MM:SS.mmm
 * Inspirado no Audacity
 */
class TimeEdit : public QWidget
{
    Q_OBJECT

public:
    explicit TimeEdit(QWidget *parent = nullptr);
    
    /**
     * @brief Define o tempo em segundos
     */
    void setTime(double seconds);
    
    /**
     * @brief Obtém o tempo em segundos
     */
    double time() const;
    
    /**
     * @brief Define se é somente leitura
     */
    void setReadOnly(bool readOnly);

signals:
    /**
     * @brief Emitido quando o tempo é alterado pelo usuário
     */
    void timeChanged(double seconds);

private slots:
    void onTextEdited();
    void onEditingFinished();

private:
    void setupUI();
    void updateDisplay();
    QString formatTime(double seconds) const;
    double parseTime(const QString &text) const;

private:
    QLineEdit *m_hoursEdit;
    QLineEdit *m_minutesEdit;
    QLineEdit *m_secondsEdit;
    QLineEdit *m_millisecondsEdit;
    
    double m_time;  // tempo em segundos
    bool m_updating;
};

#endif // TIMEEDIT_H
