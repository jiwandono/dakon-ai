#ifndef DAKON_H
#define	DAKON_H

#undef NDEBUG

#include <stdio.h>
#include <vector>
#include <queue>
#include <assert.h>

#define DAKON_DEPTH 8
#define DAKON_LUBANG 7
#define DAKON_PLAYER_HUMAN 0
#define DAKON_PLAYER_COMPUTER 1
#define DAKON_PLAYER_HUMAN_NAME "HUMAN"
#define DAKON_PLAYER_COMPUTER_NAME "COMPUTER"
#define DAKON_INF_SCORE 10000

#define dakon_get_player_name(p) (p == DAKON_PLAYER_HUMAN ? "HUMAN" : "COMPUTER")

int dakon_node_counter;

typedef struct {
	int giliran;
	int lumbung[2];
	int lubang[2][DAKON_LUBANG];
} board;

typedef struct {
	int move;
	int priority;
} dakon_move;

class Compare_Move {
public:
	bool operator() (dakon_move& m1, dakon_move& m2) {
		return m1.priority > m2.priority;
	}
};

void dakon_print(board b) {
	printf("\n");

	printf("           ");
	for(int i = 0; i < DAKON_LUBANG; i++) {
			printf(" C%d  ", i+1);
	}
	printf("\n");

	printf("          |");
	for(int i = 0; i < DAKON_LUBANG; i++) {
		if(b.lubang[DAKON_PLAYER_COMPUTER][i] > 0) {
			printf(" %2d |", b.lubang[1][i]);
		} else {
			printf(" -- |");
		}
	}
	printf("\n");

	printf("HUMAN  %2d |", b.lumbung[DAKON_PLAYER_HUMAN]);
	for(int i = 0; i < DAKON_LUBANG; i++) {
		if(i < DAKON_LUBANG -1) {
			printf("     ");
		} else {
			printf("    ");
		}
	}
	printf("| %2d  COMPUTER\n", b.lumbung[DAKON_PLAYER_COMPUTER]);

	printf("          |");
	for(int i = DAKON_LUBANG-1; i >= 0 ; i--) {
		if(b.lubang[DAKON_PLAYER_HUMAN][i] > 0) {
			printf(" %2d |", b.lubang[DAKON_PLAYER_HUMAN][i]);
		} else {
			printf(" -- |");
		}
	}
	printf("\n");

	printf("           ");
	for(int i = DAKON_LUBANG-1; i >= 0 ; i--) {
		printf(" H%d  ", i+1);
	}
	printf("\n");

	printf("\n");
}

void dakon_init(board* b) {
	b->giliran = 0;

	b->lumbung[0] = 0;
	b->lumbung[1] = 0;

	for(int i = 0; i < DAKON_LUBANG; i++) {
		b->lubang[0][i] = DAKON_LUBANG;
		b->lubang[1][i] = DAKON_LUBANG;
	}
}

bool dakon_is_playable(board b) {
	bool playable[2] = { false };

	int lubang = 0;
	while(lubang < DAKON_LUBANG) {
		if(b.lubang[DAKON_PLAYER_HUMAN][lubang] > 0) {
			playable[DAKON_PLAYER_HUMAN] = true;
		}

		if(b.lubang[DAKON_PLAYER_COMPUTER][lubang] > 0) {
			playable[DAKON_PLAYER_COMPUTER] = true;
		}

		lubang++;
	}

	return playable[0] && playable[1];
}

void dakon_play(board* b, int lubang, int lubang_siapa) {
	int biji;
	bool stop_di_lumbung;
	bool play;
	bool bedhil;

	assert(b);
	assert(lubang >= 0 && lubang < DAKON_LUBANG);
	assert(lubang_siapa == DAKON_PLAYER_HUMAN || lubang_siapa == DAKON_PLAYER_COMPUTER);

	biji = b->lubang[lubang_siapa][lubang];
	assert(biji > 0);
	b->lubang[lubang_siapa][lubang] = 0;

	lubang++;

	stop_di_lumbung = false;
	while(biji > 0) {
		while(biji > 0 && lubang < DAKON_LUBANG) {
			biji--;
			b->lubang[lubang_siapa][lubang]++;
			lubang++;
			stop_di_lumbung = false;
		}

		if(biji > 0 && b->giliran == lubang_siapa) {
			biji--;
			b->lumbung[b->giliran]++;
			stop_di_lumbung = true;
		}

		/* Switch */
		if(biji > 0) {
			lubang_siapa ^= 1;
			lubang = 0;
		}
	}

	play = true;
	bedhil = false;

	if(stop_di_lumbung) {
		play = false;
	} else {
		lubang--;
		if(b->lubang[lubang_siapa][lubang] == 1) {
			play = false;
			if(lubang_siapa == b->giliran) {
				bedhil = true;
			}
		}
	}

	if(play) {
		dakon_play(b, lubang, lubang_siapa);
	} else {
		if(bedhil) {
			int bedhil_target_player = lubang_siapa^1;
			int bedhil_target_lubang = DAKON_LUBANG-1 -lubang;
			int bedhil_hasil = b->lubang[bedhil_target_player][bedhil_target_lubang];
			if(bedhil_hasil > 0) {
				b->lumbung[b->giliran] += bedhil_hasil;
				b->lubang[bedhil_target_player][bedhil_target_lubang] = 0;
				//~ printf("BEDHIL!! Masukkan %d biji %s ke lumbung.\n", bedhil_hasil, dakon_get_player_name(bedhil_target_player));
			}
		}

		// Switch player
		b->giliran ^= 1;
	}
}

int dakon_diff(board b, int player) {
	int biji[2];
	int diff;

	for(int p = 0; p < 2; p++) {
		biji[p] = b.lumbung[p];
		for(int lubang = 0; lubang < DAKON_LUBANG; lubang++) {
			biji[p] += b.lubang[p][lubang];
		}
	}

	diff = biji[DAKON_PLAYER_COMPUTER] - biji[DAKON_PLAYER_HUMAN];
	diff *= (player == DAKON_PLAYER_COMPUTER) ? 1 : -1;

	return diff;
}

int dakon_eval(board b, int player) {
	//~ return dakon_diff(b, player);

	int score;

	score = b.lumbung[DAKON_PLAYER_COMPUTER] - b.lumbung[DAKON_PLAYER_HUMAN];
	score *= (player == DAKON_PLAYER_COMPUTER) ? 1 : -1;

	return score;
}

int rec_minimax(board b, int depth, int move) {
	assert(depth >= 0);
	dakon_node_counter++;

	int ret;
	int giliran = b.giliran;

	//~ printf("---- Recursive minimax at depth %d, move = %d\n", depth, move+1);

	dakon_play(&b, move, b.giliran);

	if(!dakon_is_playable(b) || depth == 0) {
		ret = dakon_eval(b, giliran);
	} else {
		ret = -DAKON_INF_SCORE;
		for(int i = 0; i < DAKON_LUBANG; i++) {
			if(b.lubang[b.giliran][i] > 0) {
				int tmp = -rec_minimax(b, depth-1, i);
				ret = std::max(ret, tmp);
			}
		}
	}

	return ret;
}

int minimax(board b, int depth) {
	dakon_node_counter = 0;

	int best_move = -1;
	int best_value = -DAKON_INF_SCORE;

	for(int move = 0; move < DAKON_LUBANG; move++) {
		if(b.lubang[b.giliran][move] > 0) {
			int v = rec_minimax(b, depth-1, move);
			//~ printf("-- Minimax value for lubang %d: %d\n", move+1, v);

			if(v > best_value) {
				best_value = v;
				best_move = move;
			}
		}
	}

	return best_move;
}

int rec_abprune(board b, int depth, int move, int alpha, int beta, bool heur) {
	assert(depth >= 0);
	dakon_node_counter++;

	int ret;
	int giliran = b.giliran;

	//~ printf("---- Recursive abprune at depth %d, move = %d\n", depth, move+1);

	dakon_play(&b, move, b.giliran);

	if(!dakon_is_playable(b) || depth == 0) {
		ret = dakon_eval(b, giliran);
	} else {
		ret = -DAKON_INF_SCORE;

		if(heur) {
			std::priority_queue<dakon_move, std::vector<dakon_move>, Compare_Move> pq;
			dakon_move m;
			for(int i = 0; i < DAKON_LUBANG; i++) {
				m.move = i;
				m.priority = b.lubang[b.giliran][i];
				pq.push(m);
			}

			while(!pq.empty()) {
				m = pq.top();
				int i = m.move;
				if(b.lubang[b.giliran][i] > 0) {
					int tmp = -rec_abprune(b, depth-1, i, -beta, -alpha, heur);

					if(tmp >= beta) {
						return tmp;
					}

					if(tmp > alpha) {
						alpha = tmp;
					}
				}
				pq.pop();
			}
		} else {
			for(int i = 0; i < DAKON_LUBANG; i++) {
				if(b.lubang[b.giliran][i] > 0) {
					int tmp = -rec_abprune(b, depth-1, i, -beta, -alpha, heur);

					if(tmp >= beta) {
						return tmp;
					}

					if(tmp > alpha) {
						alpha = tmp;
					}
				}
			}
		}

		return alpha;
	}

	return ret;
}

int abprune(board b, int depth, bool heur) {
	dakon_node_counter = 0;
	int best_decision = -1;
	int best_value = -DAKON_INF_SCORE;

	if(heur) {
		std::priority_queue<dakon_move, std::vector<dakon_move>, Compare_Move> pq;
		dakon_move m;
		for(int move = 0; move < DAKON_LUBANG; move++) {
			m.move = move;
			m.priority = b.lubang[b.giliran][move];
			pq.push(m);
		}

		while(!pq.empty()) {
			m = pq.top();
			int move = m.move;
			if(b.lubang[b.giliran][move] > 0) {
				int v = rec_abprune(b, depth-1, move, -DAKON_INF_SCORE, DAKON_INF_SCORE, heur);
				//~ printf("-- abprune value for lubang %d: %d\n", move+1, v);

				if(v > best_value) {
					best_value = v;
					best_decision = move;
				}
			}
			pq.pop();
		}
	} else {
		for(int move = 0; move < DAKON_LUBANG; move++) {
			if(b.lubang[b.giliran][move] > 0) {
				int v = rec_abprune(b, depth-1, move, -DAKON_INF_SCORE, DAKON_INF_SCORE, heur);
				//~ printf("-- abprune value for lubang %d: %d\n", move+1, v);

				if(v > best_value) {
					best_value = v;
					best_decision = move;
				}
			}
		}
	}

	return best_decision;
}

#endif
