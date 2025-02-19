#pragma once

#include "custom_type.hpp"
#include "globals.hpp"
#include <functional>
#include <map>
#include <string>


namespace nnwcli
{
    class DLL_PUBLIC TypeRegistry
    {
        std::map<const std::string,
            std::function<AbstractCustomType(std::string)>&> m_custom_types;

    public:
        void register_custom_type(const std::string name, std::function<AbstractCustomType(std::string)>& constructor);
        void unregister_custom_type(const std::string name);

        std::function<AbstractCustomType(std::string)>& get_type_constructor(const std::string name);
    };
}
