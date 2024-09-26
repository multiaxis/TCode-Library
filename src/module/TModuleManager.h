// TCode-Interface-Manager-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "TModuleBase.h"
#include <Arduino.h>
#include <vector>

namespace TCode::Module {
    /**
     * @brief Manages the lifecycle and updates of registered TCode modules.
     *
     * The `TCodeModuleManager` class is responsible for managing a collection of modules
     * that derive from `TCodeModuleBase`. It provides methods to register new modules
     * and to update all registered modules within the context of the system. The manager
     * ensures that each module is updated during the system's update cycle.
     */
    class TCodeModuleManager {
    private:
        std::vector<TCodeModuleBase *> registeredInterfaces; ///< A collection of pointers to registered TCode modules.

    public:
        /**
         * @brief Registers a module interface with the manager.
         *
         * This method adds a `TCodeModuleBase` module to the list of managed interfaces.
         * The registered module will then be included in the update cycle when the manager's
         * `update` method is called.
         *
         * @param interface A pointer to the `TCodeModuleBase` interface to be registered.
         */
        void registerInterface(TCodeModuleBase *interface);

        /**
         * @brief Updates all registered modules.
         *
         * This method iterates through all registered modules and calls their `update`
         * method, passing the provided `TCodeContext`. Each module can then execute
         * its specific update logic using the context.
         *
         * @param context The `TCodeContext` object that provides access to necessary system components.
         */
        void update(TCodeContext &context);
    };
};