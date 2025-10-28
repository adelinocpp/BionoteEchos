#include "views/MainWindow.h"
#include "utils/Logger.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Inicializar sistema de logs
    Logger::instance();
    LOG("=== AudioAnnotator Iniciado ===");
    
    // Configurações da aplicação
    QApplication::setApplicationName("AudioAnnotator");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("AudioAnnotator");
    QApplication::setOrganizationDomain("audioannotator.org");
    
    // Definir locale para português brasileiro
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
    
    // Nota: As traduções estão embutidas no código usando tr()
    // Para suporte futuro a múltiplos idiomas, poderá ser adicionado
    // sistema de tradução com QTranslator e arquivos .qm
    
    // Criar e mostrar janela principal
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}

