/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcollon <jcollon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 09:53:47 by amiguez           #+#    #+#             */
/*   Updated: 2022/06/15 06:17:16 by jcollon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

void	init_shell(char **env);
char	**split_pipe(char *line);
char	**split_cmd(char *line);

#endif