#ifndef __UO_SET_UTILS_HPP__
#define __UO_SET_UTILS_HPP__
#include <memory>
#include <unordered_set>
#include <string>
#include "utils.h"


namespace vas_lib
{
    using vas_lib::utils::str_p;
    struct StringPtrHash {

        std::size_t operator()(const str_p& ptr) const {
            if (!ptr) {
                return 0;
            }
            return std::hash<std::string>{}(*ptr);
        }
    };

    struct StringPtrEqual {
        bool operator()(const str_p& lhs, const str_p& rhs) const {
            if (!lhs || !rhs) {
                return false;
            }

            return *lhs == *rhs;
        }
    };

    using uo_set = std::unordered_set<std::shared_ptr<std::string>, StringPtrHash, StringPtrEqual>;
    using uo_set_it = std::unordered_set<std::shared_ptr<std::string>, StringPtrHash, StringPtrEqual>::iterator;
};




#endif /* __UO_SET_UTILS_HPP__ */

