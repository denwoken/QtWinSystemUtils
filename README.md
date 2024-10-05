# myServiceCode



## class SysFileType

The SysFileType class позволяет зарегистрировать тип файла в системе, добавить расширение и ассоциировать его к типу файла.
* При необходимости позволит удалить тип файла и расширение.
* Данный класс не будет работать на Linux (работа проверена на <Win 10 22H2 build: 19045.3803>, <> )

### Примеры использования
Регистрация типа файла и расширения
```
{
    SysFileType myfiletype("mytiffFile");                              // привязывем обект к названию типа файла (создание типа файла происходит при вызове долбнейших функций)
    myfiletype.addFileExtension(".mytiff");                            // создается расширение файла привязанное к типу файла "mytiffFile"
    myfiletype.setIconPath("C:/Qt/myType.ico");                        // установление пути до иконки
    myfiletype.setExecPath(QApplication::applicationFilePath());       // установление пути до исполняемого файла
    myfiletype.setDescription("mytiffFile for Tolmi Application");     // устанавливаем описание для типа файла
    SysFileType::sysCacheUpdate();                                     // обновляем кэш
}
```
Считывание и удаление типа файла и расширения
```
{
    SysFileType myfiletype("mytiffFile");                                // привязывем обект к названию типа файла

    bool exist = myfiletype.exist();                                     // проверяем существует ли тип файла в системе
    bool existExt = myfiletype.existExtension(".mytiff");
    qDebug() << "filetype already exist:" << exist;
    qDebug() << "extension already exist:" << existExt;
    qDebug() << "filetype exec path:" << myfiletype.getExexPath();
    qDebug() << "filetype icon path:" << myfiletype.getIconPath();
    qDebug() << "filetype Description:" << myfiletype.getDescription();

    myfiletype.remove()                                                  // полностью удаляем тип файла
    SysFileType::removeFileExtension(".mytiff");                         // удаление расширения файла
    SysFileType::sysCacheUpdate();                                       // обновляем кэш

}
```

Создаем расширение файла и привязываем его к уже существующему типу файла, а затем удаляем расширение
```
{
    SysFileType myfiletype("txtfile");      // привязывем обект к существующему названию типа файла
    myfiletype.addFileExtension(".my");     // создаем расширение .my который ассоциируеся с txtfile, котрый открывается обычным блокнотом
    // теперь .my открывается также как .txt

    // для удаления расширения .my
    SysFileType::removeFileExtension(".my");
    // без myfiletype.remove() чтобы .txt файлы открывались корректно
}
```



## class SingleApplication
The SingleApplication class используется для проверки запущен ли уже экземпляр приложения.

При вызове конструктора проверяется существует ли элемент разделяемой памяти, именованный названием приложения, если нет, то приложение открыто в первый раз и происходит создание разделяемой памяти.

### Примеры использования

```
SingleApplication sApp("uniqueApplication");
if(sApp.isAlreadyRunning())
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.warning(nullptr, "Warning","Приложение уже запущено.\n"
                                      "Разрешено запускать только один экземпляр приложения.");
    return 1;
}
```
```
// При первом запуске
SingleApplication sApp("uniqueTolmiApp");
qDebug() << sApp.isAlreadyRunning()             // выводит false
qDebug() << sApp.isAlreadyRunning()             // выводит false

SingleApplication app("uniqueTolmiApp");
qDebug() << app.isAlreadyRunning()             // выводит true
qDebug() << app.isAlreadyRunning()             // выводит true
```
