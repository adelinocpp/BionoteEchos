#include "views/AboutDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QFont>
#include <QDesktopServices>
#include <QUrl>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("Sobre BionoteEchos");
    setModal(true);
    setFixedSize(600, 500);
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Logo do Bionote
    QLabel *bionoteLogoLabel = new QLabel(this);
    QPixmap bionoteLogo(":/icons/Bionote_logo_black.png");
    if (!bionoteLogo.isNull()) {
        bionoteLogoLabel->setPixmap(bionoteLogo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        bionoteLogoLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(bionoteLogoLabel);
    }
    
    // Nome do aplicativo
    QLabel *appNameLabel = new QLabel("BionoteEchos", this);
    QFont appNameFont;
    appNameFont.setPointSize(24);
    appNameFont.setBold(true);
    appNameLabel->setFont(appNameFont);
    appNameLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(appNameLabel);
    
    // Versão
    QLabel *versionLabel = new QLabel("Versão 1.0.0", this);
    QFont versionFont;
    versionFont.setPointSize(12);
    versionLabel->setFont(versionFont);
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setStyleSheet("color: #666;");
    mainLayout->addWidget(versionLabel);
    
    // Descrição
    QLabel *descLabel = new QLabel(
        "Aplicação para anotação de arquivos de áudio com visualização\n"
        "de forma de onda, espectrograma e análise acústica.",
        this
    );
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("color: #444;");
    mainLayout->addWidget(descLabel);
    
    mainLayout->addSpacing(10);
    
    // Desenvolvedor
    QLabel *devLabel = new QLabel("<b>Desenvolvedor:</b>", this);
    mainLayout->addWidget(devLabel);
    
    QLabel *devNameLabel = new QLabel("Dr. Adelino Pinheiro Silva", this);
    devNameLabel->setStyleSheet("margin-left: 20px;");
    mainLayout->addWidget(devNameLabel);
    
    // Email 1
    QLabel *email1Label = new QLabel(
        "<a href='mailto:adelinocpp@gmail.com'>adelinocpp@gmail.com</a>",
        this
    );
    email1Label->setOpenExternalLinks(true);
    email1Label->setStyleSheet("margin-left: 20px;");
    mainLayout->addWidget(email1Label);
    
    // Email 2
    QLabel *email2Label = new QLabel(
        "<a href='mailto:adelinocpp@yahoo.com'>adelinocpp@yahoo.com</a>",
        this
    );
    email2Label->setOpenExternalLinks(true);
    email2Label->setStyleSheet("margin-left: 20px;");
    mainLayout->addWidget(email2Label);
    
    mainLayout->addSpacing(10);
    
    // Empresa
    QLabel *companyLabel = new QLabel("<b>Empresa:</b>", this);
    mainLayout->addWidget(companyLabel);
    
    // Logos da Log Nature
    QHBoxLayout *logosLayout = new QHBoxLayout();
    logosLayout->setSpacing(20);
    
    QLabel *logoLabel1 = new QLabel(this);
    QPixmap logo1(":/icons/log_nature_logo_00.png");
    if (!logo1.isNull()) {
        logoLabel1->setPixmap(logo1.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    logosLayout->addWidget(logoLabel1);
    
    QLabel *logoLabel2 = new QLabel(this);
    QPixmap logo2(":/icons/log_yt_02.png");
    if (!logo2.isNull()) {
        logoLabel2->setPixmap(logo2.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    logosLayout->addWidget(logoLabel2);
    
    logosLayout->addStretch();
    mainLayout->addLayout(logosLayout);
    
    mainLayout->addSpacing(10);
    
    // GitHub
    QLabel *githubLabel = new QLabel(
        "<b>GitHub:</b> <a href='https://github.com/adelinocpp/BionoteEchos'>https://github.com/adelinocpp/BionoteEchos</a>",
        this
    );
    githubLabel->setOpenExternalLinks(true);
    githubLabel->setWordWrap(true);
    mainLayout->addWidget(githubLabel);
    
    mainLayout->addStretch();
    
    // Botão Fechar
    QPushButton *closeButton = new QPushButton("Fechar", this);
    closeButton->setFixedWidth(100);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}
