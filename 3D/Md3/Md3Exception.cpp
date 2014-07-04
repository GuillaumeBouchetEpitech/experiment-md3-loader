

#include "Md3Exception.hh"


Md3Exception::Md3Exception (const std::string &error)
  : std::runtime_error (error)
{
  this->msg = error;
  // this->where_msg = where;
}

Md3Exception::Md3Exception (const std::string &error, const std::string &name)
  : std::runtime_error (error)//, _which (name)
{
  this->msg = error;
  this->where_msg = name;
}

Md3Exception::~Md3Exception () throw ()
{
}

// const char*	Md3Exception::which () const throw ()
// {
//   return _which.c_str ();
// }

const char*	Md3Exception::what() const throw()
{
  return (this->msg.c_str());
}

const char*	Md3Exception::where() const throw()
{
  return (this->where_msg.c_str());
}
