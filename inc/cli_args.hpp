#ifndef __CLI_ARGS__
#define __CLI_ARGS__

#include <memory>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>

#define TO_StrSP(instr) std::make_shared<std::string>(instr)


namespace ArgParser
{
    /* Forward declaration for Shared Pointer utils */
    struct CLI_Arg;

    using CLI_Arg_p = std::shared_ptr<CLI_Arg>;

    using str_p = std::shared_ptr<std::string>;
    using int_p = std::shared_ptr<int>;
    using double_p = std::shared_ptr<double>;
    using bool_p = std::shared_ptr<bool>;

    struct CLI_Arg {
        public:
            enum arg_type {
                ArgString = 1,
                ArgInt = 2,
                ArgDouble = 3,
                ArgFlag = 4,
                ArgLast = 5
            };


        public:
            CLI_Arg(str_p name, str_p val, const std::string& helpstr = ""): name_(name),
                str_val_(val)
            {
                help_str_ = TO_StrSP(helpstr);
                type_ = ArgString;
                is_flag_ = false;
            }

            CLI_Arg(str_p name, int_p val, const std::string& helpstr = ""): name_(name),
                int_val_(val)
            {
                help_str_ = TO_StrSP(helpstr);
                type_ = ArgInt;
                is_flag_ = false;
            }

            CLI_Arg(str_p name, double_p val, const std::string& helpstr = ""): name_(name),
                double_val_(val)
            {
                help_str_ = TO_StrSP(helpstr);
                type_ = ArgDouble;
                is_flag_ = false;
            }

            CLI_Arg(str_p name, bool_p val, const std::string& helpstr = ""): name_(name),
                flag_(val)
            {
                help_str_ = TO_StrSP(helpstr);
                type_ = ArgFlag;
                is_flag_ = true;
            }

            str_p to_str_p(const char* inname)
            {
                return TO_StrSP(inname);
            }

            str_p name(void)
            {
                return name_;
            }

            str_p help_str(void)
            {
                std::stringstream ss;
                ss << *name_;
                int pad = (name_->size() >= 20) ? 0 : 20 - name_->size();
                for (int ii = 0; ii < pad; ii++) {
                    ss << ' ';
                }

                switch (type_) {
                    case ArgString:
                        ss << "STRING";
                        break;

                    case ArgInt:
                        ss << "INT   ";
                        break;

                    case ArgDouble:
                        ss << "DOUBLE";
                        break;

                    case ArgFlag:
                        ss << "FLAG  ";
                        break;

                    default:
                        ss << "??????";
                        break;
                }

                ss << ": ";

                ss << *help_str_ << std::endl;
                return TO_StrSP(ss.str());
            }

            auto is_flag(void)
            {
                return is_flag_;
            }

            void parse(std::string& inval)
            {
                switch (type_) {
                    case ArgString:
                        parse_string(inval);
                        break;

                    case ArgInt:
                        parse_int(inval);
                        break;

                    case ArgDouble:
                        parse_double(inval);
                        break;

                    default:
                        throw std::runtime_error("Invalid argument type");
                }
            }

            void parse(void)
            {
                *flag_ = true;
            }

            void parse_string(std::string &inval)
            {
                *str_val_ = inval;
            }

            void parse_int(std::string &inval)
            {
                *int_val_ = std::stoi(inval);
            }

            void parse_double(std::string &inval)
            {
                *double_val_ = std::stod(inval);
            }


        protected:
            str_p name_;
            str_p str_val_;
            int_p int_val_;
            double_p double_val_;
            bool_p flag_;
            bool is_flag_;
            arg_type type_;
            str_p help_str_;
    };


    struct CLI_Args {
        public:
            CLI_Args(int ac, char** av): ac_(ac), av_(av)
            {
            }

            virtual ~CLI_Args() {}

            str_p get_args(void)
            {
                std::stringstream ss;
                for (auto arg : n_args_) {
                    ss << *arg << ", ";
                }

                return TO_StrSP(ss.str());
            }

            std::vector<str_p>& positional_args(void)
            {
                return n_args_;
            }

            auto add_str(const char* sh_opt = "", const char* lg_opt = "",
                         const char* descr = "", const char* defval = "")
            {
                auto arg = TO_StrSP(defval);
                add_arg(arg, sh_opt, lg_opt, descr);
                return arg;
            }

            auto add_int(const char* sh_opt = "", const char* lg_opt = "",
                         const char* descr = "", int defval = 0)
            {
                auto arg = std::make_shared<int>(defval);
                add_arg(arg, sh_opt, lg_opt, descr);
                return arg;
            }

            auto add_double(const char* sh_opt = "", const char* lg_opt = "",
                            const char* descr = "", double defval = 0.0)
            {
                auto arg = std::make_shared<double>(defval);
                add_arg(arg, sh_opt, lg_opt, descr);
                return arg;
            }

            auto add_flag(const char* sh_opt = "", const char* lg_opt = "",
                          const char* descr = "", bool defval = false)
            {
                auto arg = std::make_shared<bool>(defval);
                add_arg(arg, sh_opt, lg_opt, descr);
                return arg;
            }

            template <typename T>
            void add_arg(T target, const char* sh_opt = "", const char* lg_opt = "",
                         const char* descr = "")
            {
                if (std::string(sh_opt) != "") {
                    auto sh_opt_p = TO_StrSP(sh_opt);
                    auto opt = std::make_shared<CLI_Arg>(sh_opt_p, target, descr);
                    args_[*sh_opt_p] = opt;
                }

                if (std::string(lg_opt) != "") {
                    auto lg_opt_p = TO_StrSP(lg_opt);
                    auto opt = std::make_shared<CLI_Arg>(lg_opt_p, target, descr);
                    args_[*lg_opt_p] = opt;
                }
            }

            str_p help(void)
            {
                auto ostr = std::string("");

                for (const auto& cliarg : args_) {
                    ostr.append(*cliarg.second->help_str());
                }

                return TO_StrSP(ostr);
            }

            str_p exp_args(void)
            {
                auto expargs = std::string("");

                for (const auto& cliarg : args_) {
                    auto arg = *cliarg.second->name() + " ";
                    expargs.append(arg);
                }

                return TO_StrSP(expargs);
            }

            void parse_args(void)
            {
                /* Use std::string for UTF-8 compatibility - fingers crossed */
                for (idx_ = 1; idx_ < ac_; idx_++) {
                    /* TODO
                     * ====
                     *  1. Parse UTF-8
                     *  2. Parse sequence flags : -aluh */
                    if (std::string(av_[idx_]).find('-') != 0) {
                        n_args_.push_back(TO_StrSP(av_[idx_]));
                        continue;
                    }

                    if (std::string(av_[idx_]).find("--") == 0) {
                        parse_long_arg();
                        continue;
                    }

                    parse_short_args();
                }

                return;
            }

            void parse_long_arg(void)
            {
                /* This is value arg, use value */
                auto inarg = TO_StrSP(av_[idx_]);
                auto val_ofst = inarg->find('=');
                auto val_len = inarg->size();
                CLI_Arg_p parser;

                if (std::string::npos == val_ofst) {
                    /* If the '=' is not found in the string, it is probably a flag.
                     * Try anyways before giving up. */
                    parser = args_[*inarg];
                } else {
                    auto key = TO_StrSP(inarg->substr(0, val_ofst));
                    parser = args_[key->c_str()];
                }

                if (!parser) {
                    throw std::runtime_error("Invalid arguments: " + *inarg);
                }

                if (parser->is_flag()) {
                    parser->parse();
                    return;
                }

                val_len -= val_ofst;
                auto inval = inarg->substr(val_ofst + 1, val_len);
                parser->parse(inval);
            }

            void parse_short_args(void)
            {
                auto inarg = std::string(av_[idx_]);
                if (inarg.find_last_of('-') != 0) {
                    throw std::runtime_error("Invalid arguments: " + std::string(av_[idx_]));
                }

                for (size_t ii = 1; ii < inarg.size(); ii++) {
                    parse_short_arg(inarg, ii);
                }

                return;
            }

            void parse_short_arg(std::string& inarg, size_t ii)
            {
                auto flag = std::string("-");
                flag += inarg.at(ii);
                auto parser = args_[flag];
                if (!parser) {
                    throw std::runtime_error("Invalid arguments: " + std::string(flag));
                }

                if (parser->is_flag()) {
                    parser->parse();
                    return;
                }

                /* Additional validation to ensure input is not a flag,
                 * but the current CLI arg is at the end of current string */
                if (ii != (inarg.size() - 1)) {
                    throw std::runtime_error("Values must succeed arguments: " +
                                             std::string(flag));
                }

                auto inval = std::string(av_[idx_ + 1]);
                if (inval.find('-') == 0) {
                    throw std::runtime_error("Invalid value for argument: " + flag);
                }

                idx_++;
                parser->parse(inval);

                return;
            }

        protected:
            int ac_;
            char** av_;
            std::map<std::string, CLI_Arg_p> args_;
            std::vector<str_p> n_args_;
            int idx_;
    };
};


#endif /* __CLI_ARGS__ */
