#include "cdialogmanager.h"

CDialogManager *CDialogManager::m_instance = NULL;

CDialogManager::~CDialogManager()
{
}

CDialogManager *CDialogManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CDialogManager;
    }
    return m_instance;
}

CDialogManager::CDialogManager(QObject *parent)
    : CBaseDialogManager(parent)
{
}
