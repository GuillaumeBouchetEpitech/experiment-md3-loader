
#ifndef MD3EXCEPTION_HH
#define MD3EXCEPTION_HH

#include <stdexcept>
#include <string>

class Md3Exception : public std::runtime_error
{
public:
  Md3Exception (const std::string &error);
  Md3Exception (const std::string &error, const std::string &name);
  virtual ~Md3Exception () throw ();

public:
  // virtual const char *which () const throw ();
  virtual const char * what() const throw();
  virtual const char * where() const throw();

private:
  // std::string _which;
  std::string   msg;
  std::string   where_msg;


};

#endif
