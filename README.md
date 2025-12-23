
# QtWinSystemUtils

**QtWinSystemUtils** is a lightweight C++/Qt utility library for Windows that provides simple access to common system-level application features, such as file type registration and single-instance application control.

    ⚠️ **Windows only**
    The library relies on Windows system APIs and registry access.
    Tested on Windows 10 22H2 (build 19045.3803).

## Features
- Register and remove custom file types
- Add and remove file extensions
- Associate file types with:
    - executable path
    - icon
    - description
- Update Windows system file association cache
- Prevent running multiple instances of the same application

## Requirements
- Windows 10
- Qt 5 or Qt 6
- C++17 or newer
- Administrator privileges may be required for file type registration


## Classes Overview

### SysFileType
Provides an API for registering, querying, and removing file types and file extensions in Windows.

#### Main capabilities
- Create or access a file type by name
- Bind file extensions to a file type
- Set icon, executable path, and description
- Check existence of file types and extensions
- Remove file types or extensions
- Force Windows to refresh file association cache

### SingleApplication
Provides a mechanism to ensure that only one instance of an application is running at the same time.    
Uses shared memory identified by a unique application name.


## Usage Examples

### Register a File Type and Extension
``` C++
{
    // привязывем обект к названию типа файла (создание типа файла происходит при вызове долбнейших функций)
    SysFileType myfiletype("mytiffFile");
    // создается расширение файла привязанное к типу файла "mytiffFile"                              
    myfiletype.addFileExtension(".mytiff");
    // установление пути до иконки                            
    myfiletype.setIconPath("C:/Qt/myType.ico");
    // установление пути до исполняемого файла                        
    myfiletype.setExecPath(QApplication::applicationFilePath());
    // устанавливаем описание для типа файла       
    myfiletype.setDescription("mytiffFile for Tolmi Application");     
    // обновляем кэш
    SysFileType::sysCacheUpdate();                                     
}
```

### Read and Remove File Type and Extension
```C++
{
    // привязывем обект к названию типа файла
    SysFileType myfiletype("mytiffFile");                                
    // проверяем существует ли тип файла в системе
    bool exist = myfiletype.exist();                                     
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

### Bind an Extension to an Existing File Type
```C++
{
    // привязывем обект к существующему названию типа файла
    SysFileType myfiletype("txtfile");      
    // создаем расширение .my который ассоциируеся с txtfile, котрый открывается обычным блокнотом
    myfiletype.addFileExtension(".my");     
    // Now ".my" files open the same way as ".txt"

    // для удаления расширения .my
    SysFileType::removeFileExtension(".my");
    // без myfiletype.remove() чтобы .txt файлы открывались корректно
}
```

### Single Application Instance Check
```C++
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

### Behavior Example
```C++
// При первом запуске
SingleApplication sApp("uniqueTolmiApp");
qDebug() << sApp.isAlreadyRunning()             // false
qDebug() << sApp.isAlreadyRunning()             // false

SingleApplication app("uniqueTolmiApp");
qDebug() << app.isAlreadyRunning()             // true
qDebug() << app.isAlreadyRunning()             // true
```


