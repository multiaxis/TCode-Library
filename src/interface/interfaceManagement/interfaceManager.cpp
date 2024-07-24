#include "interfaceManager.h"

namespace TCode
{
    TCodeInterfaceManager::TCodeInterfaceManager()
    {
    }

    TCodeInterfaceManager::~TCodeInterfaceManager()
    {
    }

    void TCodeInterfaceManager::registerInterface(TInterfaceBase *interface)
    {
        registeredInterfaces.push_back(interface);
    }

    /*      
    void TCodeInterfaceManager::update(TCodeContext &context)
    {
        for(size_t i = 0; i < registeredInterfaces.size(); i++)
        {
            registeredInterfaces[i]->update(context);
        }
    }
    */

}