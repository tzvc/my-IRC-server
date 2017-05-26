/*
** utils.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Thu May 25 19:02:40 2017 theo champion
** Last update Fri May 26 13:35:36 2017 theo champion
*/

#include "irc_server.h"

void		log_msg(int mode, const char *fmt, ...)
{
  va_list	ap;

  va_start(ap, fmt);
  if (mode == INFO)
    fprintf(stderr, "\033[34m[INFO] ");
  else if (mode == ERROR)
    fprintf(stderr, "\033[31m[ERROR] ");
  else
    fprintf(stderr, "\033[33m[DEBUG] ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n\033[0m");
  va_end(ap);
}
