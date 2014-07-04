

NAME=		exec

G_SRC=		main.cpp		\
		Game.cpp		\
		RuntimeException.cpp	\
		MultiDraw.cpp		\
		Utils.cpp		\
		GL_Light.cpp

3D_SRC=		3D/AAnimation.cpp	\
		3D/RessourceManager.cpp	\
		3D/Camera_FreeFly.cpp	\
		3D/Renderer.cpp		\
		3D/Texture1D.cpp

MD3_SRC=	3D/Md3/Md3Exception.cpp		\
		3D/Md3/Md3LodPlayer.cpp		\
		3D/Md3/Md3Mesh.cpp		\
		3D/Md3/Md3Model.cpp		\
		3D/Md3/Md3Player.cpp		\
		3D/Md3/Md3PlayerSkin.cpp	\
		3D/Md3/Md3Alien.cpp		\
		3D/Md3/Md3AlienSkin.cpp		\
		3D/Md3/NormalLookupTable.cpp

SRC=		$(G_SRC) $(3D_SRC) $(MD3_SRC)

OBJ=		$(SRC:.cpp=.o)

# LIB_PATH =	./lib/SFML-2.0/lib
# INC_PATH =	./lib/SFML-2.0/include

SFML_PATH=	/home/barbie/Programmation/Library/SFML/SFML-2.1_git/SFML
LIB_PATH=	$(SFML_PATH)/lib
INC_PATH=	$(SFML_PATH)/include

CXXFLAGS=	-Wall -W -Wextra -Wunused -g3 \
		-I $(INC_PATH) \
		-I ./	\
		-DFILE_PATH="\"./\""

# LDFLAGS=	-Wl,--rpath=$(LIB_PATH)			\
# 		$(LIB_PATH)/libsfml-graphics.so.2.0	\
# 		$(LIB_PATH)/libsfml-window.so.2.0	\
# 		$(LIB_PATH)/libsfml-system.so.2.0	\
# 		-lGL -lGLU

LDFLAGS=	-Wl,--rpath=$(LIB_PATH)			\
		$(LIB_PATH)/libsfml-graphics.so.2.1	\
		$(LIB_PATH)/libsfml-window.so.2.1	\
		$(LIB_PATH)/libsfml-system.so.2.1	\
		-lGL -lGLU

CXX=		g++
RM=		rm -f


all:		$(NAME)

$(NAME):	$(OBJ)
		$(CXX) $(OBJ) -o $(NAME) $(LDFLAGS)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY:		all clean fclean re

