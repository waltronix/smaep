#pragma once

#include <exception>
#include <string>

namespace smaep {
class smaep_exceptions : public std::exception {
protected:
  std::string m_message;

public:
  smaep_exceptions(std::string message) : m_message(std::move(message)) {}
};

class data_exception : public smaep_exceptions {
public:
  using smaep_exceptions::smaep_exceptions;
};

class parsing_excpetion : public smaep_exceptions {
public:
  using smaep_exceptions::smaep_exceptions;
};
} // namespace smaep