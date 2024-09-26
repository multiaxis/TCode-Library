// TCode-External-Interface-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once


namespace TCode {
    class TCodeContext;
}

namespace TCode::Module {
    /**
     * @brief Abstract base class for TCode modules.
     *
     * The `TCodeModuleBase` class serves as a base for all modules within the TCode framework.
     * It defines a pure virtual method `update`, which must be implemented by any derived module.
     * Each module is responsible for performing its specific update logic during the system's
     * update cycle, using the provided `TCodeContext` to interact with other components.
     */
    class TCodeModuleBase {
    public:
        /**
         * @brief Pure virtual method for updating the module's state.
         *
         * This method is called during the system's update cycle and must be implemented by
         * any module that inherits from `TCodeModuleBase`. It is responsible for executing
         * the module's specific functionality, utilizing the provided `TCodeContext` to access
         * necessary managers, settings, or other resources.
         *
         * @param context The `TCodeContext` object that provides references to important system components.
         */
        virtual void update(TCodeContext &context) = 0;
    };
};
