#include "SingleApplication.h"



SingleApplication::SingleApplication(QString appName) : m_semaphore(appName + "Sem", 1)
{
    m_sharedMemory.setKey(appName + "Mem");
    isRunning = true;
    m_semaphore.acquire();

    {// для совместимости с  UNIX системами.
        QSharedMemory shmemUnixFix(m_sharedMemory.key());
        shmemUnixFix.attach();
    }
    if(!m_sharedMemory.attach())
    {
        m_sharedMemory.create(1);
        isRunning = false;
    }
    m_semaphore.release();
}

bool SingleApplication::isAlreadyRunning()
{

    return isRunning;
}
