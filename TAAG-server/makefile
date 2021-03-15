NAME=		erelia_server

##
##		RESSOURCES
##

##List every folder where he will find header
INC_DIR = 	$(shell find includes -type d)
INC_DIR += 	$(shell find ../TAAG-common/includes -type d)
INC_DIR +=	$(shell find ~/JGL/JGL/includes -type d)
##List every folder where he will find source file
SRC_DIR = 	$(shell find srcs -type d)
SRC_DIR += 	$(shell find ../TAAG-common/srcs -type d)
##Folder where he will store the .o
OBJ_DIR	=	obj
##Folder where to find libraries
LIB_DIR =	~/JGL/JGL

##Add to the vpath the folder find in SRC_DIR
vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)

##Set the download program to brew
DWL =		sudo apt-get

##List every .c found inside SRC_DIR
SRC = 		$(foreach dir, $(SRC_DIR), $(foreach file, $(wildcard $(dir)/*.cpp), $(notdir $(file))))

##Transform and place every .o from SRC
OBJ=		$(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))

##List of libraries to download and install using the function defined in DWL
LIB_DWN =	libglew-dev libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libasio-dev
##List of libraries to link while compiling
LIB =		SDL2 SDL2_image SDL2_mixer SDL2_ttf GL GLU jgl

##
##		COMPILER FLAGS
##

##Basics flags
CFLAGS=		`sdl2-config --cflags --libs` -pthread -O3 -flto -std=c++17 -Wno-deprecated-declarations

##Create the flags to includes every .h needed by this program
IFLAGS =	$(foreach dir, $(INC_DIR), -I$(dir))

##Create the flags to link every libraries needed by this program (-L and -l)
LFLAGS = 	$(foreach dir, $(LIB_DIR), -L $(dir) ) $(foreach lib, $(LIB), -l$(lib) )

##Define the compiler to use
CC=		g++

all:
		make $(NAME)

install:
		make libs
		make $(NAME)

libs:
		make re -C ../../JGL/JGL

$(NAME):	$(OBJ) $(INC_DIR)
		@echo "Compiling $(NAME) ...\c"
		@$(CC) $(CFLAGS) $(IFLAGS) -o $(NAME) $(OBJ) $(LFLAGS)
		@echo " DONE"

$(OBJ_DIR)/%.o : %.cpp
		@mkdir $(OBJ_DIR) 2> /dev/null || true
		@echo "Compiling $< ...\c"
		@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $<
		@echo " DONE"

clean:
		@rm -rf $(OBJ_DIR)
		@echo "clean"

fclean:		clean
		@rm -f $(NAME)
		@echo "fclean"

re:		fclean all

.PHONY: all clean fclean re

