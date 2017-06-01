/*
** utils.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Thu May 25 19:02:40 2017 theo champion
** Last update Thu Jun  1 15:48:04 2017 theo champion
*/

#include "irc_server.h"

void		log_msg(int mode, const char *fmt, ...)
{
  va_list	ap;

  va_start(ap, fmt);
  if (mode == INFO)
    fprintf(stderr, "\033[34m[+] ");
  else if (mode == ERROR)
    fprintf(stderr, "\033[31m[!] ");
  else
    fprintf(stderr, "\033[33m[-] ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n\033[0m");
  va_end(ap);
}

char	*dyn_strcat(char *s1, const char *s2)
{
  const size_t a = strlen(s1);
  const size_t b = strlen(s2);

  s1 = realloc(s1, a + b + 1);
  memcpy(s1 + a, s2, b + 1);
  return (s1);
}
