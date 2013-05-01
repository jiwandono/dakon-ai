#include <stdio.h>
#include "dakon.h"

int main() {
	board b;
	int in;

	dakon_init(&b);

	printf("Siapa yang main dulu (0=HUMAN, 1=COMPUTER) ? ");
	scanf("%d", &in);
	b.giliran = in;

	dakon_print(b);

	while(dakon_is_playable(b)) {
		int decision;
		printf("-- Giliran %s\n", dakon_get_player_name(b.giliran));

		if(b.giliran == DAKON_PLAYER_HUMAN) {
			do {
				printf("Pilih lubang 1-%d: ", DAKON_LUBANG);
				scanf("%d", &in);
				decision = in-1;
			} while(b.lubang[b.giliran][decision] == 0);
		} else {
			decision = abprune(b, 10, true);
			printf("Pilih lubang 1-%d: %d\n", DAKON_LUBANG, decision+1);
		}

		dakon_play(&b, decision, b.giliran);
		dakon_print(b);
	}

	int diff = dakon_diff(b, DAKON_PLAYER_HUMAN);

	if(diff > 0) {
		printf("%s menang!\n", DAKON_PLAYER_HUMAN_NAME);
	} else if(diff < 0) {
		printf("%s menang!\n", DAKON_PLAYER_COMPUTER_NAME);
	} else {
		printf("Seri!\n");
	}

	return 0;
}
