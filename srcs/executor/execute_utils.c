/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 15:40:02 by mafarino          #+#    #+#             */
/*   Updated: 2026/02/05 20:20:43 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/* ========================================================================== */
/*                    HELPER FUNCTIONS                                        */
/* ========================================================================== */

/*
** is_absolute_or_relative_path - Checks if the command is a path
**
** PARAMETERS:
** cmd: Command to check
**
** RETURN:
** true if the command contains '/' (a path), false otherwise
**
** EXAMPLES:
** "/bin/ls"      → true (absolute path)
** "./minishell"  → true (relative path)
** "../test"      → true (relative path)
** "ls"           → false (search required in PATH)
*/
static bool	is_absolute_or_relative_path(char *cmd)
{
	int	i;

	if (!cmd)
		return (false);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (true);
		i++;
	}
	return (false);
}

/*
** is_executable - Checks if the file exists and is executable
**
** PARAMETERS:
** path: File path
**
** RETURN:
** true, if the file exists and is executable
** false otherwise
**
** CHECKS:
** 1. access(path, F_OK) - file exists
** 2. access(path, X_OK) - file is executable
*/
bool	is_executable(char *path)
{
	if (!path)
		return (false);
	if (access(path, F_OK) != 0)
		return (false);
	if (access(path, X_OK) != 0)
		return (false);
	return (true);
}

/*
** build_path – Joins a directory and a command name into a full path
**
** PARAMETERS:
** dir: Directory (e.g., "/usr/bin")
** cmd: Command name (e.g., "ls")
**
** RETURN:
** Full path (e.g., "/usr/bin/ls") or NULL on error
**
** IMPORTANT:
** - Adds '/' between the directory and the command
** - The returned string must be freed
**
** EXAMPLES:
** build_path("/usr/bin", "ls")   → "/usr/bin/ls"
** build_path("/bin", "cat")      → "/bin/cat"
*/
static char	*build_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*path;

	if (!dir || !cmd)
		return (NULL);
	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}

/* ========================================================================== */
/*                    SEARCH IN PATH                                          */
/* ========================================================================== */

/*
** search_in_path - Searches for the command in all directories of PATH
**
** PARAMETERS:
** cmd:   Command name
** paths: Array of directories from PATH
**
** RETURN:
** Full path to the command or NULL if not found
**
** ALGORITHM:
** 1.For each directory in paths:
** - Build the full path (dir/cmd)
** - Check if it is executable
** - If yes – return the path
** 2. If not found – return NULL
**
** EXAMPLES:
** paths = ["/usr/bin", "/bin", "/usr/local/bin"]
** cmd = "ls"
** → Checks:
**   1. "/usr/bin/ls"  → exists and is executable → RETURN
**   2. (the others are not checked)
*/
static char	*search_in_path(char *cmd, char **paths)
{
	char	*full_path;
	int		i;

	if (!cmd || !paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = build_path(paths[i], cmd);
		if (!full_path)
		{
			i++;
			continue;
		}
		if (is_executable(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

/* ========================================================================== */
/*                     MAIN FUNCTION                                          */
/* ========================================================================== */

/*
** find_command_path - Finds the full path to the command
**
** PARAMETERS:
** cmd: Command name or path
** env: List of environment variables
**
** RETURN:
** - Full path to the command (must be freed!)
** - NULL if the command is not found
**
** ALGORITHM:
** 1. If the command contains '/' (absolute or relative path):
** - Check if it is executable
** - Return a duplicate of the path
** 2. Otherwise (command without path):
** - Get PATH from the environment variables
** - Split PATH into directories (split by ':')
** - Search for the command in each directory
** - Return the first path found
** 3. If not found – return NULL
**
** EXAMPLES:
**
** EXAMPLE 1: Absolute path
** cmd = "/bin/ls"
** → Checks /bin/ls
** → Returns "/bin/ls"
**
** EXAMPLE 2: Relative path
** cmd = "./minishell"
** → Checks ./minishell
** → Returns "./minishell"
**
** EXAMPLE 3: Search in PATH
** cmd = "ls"
** PATH = "/usr/local/bin:/usr/bin:/bin"
** → Checks /usr/local/bin/ls (not found)
** → Checks /usr/bin/ls (found!)
** → Returns "/usr/bin/ls"
**
** EXAMPLE 4: Command not found
** cmd = "nonexistent"
** → Checks all directories in PATH
** → Returns NULL
**
** EXAMPLE 5: PATH not set
** cmd = "ls"
** PATH = NULL
** → Returns NULL
**
** USAGE:
**
** В execute_single_command():
** char *path = find_command_path(cmd->args[0], env);
** if (!path)
** {
**     printf("minishell: %s: command not found\n", cmd->args[0]);
**     exit(127);
** }
** execve(path, cmd->args, envp);
** free(path);
*/
char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*result;
	int		i;

	if (!cmd || !cmd[0])
		return (NULL);
	
	// CASE 1: Absolute or relative path
	if (is_absolute_or_relative_path(cmd))
	{
		if (is_executable(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	
	// CASE 2: Search in PATH
	
	// Get the value of the PATH variable
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	
	// Split PATH into directories (split by ':')
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	
	// Search the command in each directory
	result = search_in_path(cmd, paths);
	
	// Free the array of directories
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	
	return (result);
}
