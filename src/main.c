/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/05/09 11:31:54 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(void)
{
	char	*command_line;

	write(1, "\033[H\033[2J", 7);
	// TERMINALI TEMIZLEMEK ICIN WRITE FONKSIYONU
	// \033 2 adet var bunlar escapeler toplam 2 karakter, [H ile imleci ekranın sol üstüne taşıyoruz (2 karakter)
	// [2J ile ekranı temizliyoruz (3 karakter) toplam 7 karakter
	while (1)
	{
		command_line = readline("MiniShell: ");
		if (command_line)
		{
			add_history(command_line);
			//free(first); exit komutu girilince adres bloğu her halükarda serbest bırakıldığı için strcmp fonksiyonu karşılaştırma yapamıyor...
			if (!ft_strcmp(command_line, "exit")) // komut "exit" mi diye kontrol et
				break ;
			printf("(?) Unkown command: %s\n", command_line); // exit veya bizim komutlar değilse bilinmeyen komut yazdır.
		}
	}
	return (0);
}
