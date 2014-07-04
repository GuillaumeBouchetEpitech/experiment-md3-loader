

#include	<iostream>
#include	"Game.hh"
#include	"RuntimeException.hh"


int		main(int argc, char** argv, char** env)
{
  static_cast<void>(argc);
  static_cast<void>(argv);

  bool			display = false;
  const std::string	cmp("DISPLAY=:0.0");
  const std::string	cmp2("DISPLAY=:0");

  for (int i = 0; env[i]; ++i)
    if (cmp == env[i] || cmp2 == env[i])
      {
  	display = true;
  	break;
      }

  if (!display)
    std::cerr << "Bad environment (DISPLAY not present or valid)" << std::endl;
  else
    {
      try
	{
	  Game	game;
	  game.Run();
	}
      catch (const RuntimeException &exeption)
	{
	  std::cerr << exeption.what() << " in " << exeption.where() << std::endl;
	}
      catch (const std::bad_alloc &)
	{
	  std::cerr << "Empty memory" << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Unknown error" << std::endl;
	}
    }

  return (EXIT_SUCCESS);
}
