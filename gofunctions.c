/*Go game. Konstantinos Stefanidis (stef), Giannis Lamprou (koropi). Athens 2015*/
#include <stdio.h>
#include <stdlib.h>

int eligibility(char pl, int let, int num, int N, char **board){ 	/* function to determine whether position is valid for black or white stone placement */
char player, opponent;
if (pl == 'w') {		/* determine player symbol */
	player = 'O';
	opponent = 'X';
}
else {
	player = 'X';
	opponent = 'O';
}
if (hasfreedom(let, num, N, board))		/*if input block in the board has any freedoms then its eligible */
	return 1;
else {		/* else if the block doesn't have any freedoms but its about to capture blocks then its eligible */
	if (num + 1 < N)
		if (board[num + 1][let] == opponent && !(hasfreedom(let, num + 1, N, board)))
			return 1;
	if (num - 1 >= 0)
		if (board[num - 1][let] == opponent && !(hasfreedom(let, num - 1, N, board)))		
			return 1;
	if (let + 1 < N)
		if (board[num][let + 1] == opponent && !(hasfreedom(let + 1, num, N, board)))
			return 1;
	if (let - 1 >= 0)
		if (board[num][let - 1] == opponent && !(hasfreedom(let - 1, num, N, board)))
			return 1;		
}
return 0;
}

int hasfreedom(int let, int num, int N, char **board) {		/* function that prepares a temporary copy of the board for the recursive function rec_hasfreedom and calls it */
int i, j, freedomvalue;
char **temp_board;
temp_board = malloc(N * sizeof(char*));		/* memory allocation for temp_board a temporary parallel array to board */
if (temp_board == NULL) {
		fprintf(stderr, "Memory allocation for temp_board unsuccessful");
		return -1;
}
for (i = 0; i < N; i++) {
	temp_board[i] = malloc(N * sizeof(char));
	if (temp_board[i] == NULL) {
	fprintf(stderr, "Memory allocation for temp_board unsuccessful");
	return -1;
	}
}
for (i = 0; i < N; i++) {
	for (j = 0; j < N; j++) {
		temp_board[i][j] = board[i][j];
	}
}
freedomvalue = rec_hasfreedom(let, num, N, temp_board);		/* calling the recursive function rec_hasfreedom */
for (i = N - 1; i >= 0; i--)		
	free(temp_board[i]);
free(temp_board);
return freedomvalue;
}

int rec_hasfreedom(int let, int num, int N, char **temp_board) {		/* recursive function that determines if input block in the board has any freedoms whether its a part of a block or not */
char des_player;
des_player = temp_board[num][let];
temp_board[num][let] = 'C';		/* mark input block as 'C' (checked) */
if (num + 1 < N) {
	if (temp_board[num + 1][let] == '.')		/* if block has any freedoms return 1 */
		return 1;
	else if (temp_board[num + 1][let] == des_player)		/* else call the function recursively to check neighboring stones if any */
		if (rec_hasfreedom (let, num + 1, N, temp_board))
			return 1;	
}
if (num - 1 >= 0) {
	if (temp_board[num - 1][let] == '.')		/* if block has any freedoms return 1 */
		return 1;
	else if (temp_board[num - 1][let] == des_player)		/* else call the function recursively to check neighboring stones if any */
		if (rec_hasfreedom (let, num - 1, N, temp_board))
			return 1;
}
if (let + 1 < N) {
	if (temp_board[num][let + 1] == '.')		/* if block has any freedoms return 1 */
		return 1;
	else if (temp_board[num][let + 1] == des_player)		/* else call the function recursively to check neighboring stones if any */
		if (rec_hasfreedom (let + 1, num, N, temp_board))
			return 1;
}
if (let - 1 >= 0) {
	if (temp_board[num][let - 1] == '.')		/* if block has any freedoms return 1 */
		return 1;
	else if (temp_board[num][let - 1] == des_player)		/* else call the function recursively to check neighboring stones if any */
		if (rec_hasfreedom (let - 1, num, N, temp_board))
			return 1;
}
return 0;
}


int rec_removeblock(int let, int num, int N, char **board) {		/* recursive function that removes a block of stones of the same colour */
	char des_player;		
	des_player = board[num][let];			/* designated play that the function searches blocks to remove for */
	board[num][let] = '.';		/* put '.' in input block position (remove player stone) */
	if (num + 1 < N) {
		if (board[num + 1][let] == des_player) {		/* if there is a neighboring stone */
			rec_removeblock (let, num + 1, N, board);		/* call function recursively for that block */
		}
	}
	if (num - 1 >= 0) {
		if (board[num - 1][let] == des_player) {		/* if there is a neighboring stone */
			rec_removeblock (let, num - 1, N, board);		/* call function recursively for that block */
		}	
	}
	if (let + 1 < N) {
		if (board[num][let + 1] == des_player) {		/* if there is a neighboring stone */
			rec_removeblock (let + 1, num, N, board);		/* call function recursively for that block */
		}
	}
	if (let - 1 >= 0) {
		if (board[num][let - 1] == des_player) {		/* if there is a neighboring stone */
			rec_removeblock (let - 1, num, N, board);		/* call function recursively for that block */
		}
	}
	return 1;
}


int play(char pl, int let, int num, int N, int z, char **board, char ***unboard) {		/* function that implements the play move with input player and block coordinates */
int remsum = 0, i , j, kodifferent = 0;
char player, opponent;
if (pl == 'w') {		/* determine player symbol */
	player = 'O';
	opponent = 'X';
}
else {
	player = 'X';
	opponent = 'O';
}
if (num < 0 || num >= N || let < 0 || let >= N || board[num][let] != '.')		/* if coordinates are not in the board or if the board block is occupied return error */
	return -1;
else
	board[num][let] = player;	/* place the designated player stone */
if (eligibility(pl, let, num, N, board)) {		/* check if input move is eligible */
	if (num + 1 < N) {		
		if (board[num + 1][let] == opponent && !(hasfreedom (let, num + 1, N, board)))		/* check if there needs to be a block removal */
			rec_removeblock (let, num + 1, N, board);
	}
	if (num - 1 >= 0) {
		if (board[num - 1][let] == opponent && !(hasfreedom (let, num - 1, N, board)))		/* check if there needs to be a block removal */
			rec_removeblock (let, num - 1, N, board);
	}
	if (let + 1 < N) {
		if (board[num][let + 1] == opponent && !(hasfreedom (let + 1, num, N, board)))		/* check if there needs to be a block removal */
			rec_removeblock (let + 1, num, N, board);
	}
	if (let - 1 >= 0) {
		if (board[num][let - 1] == opponent && !(hasfreedom (let - 1, num, N, board)))		/* check if there needs to be a block removal */
			rec_removeblock (let - 1, num, N, board);
	}
	if ( z >= 2) {		/* if more than 2 moves have been played */
		for (i = 0; i < N; i++)			/* check for infinite loops (ko ruling) */
			for (j = 0; j < N; j++)
				if (z >= 2 && board[i][j] != unboard[z - 2][i][j])
					kodifferent++;
		if (kodifferent == 0) {
			board[num][let] = '.';		/* if move creates an infinite loop and is prohibited by the ko ruling then put '.' in the block coordinates again */
			return -1;
		}
	}
}
else {		/* if move is not eligible then put '.' in the block coordinates again */
	board[num][let] = '.';
	return -1;
}
return 1;
}


int genmove(char pl, int N, int z, char **board, char ***unboard, char *response) {		/* function that generates a move */
int i = -1, j, success = 0;
while (!success && i < N) {
	i++;
	j = 0;
	while (!success && j < N) {
		if (play(pl, i, j, N, z, board, unboard) == 1)
			success = 1;
		else
			j++;
	}
}
if (success) {		/* if there are possible moves then put move in the response array */
	response[0] = i;
	response[1] = j;
	return 1;
}
else		/* return 0 for pass */
	return 0;
}


void showboard (int N, char **board) {		/* function that shows the current state of the board */
	int i, j, iswitch = 0;
	printf("   ");
	for (i = 0; i < N; i++) {
		if ('A' + i == 'I')
			iswitch = 1;
		putchar('A' + i + iswitch);
		printf(" ");
	}
	printf("\n");
	for (i = N - 1; i >= 0; i--) {
		if (i + 1 < 10)
			printf(" ");
		printf("%d ", i + 1);
		for (j = 0; j < N; j++) 
			printf("%c ", board[i][j]); 
		printf("%d\n", i + 1);
	}
	printf("   ");
	iswitch = 0;
	for (i = 0; i < N; i++) {
		if ('A' + i == 'I')
			iswitch = 1;
		putchar('A' + i + iswitch);
		printf(" ");
	}
}

void commandassignments (char **commands) {		/* function that fills the commands array with the possible commands */
	commands[0]="protocol_version";
	commands[1]="name";
	commands[2]="version";
	commands[3]="known_command";
	commands[4]="list_commands";
	commands[5]="quit";
	commands[6]="play";
	commands[7]="genmove";
	commands[8]="boardsize";
	commands[9]="showboard";
	commands[10]="komi";
	commands[11]="pass";
	commands[12]="clear_board";
	commands[13]="final_score";
	commands[14]="undo";
}


