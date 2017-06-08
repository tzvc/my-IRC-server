/*
** main.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/client/src
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Fri May 26 13:07:15 2017 bufferking
** Last update Tue Jun  6 14:29:38 2017 
*/

#include "irc_client.h"

struct termios	g_s_orig_termios;

int		logmsg(enum e_logtype mode, char *format, ...)
{
  va_list	args;

  va_start(args, format);
  if (mode == INFO)
    fprintf(stderr, "\033[92m[+] ");
  else if (mode == ERROR)
    fprintf(stderr, "\033[91m[!] ");
  else
    fprintf(stderr, "\033[94m[-] ");
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\033[0m");
  return (0);
}

int	print_error(const char *func_name)
{
  logmsg(ERROR, "Error in %s(): %s\n", func_name, strerror(errno));
  return (EXIT_FAILURE);
}

static void	reset_terminal_mode(void)
{
    tcsetattr(0, TCSANOW, &g_s_orig_termios);
}

static void		set_conio_terminal_mode(void)
{
    struct termios	new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &g_s_orig_termios);
    memcpy(&new_termios, &g_s_orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int			main(void)
{  
  int			ret;

  //set_conio_terminal_mode();
  ret = client_wrapper();
  //tcsetattr(0, TCSANOW, &g_s_orig_termios);
  return (ret);
}
