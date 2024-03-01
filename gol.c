/*
 * Copyright (c) 2024 Jose G Perez Taveras <josegpt27@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HEIGHT 5
#define WIDTH  5

enum cell {
	DEAD,
	ALIVE
};

struct board {
	enum cell cells[HEIGHT][WIDTH];
};

void printboard(struct board);
struct board nextboard(struct board);
size_t cellneighbors(struct board, size_t, size_t);

/*
.#...
..#..
###..
.....
.....

.....
#.#..
.##..
.#...
.....
*/

static struct board state = {
	{
		{DEAD, ALIVE, DEAD, DEAD, DEAD},
		{DEAD, DEAD, ALIVE, DEAD, DEAD},
		{ALIVE, ALIVE, ALIVE, DEAD, DEAD}
	}
};

int
main(void)
{
	for (;;) {
		printboard(state);
		state = nextboard(state);
		usleep(150000);
		printf("\033[%dA", HEIGHT);
		printf("\033[%dD", WIDTH);
	}
	return EXIT_SUCCESS;
}

void
printboard(struct board state)
{
	size_t x, y;

	for (y = 0; y < HEIGHT; ++y) {
		for (x = 0; x < WIDTH; ++x) {
			switch (state.cells[y][x]) {
			case DEAD:
				printf(".");
			break;
			case ALIVE:
				printf("#");
			break;
			default: assert(0 && "Unreachable");
			}
		}
		printf("\n");
	}
}

struct board
nextboard(struct board state)
{
	struct board result;
	size_t n, x, y;

	for (y = 0; y < HEIGHT; ++y) {
		for (x = 0; x < WIDTH; ++x) {
			n = cellneighbors(state, y, x);
			switch (state.cells[y][x]) {
			case DEAD: {
				if (n == 3) {
					result.cells[y][x] = ALIVE;
				} else {
					result.cells[y][x] = DEAD;
				}
			} break;
			case ALIVE: {
				if (n == 2 || n == 3) {
					result.cells[y][x] = ALIVE;
				} else {
					result.cells[y][x] = DEAD;
				}
			} break;
			default: assert(0 && "Unreachable");
			}
		}
	}
	return result;
}

size_t
cellneighbors(struct board state, size_t y0, size_t x0)
{
	size_t n;
	int x, y, dx, dy;

	n = 0;
	for (dy = -1; dy <= 1; ++dy) {
		for (dx = -1; dx <= 1; ++dx) {
			if (dy != 0 || dx != 0) {
				y = y0 + dy;
				x = x0 + dx;

				if (y < 0) y += HEIGHT;
				else       y %= HEIGHT;

				if (x < 0) x += WIDTH;
				else       x %= WIDTH;

				if (state.cells[y][x] == ALIVE) ++n;
			}
		}
	}
	assert(n < 9);
	return n;
}

