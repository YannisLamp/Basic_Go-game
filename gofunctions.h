/*Go game. Konstantinos Stefanidis (stef), Giannis Lamprou (koropi). Athens 2015*/
int eligibility(char, int, int, int, char **);
int hasfreedom(int, int, int, char **);
int rec_hasfreedom(int, int, int, char **);
int rec_removeblock(int, int, int, char **);
int play(char, int, int, int, int, char **, char ***);
int genmove(char, int, int, char **, char ***, char *);
void showboard (int , char **);
void commandassignments (char **);
