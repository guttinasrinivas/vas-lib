#ifndef __CLI_ARGS__
#define __CLI_ARGS__

#include <memory>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>


struct CLI_Arg;

using CLI_Arg_p = std::shared_ptr<CLI_Arg>;

using str_p = std::shared_ptr<std::string>;
using int_p = std::shared_ptr<int>;
using double_p = std::shared_ptr<double>;
using bool_p = std::shared_ptr<bool>;


struct CLI_Arg
{
public:
    enum arg_type
    {
        ArgString = 1,
        ArgInt = 2,
        ArgDouble = 3,
        ArgFlag = 4,
        ArgLast = 5
    };


public:
    CLI_Arg(str_p name, str_p val, const std::string& helpstr=""): name_(name), str_val_(val)
    {
        help_str_ = std::make_shared<std::string>(helpstr);
        type_ = ArgString;
        is_flag_ = false;
    }

    CLI_Arg(str_p name, int_p val, const std::string& helpstr=""): name_(name), int_val_(val)
    {
        help_str_ = std::make_shared<std::string>(helpstr);
        type_ = ArgInt;
        is_flag_ = false;
    }

    CLI_Arg(str_p name, double_p val, const std::string& helpstr=""): name_(name), double_val_(val)
    {
        help_str_ = std::make_shared<std::string>(helpstr);
        type_ = ArgDouble;
        is_flag_ = false;
    }

    CLI_Arg(str_p name, bool_p val, const std::string& helpstr=""): name_(name), flag_(val)
    {
        help_str_ = std::make_shared<std::string>(helpstr);
        type_ = ArgFlag;
        is_flag_ = true;
    }

    str_p to_str_p(const char *inname)
    {
        return std::make_shared<std::string>(inname);
    }

    str_p name(void)
    {
        return name_;
    }

    str_p help_str(void)
    {
        std::stringstream ss;
        ss << *name_ << ": " << *help_str_ << std::endl;
        return std::make_shared<std::string>(ss.str());
    }

    auto is_flag(void)
    {
        return is_flag_;
    }

    void parse(std::string& inval)
    {
        switch (type_)
        {
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



struct CLI_Args
{
public:
    CLI_Args(int ac, char **av): ac_(ac), av_(av) {}
    virtual ~CLI_Args() {}

    std::vector<str_p>& get_args(void) { return n_args_; }

    auto add_flag(const char* name)
    {
        auto arg = std::make_shared<bool>(false);
        add_arg(name, arg);
        return arg;
    }

    auto add_int(const char* name)
    {
        auto arg = std::make_shared<int>(0);
        add_arg(name, arg);
        return arg;
    }

    auto add_double(const char* name)
    {
        auto arg = std::make_shared<double>(0);
        add_arg(name, arg);
        return arg;
    }

    auto add_str(const char* name)
    {
        auto arg = std::make_shared<std::string>("");
        add_arg(name, arg);
        return arg;
    }

    void add_arg(const char* name, str_p target)
    {
        auto name_p = std::make_shared<std::string>(name);
        auto opt = std::make_shared<CLI_Arg>(name_p, target);
        args_[*name_p] = opt;
    }

    void add_arg(const char* name, int_p target)
    {
        auto name_p = std::make_shared<std::string>(name);
        auto opt = std::make_shared<CLI_Arg>(name_p, target);
        args_[*name_p] = opt;
    }

    void add_arg(const char* name, double_p target)
    {
        auto name_p = std::make_shared<std::string>(name);
        auto opt = std::make_shared<CLI_Arg>(name_p, target);
        args_[*name_p] = opt;
    }

    void add_arg(const char* name, bool_p target)
    {
        auto name_p = std::make_shared<std::string>(name);
        auto opt = std::make_shared<CLI_Arg>(name_p, target);
        args_[*name_p] = opt;
    }


    str_p help(void)
    {
        auto ostr = std::string("");

        for (const auto& cliarg: args_)
        {
            ostr.append(*cliarg.second->help_str());
        }

        return std::make_shared<std::string>(ostr);
    }

    str_p exp_args(void)
    {
        auto expargs = std::string("");

        for (const auto& cliarg: args_)
        {
            auto arg = *cliarg.second->name() + " ";
            expargs.append(arg);
        }

        return std::make_shared<std::string>(expargs);
    }

    void parse_args(void)
    {
        /* Use std::string for UTF-8 compatibility - fingers crossed */
        for (idx_ = 1; idx_ < ac_; idx_++)
        {
            /* TODO
             * ====
             *  1. Parse UTF-8
             *  2. Parse sequence flags : -aluh */
            if (std::string(av_[idx_]).find('-') != 0)
            {
                n_args_.push_back(std::make_shared<std::string>(av_[idx_]));
                continue;
            }

            if (std::string(av_[idx_]).find("--") == 0)
            {
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
        auto inarg = std::string(av_[idx_]);
        auto val_ofst = inarg.find('=');
        auto max_ofst = inarg.size() - 1;
        auto val_len = inarg.size();

        if ((max_ofst <= val_ofst) ||
            (val_ofst <= 0))
        {
            throw std::runtime_error("Invalid value for argument: " + inarg);
        }

        auto parser = args_[inarg.substr(0, val_ofst).c_str()];
        if (!parser)
        {
            throw std::runtime_error("Invalid arguments: " + std::string(av_[idx_]));
        }

        if (parser->is_flag())
        {
            parser->parse();
            return;
        }
        
        val_len -= val_ofst;
        auto inval = inarg.substr(val_ofst+1, inarg.size());
        parser->parse(inval);
    }

    void parse_short_args(void)
    {
        auto inarg = std::string(av_[idx_]);
        if (inarg.find_last_of('-') != 0)
        {
            throw std::runtime_error("Invalid arguments: " + std::string(av_[idx_]));
        }

        for (size_t ii = 1; ii < inarg.size(); ii++)
        {
            parse_short_arg(inarg, ii);
        }

        return;
    }

    void parse_short_arg(std::string& inarg, size_t ii)
    {
        auto flag = std::string("-");
        flag += inarg.at(ii);
        auto parser = args_[flag];
        if (!parser)
        {
            throw std::runtime_error("Invalid arguments: " + std::string(flag));
        }

        if (parser->is_flag())
        {
            parser->parse();
            return;
        }

        /* Additional validation to ensure input is not a flag,
         * but the current CLI arg is at the end of current string */
        if (ii != (inarg.size() - 1))
        {
            throw std::runtime_error("Values must succeed arguments: " +
                                std::string(flag));
        }

        auto inval = std::string(av_[idx_+1]);
        if (inval.find('-') == 0)
        {
            throw std::runtime_error("Invalid value for argument: " + flag);
        }

        idx_++;
        parser->parse(inval);

        return;
    }

protected:
    int ac_;
    char **av_;
    std::map<std::string, CLI_Arg_p> args_;
    std::vector<str_p> n_args_;
    int idx_;
};


#endif /* __CLI_ARGS__ */
