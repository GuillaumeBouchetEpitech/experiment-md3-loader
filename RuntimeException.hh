

#ifndef	EXCEPTIONBOMBERMAN_HH
#define	EXCEPTIONBOMBERMAN_HH


#include	<stdexcept>
#include	<iostream>
#include	<exception>
#include	<string>


class	RuntimeException : public std::runtime_error
{
private:

  std::string	msg;
  std::string	where_msg;

public:

  RuntimeException(std::string const &msg, std::string const &where);
  virtual ~RuntimeException() throw();
  virtual const char * what() const throw();
  virtual const char * where() const throw();
};


#endif
