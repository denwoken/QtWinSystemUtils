#include "SysFileType.h"
#include <QDebug>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>


#include <QWidget>
#pragma comment(lib, "Dwmapi.lib")
#include <dwmapi.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
    #define DWMWA_USE_IMMERSIVE_DARK_MODE 20  // или 19 в старых версиях
#endif

void setDarkTitleBar(QWidget *w, bool darkMode_)
{
    HWND hwnd = (HWND)w->winId();
    BOOL darkMode = (darkMode_)?TRUE:FALSE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));
}
bool isWindowsDarkModeEnabled()
{
    QSettings settings(R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)", QSettings::NativeFormat);
    return settings.value("AppsUseLightTheme", 1).toInt() == 0;
}
bool isApplicationRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    HANDLE token = nullptr;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        TOKEN_ELEVATION elevation;
        DWORD size;
        if (GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &size)) {
            isAdmin = elevation.TokenIsElevated;
        }
        CloseHandle(token);
    }
    return isAdmin;
}



SysFileType::SysFileType(const QString &fileTypeName)
{
    if(!fileTypeName.size()) qCritical() << "Невозможно создать тип файла без имени";
    m_fileTypeName = fileTypeName;
}

bool SysFileType::exist()
{
    // Описание типа файла
    QSettings regedit("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    QStringList list = regedit.childGroups();  // относительно долгая операция
    return list.contains(m_fileTypeName);
}

bool SysFileType::existExtension(const QString &ext)
{
    if(ext.size() <= 1) return 0;
    QString extension(ext);
    if(extension.at(0) != '.') extension = "." + extension;
    QSettings regedit("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    QStringList list = regedit.childGroups();  // относительно долгая операция
    return list.contains(extension);
}

QString SysFileType::getDescription()
{
    QString desc;
    QSettings regedit("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    regedit.beginGroup(m_fileTypeName);
    desc = regedit.value(".").toString();
    regedit.endGroup();
    return desc;
}

void SysFileType::setDescription(const QString &desc)
{
    QSettings regedit("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    regedit.beginGroup(m_fileTypeName);
    regedit.setValue(".", desc);
    regedit.endGroup();
    regedit.sync();
}

QString SysFileType::getExexPath()
{
    QString path;
    QString regPath = "HKEY_CLASSES_ROOT\\" + m_fileTypeName + "\\shell\\open\\command";
    QSettings regedit(regPath, QSettings::NativeFormat);
    path = regedit.value(".").toString();

    QSettings reg("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    reg.beginGroup(m_fileTypeName);
    if(!countParametersInGroup(reg))  //если в групппе типа файла нет записей удаляем её
        reg.remove("");
    reg.endGroup();
    reg.sync();

    return path;
}

void SysFileType::setExecPath(const QString &execPath, const QString &execArgs)
{
    QString execPath_(execPath);
    for(int i = 0; i < execPath_.size(); i++)
        if(execPath_[i] == '/') execPath_[i] = '\\';

    QString regPath = "HKEY_CLASSES_ROOT\\" + m_fileTypeName + "\\shell\\open\\command";
    QSettings regedit(regPath, QSettings::NativeFormat);
    regedit.setValue(".", execPath_ + " " + execArgs);
    regedit.sync();
}

QString SysFileType::getIconPath()
{
    QString path;
    QSettings regedit("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    regedit.beginGroup(m_fileTypeName);
    regedit.beginGroup("DefaultIcon");
    path = regedit.value(".").toString();
    regedit.endGroup();
    regedit.endGroup();
    return path;
}

void SysFileType::setIconPath(const QString &absIconPath)
{
    QString absIconPath_(absIconPath);
    for(int i = 0; i < absIconPath_.size(); i++)
        if(absIconPath_[i] == '/') absIconPath_[i] = '\\';
    QSettings regedit("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    regedit.beginGroup(m_fileTypeName);
    regedit.beginGroup("DefaultIcon");
    regedit.setValue(".", absIconPath_);
    regedit.endGroup();
    regedit.endGroup();
    regedit.sync();
}

void SysFileType::addFileExtension(const QString &ext)
{
    if(ext.size() <= 1) return;
    QString extension(ext);
    if(extension.at(0) != '.') extension = "." + extension;
    QSettings regedit("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    regedit.beginGroup(extension);
    regedit.setValue(".", m_fileTypeName);
    regedit.endGroup();
    regedit.sync();
}

void SysFileType::removeFileExtension(const QString &ext)
{
    if(ext.size() <= 1) return;
    QString extension(ext);
    if(extension.at(0) != '.') extension = "." + extension;
    qDebug() << "try remove :" << extension;
    QSettings regedit("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    regedit.beginGroup(extension);
    regedit.remove("");
    regedit.sync();
    regedit.endGroup();

    QSettings regedit1("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts",
                       QSettings::NativeFormat);
    regedit1.beginGroup(extension);
    regedit1.remove("");
    regedit1.endGroup();
    regedit.sync();
}

void SysFileType::remove()
{
    QSettings regedit("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    // removeGroupRecursively(regedit, m_fileTypeName);
    regedit.beginGroup(m_fileTypeName);
    regedit.remove("");
    regedit.endGroup();
    regedit.sync();
}

void SysFileType::sysCacheUpdate()
{
    // Обновляем ассоциации файлов в системе  (обновление иконок и т.п.)
   // SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
}

int SysFileType::countParametersInGroup(QSettings &group)
{
    int num = 0;
    num += group.childKeys().size();
    for(int i = 0; i < group.childGroups().size(); i++)
    {
        group.beginGroup(group.childGroups()[i]);
        num += countParametersInGroup(group);
        group.endGroup();
    }
    return num;
}
