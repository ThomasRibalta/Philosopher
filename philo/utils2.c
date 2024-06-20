/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomas.rba <thomas.rba@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 22:05:40 by thomas.rba        #+#    #+#             */
/*   Updated: 2024/06/20 22:06:12 by thomas.rba       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philosopher.h"

int	aff_error(int e)
{
	if (e == 1)
		printf("Le nombre de philosopheur est incorrect\n");
	else if (e == 2)
		printf("On vous a dit au dessus de 60ms pour tout les times\n");
	else if (e == 3)
		printf("./philo_bonus nbPhilo time time time (nb_eat)\n");
	return (e);
}