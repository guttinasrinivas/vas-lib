#ifndef __VAS_LIB_UTILS_H__
#define __VAS_LIB_UTILS_H__
#ifdef __cplusplus
#include <memory>
#include <string>
#include <vector>

namespace vas_lib {
    namespace utils {
        typedef std::shared_ptr<std::string> str_p;
        typedef std::shared_ptr<int> int_p;

        typedef std::shared_ptr<std::vector<str_p>> str_vec_p;

        static str_p to_str_p(std::string& instr)
        {
            return std::make_shared<std::string>(instr);
        }

        static str_p to_str_p(const char* instr)
        {
            return std::make_shared<std::string>(instr);
        }

    }
};

#endif /* _cplusplus */

#endif /* __VAS_LIB_UTILS_H__ */

