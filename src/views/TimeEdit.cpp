#include "views/TimeEdit.h"
#include <QRegularExpressionValidator>
#include <QFont>
#include <cmath>

TimeEdit::TimeEdit(QWidget *parent)
    : QWidget(parent)
    , m_time(0.0)
    , m_updating(false)
{
    setupUI();
}

void TimeEdit::setupUI()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(2);
    
    // Fonte monoespaçada para melhor alinhamento
    QFont monoFont("Monospace");
    monoFont.setStyleHint(QFont::TypeWriter);
    
    // Horas
    m_hoursEdit = new QLineEdit(this);
    m_hoursEdit->setMaxLength(2);
    m_hoursEdit->setFixedWidth(30);
    m_hoursEdit->setAlignment(Qt::AlignCenter);
    m_hoursEdit->setFont(monoFont);
    m_hoursEdit->setText("00");
    connect(m_hoursEdit, &QLineEdit::textEdited, this, &TimeEdit::onTextEdited);
    connect(m_hoursEdit, &QLineEdit::editingFinished, this, &TimeEdit::onEditingFinished);
    
    QLabel *colon1 = new QLabel(":", this);
    colon1->setFont(monoFont);
    
    // Minutos
    m_minutesEdit = new QLineEdit(this);
    m_minutesEdit->setMaxLength(2);
    m_minutesEdit->setFixedWidth(30);
    m_minutesEdit->setAlignment(Qt::AlignCenter);
    m_minutesEdit->setFont(monoFont);
    m_minutesEdit->setText("00");
    connect(m_minutesEdit, &QLineEdit::textEdited, this, &TimeEdit::onTextEdited);
    connect(m_minutesEdit, &QLineEdit::editingFinished, this, &TimeEdit::onEditingFinished);
    
    QLabel *colon2 = new QLabel(":", this);
    colon2->setFont(monoFont);
    
    // Segundos
    m_secondsEdit = new QLineEdit(this);
    m_secondsEdit->setMaxLength(2);
    m_secondsEdit->setFixedWidth(30);
    m_secondsEdit->setAlignment(Qt::AlignCenter);
    m_secondsEdit->setFont(monoFont);
    m_secondsEdit->setText("00");
    connect(m_secondsEdit, &QLineEdit::textEdited, this, &TimeEdit::onTextEdited);
    connect(m_secondsEdit, &QLineEdit::editingFinished, this, &TimeEdit::onEditingFinished);
    
    QLabel *dot = new QLabel(".", this);
    dot->setFont(monoFont);
    
    // Milissegundos
    m_millisecondsEdit = new QLineEdit(this);
    m_millisecondsEdit->setMaxLength(3);
    m_millisecondsEdit->setFixedWidth(40);
    m_millisecondsEdit->setAlignment(Qt::AlignCenter);
    m_millisecondsEdit->setFont(monoFont);
    m_millisecondsEdit->setText("000");
    connect(m_millisecondsEdit, &QLineEdit::textEdited, this, &TimeEdit::onTextEdited);
    connect(m_millisecondsEdit, &QLineEdit::editingFinished, this, &TimeEdit::onEditingFinished);
    
    layout->addWidget(m_hoursEdit);
    layout->addWidget(colon1);
    layout->addWidget(m_minutesEdit);
    layout->addWidget(colon2);
    layout->addWidget(m_secondsEdit);
    layout->addWidget(dot);
    layout->addWidget(m_millisecondsEdit);
    layout->addStretch();
    
    setLayout(layout);
}

void TimeEdit::setTime(double seconds)
{
    if (m_updating) return;
    
    m_time = std::max(0.0, seconds);
    updateDisplay();
}

double TimeEdit::time() const
{
    return m_time;
}

void TimeEdit::setReadOnly(bool readOnly)
{
    m_hoursEdit->setReadOnly(readOnly);
    m_minutesEdit->setReadOnly(readOnly);
    m_secondsEdit->setReadOnly(readOnly);
    m_millisecondsEdit->setReadOnly(readOnly);
}

void TimeEdit::onTextEdited()
{
    // Não emitir sinal enquanto está editando
}

void TimeEdit::onEditingFinished()
{
    m_updating = true;
    
    // Calcular tempo total
    int hours = m_hoursEdit->text().toInt();
    int minutes = m_minutesEdit->text().toInt();
    int seconds = m_secondsEdit->text().toInt();
    int milliseconds = m_millisecondsEdit->text().toInt();
    
    // Validar e corrigir valores
    if (minutes >= 60) {
        hours += minutes / 60;
        minutes = minutes % 60;
    }
    if (seconds >= 60) {
        minutes += seconds / 60;
        seconds = seconds % 60;
    }
    if (milliseconds >= 1000) {
        seconds += milliseconds / 1000;
        milliseconds = milliseconds % 1000;
    }
    
    m_time = hours * 3600.0 + minutes * 60.0 + seconds + milliseconds / 1000.0;
    
    updateDisplay();
    emit timeChanged(m_time);
    
    m_updating = false;
}

void TimeEdit::updateDisplay()
{
    int totalMs = static_cast<int>(m_time * 1000);
    
    int hours = totalMs / 3600000;
    totalMs %= 3600000;
    
    int minutes = totalMs / 60000;
    totalMs %= 60000;
    
    int seconds = totalMs / 1000;
    int milliseconds = totalMs % 1000;
    
    m_hoursEdit->setText(QString("%1").arg(hours, 2, 10, QChar('0')));
    m_minutesEdit->setText(QString("%1").arg(minutes, 2, 10, QChar('0')));
    m_secondsEdit->setText(QString("%1").arg(seconds, 2, 10, QChar('0')));
    m_millisecondsEdit->setText(QString("%1").arg(milliseconds, 3, 10, QChar('0')));
}

QString TimeEdit::formatTime(double seconds) const
{
    int totalMs = static_cast<int>(seconds * 1000);
    
    int hours = totalMs / 3600000;
    totalMs %= 3600000;
    
    int minutes = totalMs / 60000;
    totalMs %= 60000;
    
    int secs = totalMs / 1000;
    int ms = totalMs % 1000;
    
    return QString("%1:%2:%3.%4")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(secs, 2, 10, QChar('0'))
        .arg(ms, 3, 10, QChar('0'));
}

double TimeEdit::parseTime(const QString &text) const
{
    // Parse formato HH:MM:SS.mmm
    QStringList parts = text.split(':');
    if (parts.size() != 3) return 0.0;
    
    int hours = parts[0].toInt();
    int minutes = parts[1].toInt();
    
    QStringList secParts = parts[2].split('.');
    int seconds = secParts[0].toInt();
    int milliseconds = secParts.size() > 1 ? secParts[1].toInt() : 0;
    
    return hours * 3600.0 + minutes * 60.0 + seconds + milliseconds / 1000.0;
}
