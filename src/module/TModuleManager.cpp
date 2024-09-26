#include "TModuleManager.h"

namespace TCode::Module {
    void TCodeModuleManager::registerInterface(TCodeModuleBase *interface) {
        if (interface != nullptr)
            registeredInterfaces.push_back(interface);
    }

    void TCodeModuleManager::update(TCodeContext &context) {
        for (size_t i = 0; i < registeredInterfaces.size(); i++) {
            registeredInterfaces[i]->update(context);
        }
    }

}