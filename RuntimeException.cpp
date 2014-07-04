

#include	"RuntimeException.hh"


RuntimeException::RuntimeException(std::string const &msg, std::string const &where)
  : std::runtime_error::runtime_error(msg)
{
  this->msg = msg;
  this->where_msg = where;
}

RuntimeException::~RuntimeException() throw()
{
}

const char	*RuntimeException::what() const throw()
{
  return (this->msg.c_str());
}

const char	*RuntimeException::where() const throw()
{
  return (this->where_msg.c_str());
}
