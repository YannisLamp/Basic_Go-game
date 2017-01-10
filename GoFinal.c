/*Go game. Konstantinos Stefanidis (stef), Giannis Lamprou (koropi). Athens 2015*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gofunctions.h"

int main (int argc, char *argv[]) {

int c1 = 0, c2 = 0, N = 0, end = 0, i, j, let, num, iminus, addscore = 0, m, found, x, y, z = 1;
float komi, scoreblack = 0, scorewhite = 0;
char *order, *val, **board, ch, color[6], **commands, player, response[2], ***unboard;
val = malloc (6 * sizeof(char));
if (val == NULL) {		
	fprintf(stderr, "Memory allocation for val unsuccessful\n\n");
	return -1;
}
order = malloc(17 * sizeof(char));
if (order == NULL) {		
	fprintf(stderr, "Memory allocation for order unsuccessful\n\n");		
	return -1;
}
commands = malloc(15*sizeof(char *));
if (commands == NULL) {		
	fprintf(stderr, "Memory allocation for command board unsuccessful\n\n");									
	return -1;
}
for (i = 0; i < 14; i++){
	commands[i] = malloc(17 * sizeof(char));
	if (order == NULL) {		
		fprintf(stderr, "Memory allocation for command board unsuccessful\n\n");									
	return -1;
	}
}
commandassignments(commands);
while (end != 2) {
	ch = getchar();
	while (isspace(ch)) {
		ch = getchar();
	}										/*skip comments starting with #*/
	if (ch == '#') {
		while (ch != '\n')
			ch = getchar();
	}
	else{
		ungetc(ch, stdin);	
	}
	scanf("%s", order);
	m = 0;
	while (order[m]){
	order[m] = tolower(order[m]);
	m++;
	}
	if (!strcmp(order, "play")) {
			scanf("%s", color);
			if (!strcmp(color, "black")) {
				player = 'b';
			}
			else if (!strcmp(color, "white")) {
				player = 'w';
			}
		scanf("%s", val);		/*scan the vertex of the move*/
		let = (int)(toupper(val[0]));	/*assign the ascii code of the letter and the number to the proper variables*/
		iminus = 0;
		if (let > 'I')					
		iminus = 1;	
		let = (let - 'A' - iminus);		/*skip the I if */		
		num = atoi(&val[1]) - 1;
		end = 0;
		if (play (player, let, num, N, z, board, unboard) != -1) { /*if the position is legit make a copy of the board to undo board*/
			for (i = 0; i < N; i++)
				for (j = 0; j < N; j++)
					unboard[z][i][j] = board[i][j];
			unboard = realloc(unboard, (++z + 1) * sizeof(char **));
			if (unboard == NULL)		
				fprintf(stderr, "Memory reallocation for undo board unsuccessful\n\n");
			unboard[z] = malloc(N * sizeof(char *));
			if (unboard[z] == NULL)		
				fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
			for(i = 0; i < N; i++){
				unboard[z][i] = malloc(N * sizeof(char));
				if (unboard[z][i] == NULL)
					fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
			}
			printf("= \n\n");
		}
		else
		printf("?illegal move\n"); /*not legit move, print error message*/
	}
	else if (!strcmp(order, "genmove")) {
		scanf("%s", color);
			if (!strcmp(color, "black")) {
				player = 'b';
			}
			else if (!strcmp(color, "white")) {
				player = 'w';
			}
		if (genmove(player, N, z, board, unboard, response)) {
			for (i = 0; i < N; i++)
				for (j = 0; j < N; j++)
					unboard[z][i][j] = board[i][j];
			unboard = realloc(unboard, (++z + 1) * sizeof(char **));
			if (unboard == NULL)		
				fprintf(stderr, "Memory reallocation for undo board unsuccessful\n\n");
			unboard[z] = malloc(N * sizeof(char *));
			if (unboard[z] == NULL)		
				fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
			for(i = 0; i < N; i++){
				unboard[z][i] = malloc(N * sizeof(char));
				if (unboard[z][i] == NULL)
					fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
			}
			end = 0;
			printf("= %c%d\n\n", 'A'+ response[0] , response[1] + 1);
		}
		else {		/* if there are no possible moves left then pass */
			for (i = 0; i < N; i++)
				for (j = 0; j < N; j++)
					unboard[z][i][j] = board[i][j];
			unboard = realloc(unboard, (++z + 1) * sizeof(char **));
			if (unboard == NULL)		
				fprintf(stderr, "Memory reallocation for undo board unsuccessful\n\n");
			unboard[z] = malloc(N * sizeof(char *));
			if (unboard[z] == NULL)		
				fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
			for(i = 0; i < N; i++){
				unboard[z][i] = malloc(N * sizeof(char));
				if (unboard[z][i] == NULL)
					fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
			}
			end++;
			printf("= PASS\n\n");
		}
	}
	else if (!strcmp(order , "pass")) { /*just make a copy of the board to undo board*/
		for (i = 0; i < N; i++)
				for (j = 0; j < N; j++)
					unboard[z][i][j] = board[i][j];
		unboard = realloc(unboard, (++z + 1) * sizeof(char **));
		if (unboard == NULL)		
			fprintf(stderr, "Memory reallocation for undo board unsuccessful\n\n");
		unboard[z] = malloc(N * sizeof(char *));
		if (unboard[z] == NULL)		
			fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
		for(i = 0; i < N; i++){
			unboard[z][i] = malloc(N * sizeof(char));
			if (unboard[z][i] == NULL)
				fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
		}
		end++;
		printf("= \n\n");		
	}
	else if (!strcmp(order , "boardsize")){ /*allocate memory for the board*/
		scanf("%s", val);
		N = atoi(val);
		board = malloc(N * sizeof(char*));
		if (board == NULL) {		
			fprintf(stderr, "Memory allocation for board unsuccessful");
			return -1;
		}
		for(i = 0; i < N; i++)
		board[i] = malloc(N * sizeof(char));
		if (board == NULL) {		
			fprintf(stderr, "Memory allocation for board unsuccessful");
			return -1;
		}
		for (i = 0; i < N; i++){
			for (j = 0; j < N; j++){
				board[i][j] = '.';
			}
		}
		unboard = malloc(2 * sizeof(char **));
		if (unboard == NULL)
			fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
		for(z=0; z<2; z++){
			unboard[z] = malloc(N * sizeof(char *));
			if (unboard[z] == NULL)		
				fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
			for(i = 0; i < N; i++){
				unboard[z][i] = malloc(N * sizeof(char));
				if (unboard[z][i] == NULL)
					fprintf(stderr, "Memory allocation for undo board unsuccessful\n\n");
			}
		}
		for (i = 0; i < N; i++)
			for (j = 0; j<N; j++)
				unboard[0][i][j] = '.';
		z = 1;
		printf("= \n\n");
	}
	else if (!strcmp(order, "showboard")){ 
		printf("= \n");
		showboard (N, board);
		printf("\n\n");
	}
	else if (!strcmp(order, "komi")){
		scanf("%f", &komi);
		printf("= \n\n");
	}
	else if (!strcmp(order, "protocol_version")){
		printf("= 2\n\n");
	}
	else if (!strcmp(order, "name")){
		printf("= takis4\n\n");	
	}
	else if (!strcmp(order, "version")){
		printf("= 224\n\n");
	}
	else if (!strcmp(order, "known_command")){ /*just a linear search*/
		scanf("%s", order);
		found = 0;
		for (i = 0; i < 13; i++){
			if (!strcmp(order, commands[i])){
				found = 1;
				break;
			}
		}
		if (found == 1){
			printf("= true\n\n");
		}
		else{
			printf("= false\n\n");
		}
	}
	else if (!strcmp(order, "list_commands")){
		printf("= \n");
		for (i = 0; i < 13; i++)
			printf("%s\n", commands[i]);
		printf("\n\n");
	}
	else if (!strcmp(order, "quit")){
		printf("= \n\n");
		return 0;
	}
	else if (!strcmp(order, "clear_board")){
		for (i = 0; i < N; i++){
			for (j = 0; j < N; j++){
				board[i][j] = '.';
			}
		}
		printf("= \n\n");
	}
	else if (!strcmp(order, "final_score")) { /*count the stones for each player an add komi*/
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++) {
				if (board[i][j] == 'X')
					scoreblack++;
				if (board[i][j] == 'O')
					scorewhite++;
			}
			scorewhite = scorewhite + komi;
			if (scoreblack > scorewhite)
				printf("= B+%f\n\n", scoreblack - scorewhite);
			else if (scoreblack < scorewhite)
				printf("= W+%f\n\n", scorewhite - scoreblack);
			else
				printf("= 0\n\n");
	}
	else if (!strcmp(order, "undo")){ /*copy the previous layer of the undo board to board*/
		if(z == 1){
			printf("?undo impossible\n");
			continue;
		}
		for (i = 0; i < N; i++){
				for (j = 0; j < N; j++)
					board[i][j] = unboard[z - 2][i][j];
				free(unboard[z][i]);
		}
		free(unboard[z--]);				
		printf("= \n\n");
	}
	fflush(stdout);
}
while (strcmp(order, "quit")) {	/*when two consecutive "pass" occur, calculate the score or quit*/ 
	scanf("%s", order);
	if (!strcmp(order, "final_score")) {
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++) {
				if (board[i][j] == 'X')
					scoreblack++;
				if (board[i][j] == 'O')
					scorewhite++;
			}
			scorewhite = scorewhite + komi;
			if (scoreblack > scorewhite)
				printf("= B+%f\n\n", scoreblack - scorewhite);
			else if (scoreblack < scorewhite)
				printf("= W+%f\n\n", scorewhite - scoreblack);
			else
				printf("= 0\n\n");
	}
}
printf("= \n\n");
free(val);
free(order);
for (i = 0; i < 14; i++)
	free(commands[i]);
free(commands);
for (j = 0; j <= z; j++) {
	for (i = 0; i < N; i++) {
		free(unboard[j][i]);
	}
	free(unboard[j]);
}
free(unboard);
for (i=0; i<N; i++)
	free(board[i]);
free(board);

}
