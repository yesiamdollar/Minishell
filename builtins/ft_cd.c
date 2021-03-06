/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamzil <aamzil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:55:09 by aamzil            #+#    #+#             */
/*   Updated: 2020/10/18 17:16:55 by aamzil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	cd_error_handling_helper(char **args, char **dir, char ***envp)
{
	if (args[0] && args[0][0] == '~')
	{
		(*dir) = ft_strjoin(ft_get_env_value("$HOME", (*envp)), &args[0][1]);
		if (!(*dir))
		{
			FT_PUTSTR_ERR("bash : cd : HOME not set : ERROR \n");
			return (1);
		}
		else
			add_mem((*dir));
	}
	(*dir) = (*dir) ? *dir : args[0];
	if (chdir(*dir) < 0)
	{
		FT_PUTSTR("bash : cd : No such file or directory\n");
		return (1);
	}
	return (0);
}

char	cd_error_handling(char **args, char **dir, char ***envp, char *prev_dir)
{
	if (!args[0])
	{
		if (!((*dir) = ft_get_env_value("$HOME", (*envp))))
		{
			FT_PUTSTR_ERR("bash : cd : HOME not SET : ERROR \n");
			return (1);
		}
	}
	else if (args[1])
	{
		FT_PUTSTR_ERR("bash : cd : TOO MANY ARGUMENT\n");
		return (1);
	}
	if (args[0] && ((*prev_dir) = !ft_strcmp(args[0], "-")))
	{
		if (!((*dir) = ft_get_env_value("$OLDPWD", (*envp))))
		{
			FT_PUTSTR_ERR("bash : cd : OLDPWD not SET : ERROR \n");
			return (1);
		}
	}
	return (cd_error_handling_helper(args, dir, envp));
}

int		ft_cd(char **args, char ***envp)
{
	char	*env_var[3];
	char	prev_dir;
	char	*cur_dir;
	char	*dir;

	prev_dir = 0;
	dir = NULL;
	if (cd_error_handling(args, &dir, envp, &prev_dir))
		return (g_there_is_error);
	cur_dir = ft_getcwd();
	add_mem(cur_dir);
	if (prev_dir)
	{
		FT_PUTSTR(cur_dir);
		FT_PUTCHAR('\n');
	}
	env_var[0] = ft_strjoin("OLDPWD=", ft_get_env_value("$PWD", (*envp)));
	add_mem(env_var[0]);
	env_var[1] = ft_strjoin("PWD=", cur_dir);
	add_mem(env_var[1]);
	env_var[2] = NULL;
	ft_export(env_var, envp);
	return (0);
}
