#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "getLine.h"

typedef struct node* trie; 

typedef struct node {
	trie** trieptrs;
	int count;
	bool word;
}node;



void loadtrie(trie* root, char* word){
	trie* next;
	// printf("Loading: %s\n", word);

	if ((*root) == NULL){ //creates a new node
		// printf("root is null\n");
		(*root) = calloc(1, sizeof(node));
		(*root)->word = false;
		(*root)->count = 0;
		(*root)->trieptrs = calloc(26, sizeof(trie*));
		// printf("survived creation\n");
	}
	// printf("Word[0] is %c\n", word[0] + 1);

	if (word[0] == '\0' ) {//reached null terminated end of string, boolean word = 1
		(*root)->word = true;
		// printf("word is null: %s\n", word);
	}
	else if ( (*root)->trieptrs[word[0] - 97] == NULL ){
		// printf("creating new trie\n");
		next = calloc(1, sizeof(trie));
		(*next) = NULL; // set it to null so it can recursively load trie and make a new word
		(*root)->trieptrs[word[0] - 97] = next;
		loadtrie(next, word + 1);
	}
	else{
		loadtrie((*root)->trieptrs[word[0] - 97], word + 1);
	}
}

void loadDict(trie* root){
	char* word;
	while ( (word = getLine(stdin)) != NULL ){
		// printf("Word is %s\n", word);
		bool valid = true;
		int len = strlen(word);
		word[len - 1] = '\0';
		// printf("length of word is %d\n", len);
		for (int i = 0; i < len ; i++) { //checks if all characters are alphanumerical
			if (!isalpha(word[i]) && word[i] != '\0') {
				valid = false;
			}
			// printf("i is %d\n", i);
			// printf("valid is %d\n", valid);
		}
		// printf("Word was valid; %s\n",word);
		if (!valid){
			free(word);
			continue;
		}
		else{
			for (int i = 0; word[i]; i++){
				word[i] = tolower(word[i]);
			}
			// printf("Lowercaseword is: %s\n", word);
			loadtrie(root, word);
			free(word);
		}

	}

}

void printTrie(trie *t, char *printword, int depth, int cflag) {
	if(cflag) {
		if((*t)->count == 0 && (*t)->word) {
			printf("%s\n", printword);
		}
	}
	else {
		if ((*t)->count > 0) {
			printf("%s: %d\n", printword, (*t)->count);
		}
	}
	for(int i = 0; i < 26; i++) {
		if((*t)->trieptrs[i] != NULL) {
			char *wordcopy = malloc( sizeof(char) * (depth + 1));
			for(int j = 0; j < depth; j++) {
				wordcopy[j] = printword[j];
			}
			wordcopy[depth] = '\0'; //null terminate
			wordcopy[depth - 1] = i + 97;
			//printf("letter: %c word: %s copy: %s\n", i + 97, printword, wordcopy);
			printTrie((*t)->trieptrs[i], wordcopy, depth + 1, cflag);
		}
	}
}


void twodimension(int rows, int cols, char* letters, char board[rows][cols])
{ //converts an array into a 2d matrix
	int i, j;
	for (i = 0; i < rows; i++){
		for (j = 0; j < cols; j++){
			board[i][j] = letters[i*cols + j];
		}
	}
}

void traversegrid(trie* t, int rows, int cols, int tflag, int i, int j, char current, char board[rows][cols], \
	bool visited[rows][cols]){
	//printf("\ncurrent char is %c\n", current);
	char next;

	if (tflag && visited[i][j]){
		return; //if t flag is specified and you've seen the tile already
	}

	if (current == '_'){//reiterate this thing for all of the possible characters
		for (int k = 0; k < 26; k++){
			traversegrid((*t)->trieptrs[k], rows, cols, tflag, i, j, k + 97, board, visited);
		}
		return;
	}
	if (t == NULL){//letter is not in the trie, go back up
		//printf("didn't find the char\n");
		return;
	}
	current = tolower(current);
	
	if ((*t)->word) {//reached end of a word, increment the count
		(*t)->count += 1;
		//printf("found the word!\n");
	}
	
	if (i - 1 >= 0){//up
		visited[i][j] = true;
		next = tolower(board[i - 1][j]);
		if (next == '_'){
			for (int k = 0; k < 26; k++){
				visited[i][j] = true;
				traversegrid((*t)->trieptrs[k], rows, cols, tflag, i - 1, j, k + 97, board, visited);
				visited[i][j] = false;
			}
		}
		else{
			traversegrid((*t)->trieptrs[next - 97], rows, cols, tflag, i - 1, j, next, board, visited);
			visited[i][j] = false;
		}
	}
	if (i + 1 < rows){//down
		visited[i][j] = true;
		next = tolower(board[i + 1][j]);
		if (next == '_'){
			for (int k = 0; k < 26; k++){
				visited[i][j] = true;
				traversegrid((*t)->trieptrs[k], rows, cols, tflag, i + 1, j, k + 97, board, visited);
				visited[i][j] = false;
			}
		}
		else{
			traversegrid((*t)->trieptrs[next - 97], rows, cols, tflag, i + 1, j, next, board, visited);
			visited[i][j] = false;
		}
	}
	if (j - 1 >= 0){//left
		visited[i][j] = true;
		next = tolower(board[i][j - 1]);
		if (next == '_'){
			for (int k = 0; k < 26; k++){
				visited[i][j] = true;
				traversegrid((*t)->trieptrs[k], rows, cols, tflag, i, j - 1, k + 97, board, visited);
				visited[i][j] = false;
			}
		}
		else{
			traversegrid((*t)->trieptrs[next - 97], rows, cols, tflag, i, j - 1, next, board, visited);
			visited[i][j] = false;
		}
	}
	if (j + 1 < cols){//right
		visited[i][j] = true;
		next = tolower(board[i][j + 1]);
		if (next == '_'){
			for (int k = 0; k < 26; k++){
				visited[i][j] = true;
				traversegrid((*t)->trieptrs[k], rows, cols, tflag, i, j + 1, k + 97, board, visited);
				visited[i][j] = false;
			}
		}
		else{
			traversegrid((*t)->trieptrs[next - 97], rows, cols, tflag, i, j + 1, next, board, visited);
			visited[i][j] = false;
		}
	}
	if (i - 1 >= 0 && j - 1 >= 0){ //upleft
		visited[i][j] = true;
		next = tolower(board[i - 1][j - 1]);
		if (next == '_'){
			for (int k = 0; k < 26; k++){
				visited[i][j] = true;
				traversegrid((*t)->trieptrs[k], rows, cols, tflag, i - 1, j - 1, k + 97, board, visited);
				visited[i][j] = false;
			}
		}
		else{
			traversegrid((*t)->trieptrs[next - 97], rows, cols, tflag, i - 1, j - 1, next, board, visited);
			visited[i][j] = false;
		}
	}
	if (i - 1 >= 0 && j + 1 < cols){ //upright
		visited[i][j] = true;
		next = tolower(board[i - 1][j + 1]);
		if (next == '_'){
			for (int k = 0; k < 26; k++){
				visited[i][j] = true;
				traversegrid((*t)->trieptrs[k], rows, cols, tflag, i - 1, j + 1, k + 97, board, visited);
				visited[i][j] = false;
			}
		}
		else{
			traversegrid((*t)->trieptrs[next - 97], rows, cols, tflag, i - 1, j + 1, next, board, visited);
			visited[i][j] = false;
		}
	}
	if (i + 1 < rows && j - 1 >= 0){ //downleft
		visited[i][j] = true;
		next = tolower(board[i + 1][j - 1]);
		if (next == '_'){
			for (int k = 0; k < 26; k++){
				visited[i][j] = true;
				traversegrid((*t)->trieptrs[k], rows, cols, tflag, i + 1, j - 1, k + 97, board, visited);
				visited[i][j] = false;
			}
		}
		else{
			traversegrid((*t)->trieptrs[next - 97], rows, cols, tflag, i + 1, j - 1, next, board, visited);
			visited[i][j] = false;
		}
	}
	if (i + 1 < rows && j + 1 < cols){ //downright
		visited[i][j] = true;
		next = tolower(board[i + 1][j + 1]);
		if (next == '_'){
			for (int k = 0; k < 26; k++){
				visited[i][j] = true;
				traversegrid((*t)->trieptrs[k], rows, cols, tflag, i + 1, j + 1, k + 97, board, visited);
				visited[i][j] = false;
			}
		}
		else{
			traversegrid((*t)->trieptrs[next - 97], rows, cols, tflag, i + 1, j + 1, next, board, visited);
			visited[i][j] = false;
		}
		
	}
	return;


}
	//up
	//down
	//left
	//right
	//upright

int main( int argc, char * argv[] )
{
	int rows = 1;
	int cols = 1;
	int cflag = 0;
	int tflag = 0;
	char* letters;
	//check command line first
	if (argc == 4){
		rows = atoi(argv[1]);
		cols = atoi(argv[2]);
		letters = argv[3];
		if (rows < 1 || cols < 1 || strlen(letters) != (rows * cols)){
			fprintf(stderr, " Invalid dimensions to Boggle: %s\n", strerror (errno));
			exit(10);
		}
	}
	else if(argc == 5) {
		rows = atoi(argv[2]);
		cols = atoi(argv[3]);
		letters = argv[4];
		if (strcmp(argv[1], "-t") == 0){
			tflag = 1;
		}
		else if (strcmp(argv[1], "-c") == 0){
			cflag = 1;
		}
		else {
			fprintf(stderr, " Invalid flags to Boggle: %s\n", strerror (errno));
			exit(10);
		}
		if (rows < 1 || cols < 1 || strlen(letters) != (rows * cols)){
			fprintf(stderr, " Invalid dimensions to Boggle: %s\n", strerror (errno));
			exit(10);
		}

	}
	else if(argc == 6) {
		rows = atoi(argv[3]);
		cols = atoi(argv[4]);
		letters = argv[5];
		if ( (strcmp(argv[1], "-t") == 0) && (strcmp(argv[2], "-c") == 0) )
		{
			tflag = 1;
			cflag = 1;
		}
		else if ( (strcmp(argv[1], "-c") == 0) && (strcmp(argv[2], "-t") == 0) )
		{
			cflag = 1;
			tflag = 1;
		}
		else {
			fprintf(stderr, " Invalid flags to Boggle: %s\n", strerror (errno));
			exit(10);
		}
		if (rows < 1 || cols < 1 || strlen(letters) != (rows * cols)){
			fprintf(stderr, " Invalid dimensions to Boggle: %s\n", strerror (errno));
			exit(10);
		}

	}
	else{
		fprintf(stderr, " Invalid args to Boggle: %s\n", strerror (errno));
		exit(10);
	}

	char board[rows][cols];
	bool visited[rows][cols];
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			visited[i][j] = false;
		}
	}

	int len = strlen(letters);
	for (int i = 0; i < len; i++){
		if (!isalpha(letters[i]) && letters[i] != '\0' && letters[i] != '_'){
			fprintf(stderr, " Illegal Characters on the Board: %s\n", strerror (errno));
			exit(10);
		}

	}

	twodimension(rows, cols, letters, board);

	// printf("the letters are %s\n", letters);
	// for (int i = 0; i < rows; i++){
	// 	for (int j = 0; j < cols; j++){
	// 		printf("%c", board[i][j]);
	// 	}
	// 	printf("\n");
	// }


	trie dictionary = NULL;
	loadDict(&dictionary);
	if (dictionary == NULL){
		exit(0);
	}
	char starttile;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			starttile = board[i][j];
			if (starttile == '_'){
				traversegrid(&dictionary, rows, cols, tflag, i, j, starttile, board, visited);
			}
			else{
				traversegrid((dictionary->trieptrs[tolower(board[i][j]) - 97]), rows, cols, tflag, i, j, starttile, board, visited);
			}
		}
	}



	char* key = calloc(15, sizeof(char));

	printTrie(&dictionary, key, 1, cflag);

	exit(0);



}
