##
## Makefile for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
## 
## Made by theo champion
## Login   <theo.champion@epitech.eu>
## 
## Started on  Wed May 24 15:39:32 2017 theo champion
## Last update Thu May 25 19:10:54 2017 theo champion
##

CC	= gcc

RM	= rm -f

SRCDIR	= ./src

CFLAGS	+= -W -Wall -Wextra -g3
CFLAGS	+= -I./include

NAME	= server

SRCS	= $(SRCDIR)/main.c			\
          $(SRCDIR)/socket.c			\
          $(SRCDIR)/client_handler.c		\
	  $(SRCDIR)/user_manager.c		\
	  $(SRCDIR)/utils.c			\
	  $(SRCDIR)/connection_registration.c	\
	  $(SRCDIR)/chan_manager.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY:	all clean fclean re
