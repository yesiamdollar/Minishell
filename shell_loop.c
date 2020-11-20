/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamzil <aamzil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:17:47 by aamzil            #+#    #+#             */
/*   Updated: 2020/10/31 10:54:21 by aamzil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		prompt(void)
{
	return (write(1, "$ ", 2));
}

void	ft_sigint(int num)
{
	(void)num;
	if (g_line != NULL)
	{
		g_line = NULL;
	}
	FT_PUTSTR("\n$ ");
}

/*
** To see the commands with their parmams,
** add this line print_list(g_cmd_list)
** before close_fd() function
*/

char	*read_line(void)
{
	char	*tmp;
	int		n;

	tmp = ft_strdup("");
	g_there_is_error = 0;
	add_mem(tmp);
	g_line = NULL;
	while ((n = get_next_line(0, &tmp)) >= 0)
	{
		if (g_line)
		{
			g_line = ft_strjoin(g_line, tmp);
			add_mem(tmp);
		}
		else
			g_line = tmp;
		add_mem(g_line);
		if (n > 0)
			break ;
		else if (n == 0 && !(*tmp) && !(*g_line))
			ft_exit(NULL, NULL);
	}
	return (g_line);
}

void	shell_loop(char **envp)
{
	signal(SIGINT, ft_sigint);
	// Ctrl - C free the line
	signal(SIGQUIT, ft_sigint);
	while (prompt())
	{
		g_line = read_line();
		if (check_semicolons(g_line))
		{
			FT_PUTSTR_ERR(ERROR_MSG);
			clear_cmd_list(&g_cmd_list);
			free_memory(&g_mem_alloc, FREE_MODE);
			continue;
		}
		treat_line(g_line);
		treat_list(g_cmd_list);
		if (THERE_IS_ERROR)
		{
			FT_PUTSTR_ERR(ERROR_MSG);
			clear_cmd_list(&g_cmd_list);
			free_memory(&g_mem_alloc, FREE_MODE);
			continue;
		}
		// sort_output_redir(g_cmd_list);
		treat_single_command(g_cmd_list);
		treat_cmd(g_cmd_list, envp);
		// print_list(g_cmd_list);
		call_commands(g_cmd_list, &envp);
		close_fd();
		clear_cmd_list(&g_cmd_list);
		free_memory(&g_mem_alloc, FREE_MODE);
	}
}
