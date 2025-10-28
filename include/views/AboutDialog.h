#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

/**
 * @brief Diálogo "Sobre" do BionoteEchos
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private:
    void setupUI();
};

#endif // ABOUTDIALOG_H
