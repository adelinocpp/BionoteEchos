#ifndef ANNOTATIONLAYERWIDGET_H
#define ANNOTATIONLAYERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <memory>

class AnnotationTier;
class AnnotationInterval;
class AnnotationPoint;

/**
 * @brief Widget para visualização e edição de camadas de anotação
 * 
 * Funcionalidades:
 * - Visualização de até 2 camadas simultaneamente
 * - Interatividade: adicionar, mover, editar intervalos e pontos
 * - Sincronização com visualização de áudio
 * - Edição de texto inline
 */
class AnnotationLayerWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construtor
     * @param parent Widget pai
     */
    explicit AnnotationLayerWidget(QWidget *parent = nullptr);

    /**
     * @brief Destrutor
     */
    ~AnnotationLayerWidget();
    
    /**
     * @brief Define a primeira camada de anotação
     * @param tier Ponteiro para a camada
     */
    void setTier1(std::shared_ptr<AnnotationTier> tier);
    
    /**
     * @brief Define a segunda camada de anotação
     * @param tier Ponteiro para a camada
     */
    void setTier2(std::shared_ptr<AnnotationTier> tier);
    
    /**
     * @brief Define a janela de tempo visível (sincronizada com AudioVisualizationWidget)
     * @param startTime Tempo inicial
     * @param endTime Tempo final
     */
    void setVisibleTimeRange(double startTime, double endTime);
    
    /**
     * @brief Limpa todas as camadas
     */
    void clearTiers();
    
    /**
     * @brief Obtém a camada atualmente selecionada
     * @return Ponteiro para a camada (nullptr se nenhuma)
     */
    std::shared_ptr<AnnotationTier> getSelectedTier() const;
    
    /**
     * @brief Define a camada selecionada
     * @param tierIndex Índice da camada (0 ou 1)
     */
    void setSelectedTier(int tierIndex);

signals:
    /**
     * @brief Sinal emitido quando um intervalo é adicionado
     */
    void intervalAdded(std::shared_ptr<AnnotationTier> tier, int index);
    
    /**
     * @brief Sinal emitido quando um intervalo é modificado
     */
    void intervalModified(std::shared_ptr<AnnotationTier> tier, int index);
    
    /**
     * @brief Sinal emitido quando um intervalo é removido
     */
    void intervalRemoved(std::shared_ptr<AnnotationTier> tier, int index);
    
    /**
     * @brief Sinal emitido quando um ponto é adicionado
     */
    void pointAdded(std::shared_ptr<AnnotationTier> tier, int index);
    
    /**
     * @brief Sinal emitido quando um ponto é modificado
     */
    void pointModified(std::shared_ptr<AnnotationTier> tier, int index);
    
    /**
     * @brief Sinal emitido quando um ponto é removido
     */
    void pointRemoved(std::shared_ptr<AnnotationTier> tier, int index);
    
    /**
     * @brief Sinal emitido quando uma seleção de intervalo é feita
     */
    void intervalSelected(std::shared_ptr<AnnotationTier> tier, int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawTier(QPainter &painter, std::shared_ptr<AnnotationTier> tier, int yOffset, int height);
    void drawIntervalTier(QPainter &painter, std::shared_ptr<AnnotationTier> tier, int yOffset, int height);
    void drawPointTier(QPainter &painter, std::shared_ptr<AnnotationTier> tier, int yOffset, int height);
    
    double pixelToTime(int pixel) const;
    int timeToPixel(double time) const;
    
    int findIntervalAtPosition(std::shared_ptr<AnnotationTier> tier, int x, int y);
    int findPointAtPosition(std::shared_ptr<AnnotationTier> tier, int x, int y);
    int findBoundaryAtPosition(std::shared_ptr<AnnotationTier> tier, int x, int y);
    
    void startEditingInterval(std::shared_ptr<AnnotationTier> tier, int index);
    void startEditingPoint(std::shared_ptr<AnnotationTier> tier, int index);
    
    void addIntervalAtPosition(std::shared_ptr<AnnotationTier> tier, double time);
    void addPointAtPosition(std::shared_ptr<AnnotationTier> tier, double time);

private:
    std::shared_ptr<AnnotationTier> m_tier1;
    std::shared_ptr<AnnotationTier> m_tier2;
    
    double m_viewStartTime;
    double m_viewEndTime;
    
    int m_selectedTierIndex;
    int m_selectedIntervalIndex;
    int m_selectedPointIndex;
    
    bool m_isDraggingBoundary;
    int m_draggingTierIndex;
    int m_draggingIntervalIndex;
    bool m_draggingLeftBoundary;
    
    bool m_isEditing;
    QString m_editingText;
    
    int m_tierHeight;
    int m_tierSpacing;
};

#endif // ANNOTATIONLAYERWIDGET_H

