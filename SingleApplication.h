#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H
#include <QString>
#include <QSharedMemory>
#include <QSystemSemaphore>


/*!
 * \brief The SingleApplication class используется для проверки запущен ли уже экземпляр приложения.
 * При вызове конструктора проверяется существует ли элемент разделяемой памяти,
 * именованный названием приложения, если нет, то приложение открыто в первый раз и происходит создание разделяемой памяти.
 *
 *

    ///------------------ Пример использования ------------------///

    SingleApplication sApp("uniqueTolmiApp");
    if(sApp.isAlreadyRunning())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.warning(nullptr, "Warning","Приложение уже запущено.\n"
                                           "Разрешено запускать только один экземпляр приложения.");
        return 1;
    }

    ///------------------ Пример использования ------------------///
    // При первом запуске
    SingleApplication sApp("uniqueTolmiApp");
    qDebug() << sApp.isAlreadyRunning()             // выводит false
    qDebug() << sApp.isAlreadyRunning()             // выводит false

    SingleApplication app("uniqueTolmiApp");
    qDebug() << app.isAlreadyRunning()             // выводит true
    qDebug() << app.isAlreadyRunning()             // выводит true
 *
 */
class SingleApplication
{
public:
    /*!
     * \brief SingleApplication  Конструктор класса принимает уникальное имя приложения,
     * проверяет зарегистрированно ли оно в системе, если нет, то регисстрирует.
     * \param appName
     */
    SingleApplication(QString appName);
    /*!
     * \brief isAlreadyRunning Возвращает true если уникальное имя уже было зарегистрированно( значит запущен второй экземпляр приложения )
     * \return
     */
    bool isAlreadyRunning();
private:
    QSystemSemaphore m_semaphore;
    QSharedMemory m_sharedMemory;
    bool isRunning = true;
};

#endif // SINGLEAPPLICATION_H
