#include "minishell.h"

char	*path_joiner(char *path, char *cmd)
{
	char	*ret;
	char	*slash;

	slash = ft_strjoin(path, "/");
	ret = ft_strjoin(slash, cmd);
	free(slash);
	return (ret);
}

char	*is_in_list(char *cmd)
{
	static char	*all[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	char		**others;
	char		*path;
	int			i;

	i = 0;
	while (i < 7)
		if (ft_strcmp(all[i++], cmd) == 0)
			return (ft_strdup(all[--i]));
	if (ft_strchr(cmd, '/') && access(cmd, F_OK) != -1)
		return (ft_strdup(cmd));
	path = getenv("PATH");
	others = ft_split(path, ':');
	i = 0;
	while (others[i])
	{
		path = path_joiner(others[i++], cmd);
		if (access(path, F_OK) != -1)
		{
			i = 0;
			while (others[i])
				free(others[i++]);
			free(others);
			return (path);
		}
		free(path);
	}
	i = 0;
	while (others[i])
		free(others[i++]);
	free(others);
	return (NULL);
}

char	*args_joiner(char *args, char *new)
{
	char	*joined;
	char	*space;

	if (args == NULL)
		return (ft_strdup(new));
	space = ft_strjoin(args, " ");
	joined = ft_strjoin(space, new);
	free(args);
	free(space);
	return (joined);
}


t_command	*get_cammand(char **buff)
{
	int			i;
	t_command	*command;
	int			ai;
	int			loop;

	ai = 0;

	i = 0;
	loop = 0;
	command = init_cmd(buff);
	while(buff[i])
	{
		command -> program = is_in_list(buff[i]);
		i++;
		while (buff[i] && buff[i][0] == FLAG_HYPHEN)
		{
			free(command -> options);
			command -> options = ft_strjoin(command -> options, buff[i++]);
		}
		if (buff[i] && ft_strchr(REDIRECTIONS, buff[i][0]))
			command -> redirection = ft_strdup(buff[i++]);
		while (buff[i])
			command -> args[ai++] = ft_strdup(buff[i++]);
	}
	command ->args[ai] = NULL;
	command -> execve = buff;
	return command;
}
