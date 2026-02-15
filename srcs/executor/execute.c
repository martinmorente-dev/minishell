/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martin <martin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 15:57:50 by mafarino          #+#    #+#             */
/*   Updated: 2026/02/15 19:43:25 by martin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ========================================================================== */
/*                    Checking built-in commands                              */
/* ========================================================================== */

/*
** is_builtin - Checks whether the command is built-in
**
** PARAMETERS:
** cmd: Command name
**
** RETURN:
** true if the command is builtin, false otherwise
**
** BUILTIN COMMANDS:
** - echo
** - cd 
** - pwd
** - export
** - unset
** - env
** - exit 
*/
bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (true);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (true);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (true);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (true);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (true);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (true);
	return (false);
}

/*
** exec_builtin - Executes a builtin command
**
** PARAMETERS:
** shell: Shell structure (para modificar exit_flag)
** cmd: Command to execute
**
** RETURN:
** Return code of the command (0 = success, 1+ = error)
*/
int	exec_builtin(t_minishell *shell, t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (builtin_echo(cmd->args));
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (builtin_cd(cmd->args));
	if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (builtin_env(shell->env));
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (builtin_exit(shell, cmd->args));
	/*if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (builtin_export(cmd->args, &shell->env));
	if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (builtin_unset(cmd->args, &shell->env));
	*/
	return (1);
}

/* ========================================================================== */
/*                     APPLICATION OF REDIRECTIONS                            */
/* ========================================================================== */

/*
** apply_redirections - Applies all command redirections
**
** PARAMETERS:
** redirs: List of redirections
**
** RETURN:
** 0 on success, -1 on error
**
** DESCRIPTION:
** Opens files for redirections and performs dup2 on stdin/stdout
**
** TYPES OF REDIRECTIONS:
** - REDIR_IN (<):      open(file, O_RDONLY), dup2 to STDIN
** - REDIR_OUT (>):     open(file, O_WRONLY|O_CREAT|O_TRUNC), dup2 to STDOUT
** - REDIR_APPEND (>>): open(file, O_WRONLY|O_CREAT|O_APPEND), dup2 to STDOUT
** - REDIR_HEREDOC (<<): Handled separately
*/
int	apply_redirections(t_redir *redirs)
{
	int		fd;
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			fd = open(current->file, O_RDONLY);
			if (fd < 0)
			{
				perror(current->file);
				return (-1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (current->type == REDIR_OUT)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror(current->file);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current->type == REDIR_APPEND)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror(current->file);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current->type == REDIR_HEREDOC)
		{
			fd = handle_heredoc(current->file);
			if (fd < 0)
				return (-1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		current = current->next;
	}
	return (0);
}

/* ========================================================================== */
/*                     Execution of a single command                          */
/* ========================================================================== */

/*
** execute_single_command - Executes a single command without a pipe
**
** PARAMETERS:
** shell: Shell structure
**
** RETURN:
** Return code of the command
**
** ALGORITHM:
** 1. Check if the command is not empty
** 2. If builtin – execute without fork
** 3. Otherwise – fork and execve
** 4. Apply redirections in the child process
** 5. Wait for the child process to complete
*/
static int	execute_single_command(t_minishell *shell)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**envp;

	if (!shell->commands || !shell->commands->args || !shell->commands->args[0])
		return (0);
	
	// If it is a builtin command – execute in the current process
	if (is_builtin(shell->commands->args[0]))
	{
		// Save the original stdin/stdout for restoration
		int	saved_stdin = dup(STDIN_FILENO);
		int	saved_stdout = dup(STDOUT_FILENO);
		
		// Apply redirections
		if (apply_redirections(shell->commands->redirs) < 0)
		{
			close(saved_stdin);
			close(saved_stdout);
			return (1);
		}
		
		// Execute builtin (ahora pasamos shell para que pueda modificar exit_flag)
		status = exec_builtin(shell, shell->commands);
		
		// Restore stdin/stdout
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		
		return (status);
	}
	
	// External command – required fork
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	
	if (pid == 0)
	{
		// Child process
		
		// Restore default signal behavior
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		
		// Apply redirections
		if (apply_redirections(shell->commands->redirs) < 0)
			exit(1);
		
		// Search for the command path
		path = find_command_path(shell->commands->args[0], shell->env);
		if (!path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(shell->commands->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
		
		// Convert env to an array for execve
		envp = env_to_array(shell->env);
		if (!envp)
			exit(1);
		
		// Execute the command
		execve(path, shell->commands->args, envp);
		
		// If execve returns – an error occurred
		perror("execve");
		exit(126);
	}
	
	// Parent process – wait for the child to finish
	waitpid(pid, &status, 0);
	
	// Return the exit code
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	
	return (1);
}

/* ========================================================================== */
/*                     Execution PIPELINE                                     */
/* ========================================================================== */

/*
** count_commands - Counts the number of commands in the pipeline
*/
static int	count_commands(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

static int env_size(t_env *env)
{
    int count = 0;
    while (env)
    {
        count++;
        env = env->next;
    }
    return count;
}

char **env_to_array(t_env *env)
{
    int i = 0;
    int size = env_size(env);
    char **envp = malloc(sizeof(char *) * (size + 1));
    if (!envp)
        return NULL;

    while (env)
    {
        int len = ft_strlen(env->key) + 1 + ft_strlen(env->value) + 1;
        envp[i] = malloc(len);
        if (!envp[i])
        {
            for (int j = 0; j < i; j++)
                free(envp[j]);
            free(envp);
            return NULL;
        }

        int j = 0;
        for (int k = 0; env->key[k]; k++, j++)
            envp[i][j] = env->key[k];

        envp[i][j++] = '=';

        for (int k = 0; env->value[k]; k++, j++)
            envp[i][j] = env->value[k];

        envp[i][j] = '\0';

        i++;
        env = env->next;
    }

    envp[i] = NULL; 
    return envp;
}

/*
** execute_pipeline – Executes multiple commands with pipes
**
** PARAMETERS:
** shell: Shell structure
**
** RETURN:
** Return code of the last command
**
** ALGORITHM:
** 1. Create an array of pipes
** 2. For each command:
** - Fork
** - In the child process:
** - Set up stdin from the previous pipe
** - Set up stdout to the next pipe
** - Close all pipes
** - Apply redirections
** - Execute the command
** 3. In the parent process:
** - Close all pipes
** - Wait for all child processes
** 4. Return the code of the last command
*/
static int	execute_pipeline(t_minishell *shell)
{
	int		num_cmds;
	int		**pipes;
	pid_t	*pids;
	int		i;
	int		status;
	t_cmd	*current;

	num_cmds = count_commands(shell->commands);
	
	// Create an array of pipes (num_cmds - 1 pipes)
	pipes = malloc(sizeof(int *) * (num_cmds - 1));
	if (!pipes)
		return (1);
	
	for (i = 0; i < num_cmds - 1; i++)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) < 0)
		{
			perror("pipe");
			// Free the already created pipes
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (1);
		}
	}
	
	// Array for storing child process PIDs
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
	{
		for (i = 0; i < num_cmds - 1; i++)
			free(pipes[i]);
		free(pipes);
		return (1);
	}
	
	// Execute each command
	current = shell->commands;
	i = 0;
	while (current)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("fork");
			// Cleanup
			for (int j = 0; j < num_cmds - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				free(pipes[j]);
			}
			free(pipes);
			free(pids);
			return (1);
		}
		
		if (pids[i] == 0)
		{
			// Child process
			
			// Restore default signal behavior
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			
			// Set up stdin: if not the first command, read from the previous pipe
			if (i > 0)
			{
				dup2(pipes[i - 1][0], STDIN_FILENO);
			}
			
			// Set up stdout: if not the last command, write to the next pipe
			if (i < num_cmds - 1)
			{
				dup2(pipes[i][1], STDOUT_FILENO);
			}
			
			// Close all pipes in the child process
			for (int j = 0; j < num_cmds - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			
			// Apply redirections (they will override pipe settings if present)
			if (apply_redirections(current->redirs) < 0)
				exit(1);
			
			// Execute the command
			if (is_builtin(current->args[0]))
			{
				// En pipelines, builtins se ejecutan en el child
				// No pueden modificar el shell del padre
				t_minishell temp_shell = *shell;
				temp_shell.commands = current;
				status = exec_builtin(&temp_shell, current);
				exit(status);
			}
			else
			{
				char *path = find_command_path(current->args[0], shell->env);
				if (!path)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(current->args[0], 2);
					ft_putstr_fd(": command not found\n", 2);
					exit(127);
				}
				
				char **envp = env_to_array(shell->env);
				if (!envp)
					exit(1);
				
				execve(path, current->args, envp);
				perror("execve");
				exit(126);
			}
		}
		
		current = current->next;
		i++;
	}
	
	// Parent process – close all pipes
	for (i = 0; i < num_cmds - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
	}
	free(pipes);
	
	// Wait for all child processes to finish
	int last_status = 0;
	for (i = 0; i < num_cmds; i++)
	{
		waitpid(pids[i], &status, 0);
		// Save the exit code of the last command
		if (i == num_cmds - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
	
	free(pids);
	return (last_status);
}

/* ========================================================================== */
/*                     MAIN EXECUTION FUNCTION                                */
/* ========================================================================== */

/*
** execute_commands - Main command execution function
**
** PARAMETERS:
** shell: Shell structure with commands to execute
**
** RETURN:
** Return code of the last command
**
** DESCRIPTION:
** Determines whether this is a single command or a pipeline,
** and calls the appropriate execution function.
**
** EXAMPLES:
**
** EXAMPLE 1: Single command
** Input: "ls -la"
** → execute_single_command()
**
** EXAMPLE 2: Pipeline
** Input: "cat file | grep hello | wc -l"
** → execute_pipeline()
**
** EXAMPLE 3: Builtin
** Input: "cd /home"
** → Executed without fork in execute_single_command()
**
** EXAMPLE 4: Redirection
** Input: "cat < in.txt > out.txt"
** → execute_single_command() with redirections applied
*/
int	execute_commands(t_minishell *shell)
{
	if (!shell->commands)
		return (0);
	
	// Check whether there is a pipe (multiple commands)
	if (shell->commands->next)
	{
		// Pipeline - multiple commands
		return (execute_pipeline(shell));
	}
	else
	{
		// Single command
		return (execute_single_command(shell));
	}
}