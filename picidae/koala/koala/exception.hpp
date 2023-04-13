#ifndef PICIDAE_KOALA_KOALA_EXCEPTION_HPP__
#define PICIDAE_KOALA_KOALA_EXCEPTION_HPP__

#include <exception>
#include <sstream>

#include "picidae/koala/koala/boxed_number.hpp"
#include "picidae/koala/koala/koala_header.hpp"
#include "picidae/koala/koala/ast_node.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(koala::exception)
class bad_any_cast : public std::bad_cast {
 public:
  const char *what() const noexcept override { return "[Koala] bad any cast"; }
};

struct load_module_error : std::runtime_error {
  explicit load_module_error(const std::string &t_reason)
      : std::runtime_error(t_reason) {}

  load_module_error(const std::string &t_name,
                    const std::vector<load_module_error> &t_errors)
      : std::runtime_error(format_error(t_name, t_errors)) {}

  load_module_error(const load_module_error &) = default;
  ~load_module_error() noexcept override = default;

  static std::string format_error(
      const std::string &t_name,
      const std::vector<load_module_error> &t_errors) {
    std::stringstream ss;
    ss << "Error loading module '" << t_name << "'\n"
       << "  The following locations were searched:\n";

    for (const auto &err : t_errors) {
      ss << "    " << err.what() << "\n";
    }

    return ss.str();
  }
};

/// Errors generated during parsing or evaluation
struct eval_error : std::runtime_error {
  std::string reason;
  FilePosition start_position;
  std::string filename;
  std::string detail;
  std::vector<AST_Node_Trace> call_stack;

  eval_error(const std::string &t_why, const FilePosition &t_where,
             const std::string &t_fname,
             const std::vector<Boxed_Value> &t_parameters,
             const std::vector<quanscript::Const_Proxy_Function> &t_functions,
             bool t_dot_notation,
             const quanscript::detail::Dispatch_Engine &t_ss) noexcept
      : std::runtime_error(format(t_why, t_where, t_fname, t_parameters,
                                  t_dot_notation, t_ss)),
        reason(t_why),
        start_position(t_where),
        filename(t_fname),
        detail(format_detail(t_functions, t_dot_notation, t_ss)) {}

  eval_error(const std::string &t_why,
             const std::vector<Boxed_Value> &t_parameters,
             const std::vector<quanscript::Const_Proxy_Function> &t_functions,
             bool t_dot_notation,
             const quanscript::detail::Dispatch_Engine &t_ss) noexcept
      : std::runtime_error(format(t_why, t_parameters, t_dot_notation, t_ss)),
        reason(t_why),
        detail(format_detail(t_functions, t_dot_notation, t_ss)) {}

  eval_error(const std::string &t_why, const File_Position &t_where,
             const std::string &t_fname) noexcept
      : std::runtime_error(format(t_why, t_where, t_fname)),
        reason(t_why),
        start_position(t_where),
        filename(t_fname) {}

  explicit eval_error(const std::string &t_why) noexcept
      : std::runtime_error("Error: \"" + t_why + "\" "), reason(t_why) {}

  eval_error(const eval_error &) = default;

  std::string pretty_print() const {
    std::ostringstream ss;

    ss << what();
    if (!call_stack.empty()) {
      ss << "during evaluation at (" << fname(call_stack[0]) << " "
         << startpos(call_stack[0]) << ")\n";
      ss << '\n' << detail << '\n';
      ss << "  " << fname(call_stack[0]) << " (" << startpos(call_stack[0])
         << ") '" << pretty(call_stack[0]) << "'";
      for (size_t j = 1; j < call_stack.size(); ++j) {
        if (id(call_stack[j]) != quanscript::AST_Node_Type::Block &&
            id(call_stack[j]) != quanscript::AST_Node_Type::File) {
          ss << '\n';
          ss << "  from " << fname(call_stack[j]) << " ("
             << startpos(call_stack[j]) << ") '" << pretty(call_stack[j])
             << "'";
        }
      }
    }
    ss << '\n';
    return ss.str();
  }

  ~eval_error() noexcept override = default;

 private:
  template <typename T>
  static AST_Node_Type id(const T &t) noexcept {
    return t.identifier;
  }

  template <typename T>
  static std::string pretty(const T &t) {
    return t.pretty_print();
  }

  template <typename T>
  static const std::string &fname(const T &t) noexcept {
    return t.filename();
  }

  template <typename T>
  static std::string startpos(const T &t) {
    std::ostringstream oss;
    oss << t.start().line << ", " << t.start().column;
    return oss.str();
  }

  static std::string format_why(const std::string &t_why) {
    return "Error: \"" + t_why + "\"";
  }

  static std::string format_types(
      const Const_Proxy_Function &t_func, bool t_dot_notation,
      const quanscript::detail::Dispatch_Engine &t_ss) {
    assert(t_func);
    int arity = t_func->get_arity();
    std::vector<Type_Info> types = t_func->get_param_types();

    std::string retval;
    if (arity == -1) {
      retval = "(...)";
      if (t_dot_notation) {
        retval = "(Object)." + retval;
      }
    } else if (types.size() <= 1) {
      retval = "()";
    } else {
      std::stringstream ss;
      ss << "(";

      std::string paramstr;

      for (size_t index = 1; index != types.size(); ++index) {
        paramstr += (types[index].is_const() ? "const " : "");
        paramstr += t_ss.get_type_name(types[index]);

        if (index == 1 && t_dot_notation) {
          paramstr += ").(";
          if (types.size() == 2) {
            paramstr += ", ";
          }
        } else {
          paramstr += ", ";
        }
      }

      ss << paramstr.substr(0, paramstr.size() - 2);

      ss << ")";
      retval = ss.str();
    }

    std::shared_ptr<const dispatch::Dynamic_Proxy_Function> dynfun =
        std::dynamic_pointer_cast<const dispatch::Dynamic_Proxy_Function>(
            t_func);

    if (dynfun && dynfun->has_parse_tree()) {
      Proxy_Function f = dynfun->get_guard();

      if (f) {
        auto dynfunguard =
            std::dynamic_pointer_cast<const dispatch::Dynamic_Proxy_Function>(
                f);
        if (dynfunguard && dynfunguard->has_parse_tree()) {
          retval += " : " + format_guard(dynfunguard->get_parse_tree());
        }
      }

      retval +=
          "\n          Defined at " + format_location(dynfun->get_parse_tree());
    }

    return retval;
  }

  template <typename T>
  static std::string format_guard(const T &t) {
    return t.pretty_print();
  }

  template <typename T>
  static std::string format_location(const T &t) {
    std::ostringstream oss;
    oss << "(" << t.filename() << " " << t.start().line << ", "
        << t.start().column << ")";
    return oss.str();
  }

  static std::string format_detail(
      const std::vector<quanscript::Const_Proxy_Function> &t_functions,
      bool t_dot_notation, const quanscript::detail::Dispatch_Engine &t_ss) {
    std::stringstream ss;
    if (t_functions.size() == 1) {
      assert(t_functions[0]);
      ss << "  Expected: " << format_types(t_functions[0], t_dot_notation, t_ss)
         << '\n';
    } else {
      ss << "  " << t_functions.size() << " overloads available:\n";

      for (const auto &t_function : t_functions) {
        ss << "      " << format_types((t_function), t_dot_notation, t_ss)
           << '\n';
      }
    }

    return ss.str();
  }

  static std::string format_parameters(
      const std::vector<Boxed_Value> &t_parameters, bool t_dot_notation,
      const quanscript::detail::Dispatch_Engine &t_ss) {
    std::stringstream ss;
    ss << "(";

    if (!t_parameters.empty()) {
      std::string paramstr;

      for (auto itr = t_parameters.begin(); itr != t_parameters.end(); ++itr) {
        paramstr += (itr->is_const() ? "const " : "");
        paramstr += t_ss.type_name(*itr);

        if (itr == t_parameters.begin() && t_dot_notation) {
          paramstr += ").(";
          if (t_parameters.size() == 1) {
            paramstr += ", ";
          }
        } else {
          paramstr += ", ";
        }
      }

      ss << paramstr.substr(0, paramstr.size() - 2);
    }
    ss << ")";

    return ss.str();
  }

  static std::string format_filename(const std::string &t_fname) {
    std::stringstream ss;

    if (t_fname != "__EVAL__") {
      ss << "in '" << t_fname << "' ";
    } else {
      ss << "during evaluation ";
    }

    return ss.str();
  }

  static std::string format_location(const File_Position &t_where) {
    std::stringstream ss;
    ss << "at (" << t_where.line << ", " << t_where.column << ")";
    return ss.str();
  }

  static std::string format(const std::string &t_why,
                            const File_Position &t_where,
                            const std::string &t_fname,
                            const std::vector<Boxed_Value> &t_parameters,
                            bool t_dot_notation,
                            const quanscript::detail::Dispatch_Engine &t_ss) {
    std::stringstream ss;

    ss << format_why(t_why);
    ss << " ";

    ss << "With parameters: "
       << format_parameters(t_parameters, t_dot_notation, t_ss);
    ss << " ";

    ss << format_filename(t_fname);
    ss << " ";

    ss << format_location(t_where);

    return ss.str();
  }

  static std::string format(const std::string &t_why,
                            const std::vector<Boxed_Value> &t_parameters,
                            bool t_dot_notation,
                            const quanscript::detail::Dispatch_Engine &t_ss) {
    std::stringstream ss;

    ss << format_why(t_why);
    ss << " ";

    ss << "With parameters: "
       << format_parameters(t_parameters, t_dot_notation, t_ss);
    ss << " ";

    return ss.str();
  }

  static std::string format(const std::string &t_why,
                            const File_Position &t_where,
                            const std::string &t_fname) {
    std::stringstream ss;

    ss << format_why(t_why);
    ss << " ";

    ss << format_filename(t_fname);
    ss << " ";

    ss << format_location(t_where);

    return ss.str();
  }
};

/// Errors generated when loading a file
struct file_not_found_error : std::runtime_error {
  explicit file_not_found_error(const std::string &t_filename)
      : std::runtime_error("File Not Found: " + t_filename),
        filename(t_filename) {}

  file_not_found_error(const file_not_found_error &) = default;
  ~file_not_found_error() noexcept override = default;

  std::string filename;
};

PICIDAE_NAMESPACE_END(koala::exception)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_KOALA_KOALA_EXCEPTION_HPP__
