/**
 * @file lexer.c
 * @author Tyler LaFleur
 * @brief This file contains function definitions from lexer.h
 * @version 0.1
 * @date 2023-09-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "lexer.h"

/**
 * @brief This function prints comment lexemes
 * 
 * @param CT comment type: 1 = single line, 2 = multiline
 * @param fp source file
 * @param saveTo destination file
 * @return int returns the lexeme type
 */
int comment(int CT,FILE *fp,FILE *saveTo){
	char current;
	int endOfComment=0;
	do{
		current=fgetc(fp);
		if(current=='\n'&&CT==1){//single line comment
			return 0;
		}else if(current=='*'&&endOfComment==0){//multiline comment warning flag
			endOfComment=1;
		}else if(current=='/'&&endOfComment==1){//multiline comment finished
			fprintf(saveTo,"%c",current);
			return 0;
		}else{//reset multiline comment warning flag if it was up
			endOfComment=0;
		}
		fprintf(saveTo,"%c",current);
	}while(current!=EOF);
	return 0;
}

/**
 * @brief This function prints string lexemes
 * 
 * @param fp source file
 * @param saveTo destination file
 * @return int returns the lexeme type
 */
int string(FILE *fp,FILE *saveTo){
	char current;
	int literal=0;
	do{
		current=fgetc(fp);
		if(current=='"'&&literal==0){//if not '\"', end string
			fprintf(saveTo,"%c",current);
			return 1;
		}else if(current=='\\'){//pop flag if current is '\' because next character may be '"'
			literal=1;
		}else if(current=='"'&&literal==1){//continue string because previous character was '\'
			literal=0;
		}else literal=0;//if previous character was '\' but was not followed by '"'
		fprintf(saveTo,"%c",current);
	}while(current!=EOF);
	return 1;
}

/**
 * @brief This function prints out character literal lexeme types
 * 
 * @param fp source file
 * @param saveTo destination file
 * @return int return the lexeme type
 */
int CHAR(FILE *fp,FILE *saveTo){
	char current;
	int literal=0;
	do{
		current=fgetc(fp);
		if(current=='\''&&literal==0){//if not '\''
			fprintf(saveTo,"%c",current);
			return 3;
		}else if(current=='\\'){//pop flag is current is '\' because next character may be '''
			literal=1;
		}else if(current=='\''&&literal==1){//continue printing character literal because previous character was '\'
			literal=0;
		}else literal=0;//drop flag
		fprintf(saveTo,"%c",current);
	}while(current!=EOF);
	return 3;
}

/**
 * @brief This function prints out numeric literal lexeme types
 * 
 * @param fp source file
 * @param first starting character of numeric literal
 * @param saveTo destination file
 * @return char the current character that source file pointer is on
 */
char num(FILE *fp,char first,FILE *saveTo){
	char current,
		 peek=first;
	int usedPeek=1,
		dot=0;
	do{
		if(usedPeek==0){
			current=fgetc(fp);
		}else if(usedPeek==1){//if we had read the next character, but did not print it out, set it to current instead of pulling a new character
			current=peek;
			usedPeek=0;
		}
		if(current=='.'&&dot==0){//check to see if float number or if ..
			dot=1;
			peek=fgetc(fp);
			if(peek=='.'){//currently printing out ..
				fprintf(saveTo,"%s"," (numeric literal)\n");
				fprintf(saveTo,"..");
				fprintf(saveTo,"%s"," (operator)\n");
				return '.';
			}else{//only printing out single . for float value
				fprintf(saveTo,"%c",current);
			}
		//if 0-9 or #
		}else if(current=='0'||current=='1'||current=='2'||current=='3'||current=='4'||current=='5'||current=='6'||current=='7'||current=='8'||current=='9'||current=='#'){
			fprintf(saveTo,"%c",current);
		}else{//if non valid character, print out the lexeme type and return the current character
			fprintf(saveTo,"%s"," (numeric literal)\n");
			return current;
		}
	}while(current!=EOF);

	fprintf(saveTo,"%s"," (numeric literal)\n");
	return current;
}

/**
 * @brief This function reads to check if the current word is a keyword or identifier
 * 
 * @param fp source file
 * @param first first character in word
 * @param keywords array of strings of keywords
 * @param saveTo destination file
 * @return char return current character
 */
char keyword(FILE *fp,char first,char**keywords,FILE *saveTo){
	char word[256];
	word[0]=first;//set first element of array to what was previously read
	char current;
	int count=1;
	do{
		current=fgetc(fp);
		if((current>=48&&current<=57)||(current>=65&&current<=90)||(current>=97&&current<=122)||(current==95)){//if 0-9, A-Z, a-z, or _ add to word
			word[count]=current;
			count++;
		}else break;//finished adding to word
	}while(current!=EOF&&count!=256);
	word[count]='\0';//cut off word string at the end of the current string
	fprintf(saveTo,"%s",word);//print out the word
	for(int i=0;i<37;i++){//check if word is part of keyword list
		if(strcmp(word,keywords[i])==0){//if keyword, then print out keyword lexeme type
			fprintf(saveTo,"%s"," (keyword)\n");
			return current;
		}
	}
	fprintf(saveTo,"%s"," (identifier)\n");//if not keyword, then print out identifer lexeme type
	return current;
}