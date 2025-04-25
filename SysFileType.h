#ifndef SYSFILETYPE_H
#define SYSFILETYPE_H

#include <QSettings>

/*!
 * \brief setDarkTitleBar делает у виджета темный titleBar
 */
void setDarkTitleBar(QWidget* w, bool darkMode_ = true);
/*!
 * \brief isWindowsDarkModeEnabled проверяет включена ли темная тема в винде
 */
bool isWindowsDarkModeEnabled();

bool isApplicationRunningAsAdmin();

/*!
 * \brief The SysFileType class позволяет зарегистрировать тип файла в системе, добавить расширение и ассоциировать его к типу файла.
 * При необходимости позволит удалить тип файла и расширение.
 *  Данный класс не будет работать на Linux (работа проверена на <Win 10 22H2 build: 19045.3803>, <> )
 *

    ///------------------ Примеры использования ------------------///

    // регистрация типа файла и расширения
    {
        SysFileType myfiletype("mytiffFile");                                       // привязывем обект к названию типа файла (создание типа файла происходит при вызове долбнейших функций)
        myfiletype.addFileExtension(".mytiff");                                     // создается расширение файла привязанное к типу файла "mytiffFile"
        myfiletype.setIconPath("C:/Qt/myType.ico");                                 // установление пути до иконки
        myfiletype.setExecPath(QApplication::applicationFilePath());                // установление пути до исполняемого файла
        myfiletype.setDescription("mytiffFile for Tolmi Application");              // устанавливаем описание для типа файла
        SysFileType::sysCacheUpdate();                                              // обновляем кэш
    }

    // считывание и удаление типа файла и расширения
    {
        SysFileType myfiletype("mytiffFile");                                       // привязывем обект к названию типа файла

        bool exist = myfiletype.exist();                                            // проверяем существует ли тип файла в системе
        bool existExt = myfiletype.existExtension(".mytiff");
        qDebug() << "filetype already exist:" << exist;
        qDebug() << "extension already exist:" << existExt;
        qDebug() << "filetype exec path:" << myfiletype.getExexPath();
        qDebug() << "filetype icon path:" << myfiletype.getIconPath();
        qDebug() << "filetype Description:" << myfiletype.getDescription();

        myfiletype.remove()                                                         // полностью удаляем тип файла
        SysFileType::removeFileExtension(".mytiff");                                // удаление расширения файла
        SysFileType::sysCacheUpdate();                                              // обновляем кэш

    }

    // cоздаем расширение файла и привязываем его к уже существующему типу файла, а затем удаляем расширение
    {
        SysFileType myfiletype("txtfile");                                          // привязывем обект к существующему названию типа файла
        myfiletype.addFileExtension(".my");                                         // создаем расширение .my который ассоциируеся с txtfile, котрый открывается обычным блокнотом
        // теперь .my открывается также как .txt

        // для удаления расширения .my
        SysFileType::removeFileExtension(".my");
        // без myfiletype.remove() чтобы .txt файлы открывались корректно
    }


 *
 */
class SysFileType
{
public:
    SysFileType(const QString &fileTypeName);;

    /*!
     * \brief exist проверяет существует ли данный тип файла.
     * Может долго выполняться, если в реестре очень много записей.
     * \return
     */
    bool exist();
    /*!
     * \brief existExtension проверяет существует ли расширение файла
     * \param ext
     * \return true если существует
     */
    static bool existExtension(const QString &ext);


    /*!
     * \brief getExexPath достает из реестра описание типа файла
     * \return
     */
    QString getDescription();
    /*!
     * \brief setDescription устанавливает описание типа файла.
     * \param desc
     */
    void setDescription(const QString &desc);


    /*!
     * \brief getExexPath достает из реестра путь до исполняемого файла, весте с аргументам разделенными пробелами
     * \return
     */
    QString getExexPath();
    /*!
     * \brief setExecPath устанавливает асолютный путь исполняемого файла и аргументы для запуска программы.
     * \param execPath
     * \param execArgs аргументы при запуске .exe (по дефолту< "%1" >, для отправки пути к открываемому файлу в приложение )
     */
    void setExecPath(const QString &execPath, const QString &execArgs = "\"%1\"");


    /*!
     * \brief getIconPath достает из реестра путь к иконке данного типа файла.
     * \return
     */
    QString getIconPath();
    /*!
     * \brief setIconPath устанавливает путь к иконке для данного типа файла.
     * \param absIconPath
     */
    void setIconPath(const QString &absIconPath);


    /*!
     * \brief addFileExtension добавляет расширение файла в реестр и привязывает к текущему типу файла
     * \param ext
     */
    void addFileExtension(const QString &ext);
    /*!
     * \brief removeExtension удаляет расширение файла, зарегистрированный тип файла не удаляется.
     * \param ext
     */
    static void removeFileExtension(const QString &ext);


    /*!
     * \brief remove полностью удаляет тип файла из реестра.
     * (Не рекомендуется, если типом файла пользуется другое приложение, оно перестанет нормально его воспринимать)
     *  Расширения файлов ассоциируемые с данным типом файлов остаются,
     *   для их удаления нужно вызвать removeExtension()
     */
    void remove();

    /*!
     * \brief sysCacheUpdate Отправляет сигнал Windows, что необходимо обновить кеш ассоциаций файлов и иконок.
     * Вызывать не обязательно, но после изменения иконки визуальные изменения вступят в силу только
     * после перезагрузки explorer.exe или вызова данной функции.
     * Рекомендуеся вызывать после всех изменений и регистраий файлов.
     */
    static void sysCacheUpdate();

private:
    QString m_fileTypeName;
    /*!
     * \brief countParametersInGroup считает колисество настроек входящих в группу (рекурсивно).
     *  Позвляет понять пустая ли группа для удаления.
     * \param group
     * \return
     */
    int countParametersInGroup(QSettings &group);
};

#endif // SYSFILETYPE_H
