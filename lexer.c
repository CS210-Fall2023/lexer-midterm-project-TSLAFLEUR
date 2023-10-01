/**
 * @file lexer.c
 * @author Tyler LaFleur
 * @brief 
 * @version 0.1
 * @date 2023-09-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "lexer.h"


int comment(int CT,FILE *fp,FILE *saveTo){
	char current;
	int endOfComment=0;
	do{
		current=fgetc(fp);
		if(current=='\n'&&CT==1){
			return 0;
		}else if(current=='*'&&endOfComment==0){
			endOfComment=1;
		}else if(current=='/'&&endOfComment==1){
			fprintf(saveTo,"%c",current);
			return 0;
		}else{
			endOfComment=0;
		}
		fprintf(saveTo,"%c",current);
	}while(current!=EOF);
	return 0;
}

int string(FILE *fp,FILE *saveTo){
	char current;
	int literal=0;
	do{
		current=fgetc(fp);
		if(current=='"'&&literal==0){
			fprintf(saveTo,"%c",current);
			return 1;
		}else if(current=='\\'){
			literal=1;
		}else if(current=='"'&&literal==1){
			literal=0;
		}else literal=0;
		fprintf(saveTo,"%c",current);
	}while(current!=EOF);
	return 1;
}

int CHAR(FILE *fp,FILE *saveTo){
	char current;
	int literal=0;
	do{
		current=fgetc(fp);
		if(current=='\''&&literal==0){
			fprintf(saveTo,"%c",current);
			return 3;
		}else if(current=='\\'){
			literal=1;
		}else if(current=='\''&&literal==1){
			literal=0;
		}else literal=0;
		fprintf(saveTo,"%c",current);
	}while(current!=EOF);
	return 3;
}

char num(FILE *fp,char first,FILE *saveTo){
	char current,
		 peek=first;
	int usedPeek=1,
		dot=0;
	do{
		if(usedPeek==0){
			current=fgetc(fp);
		}else if(usedPeek==1){
			current=peek;
			usedPeek=0;
		}
		if(current=='.'&&dot==0){
			dot=1;
			peek=fgetc(fp);
			if(peek=='.'){
				fprintf(saveTo,"%s"," (numeric literal)\n");
				fprintf(saveTo,"..");
				fprintf(saveTo,"%s"," (operator)\n");
				return '.';
			}else{
				fprintf(saveTo,"%c",current);
			}
		}else if(current=='0'||current=='1'||current=='2'||current=='3'||current=='4'||current=='5'||current=='6'||current=='7'||current=='8'||current=='9'||current=='#'){
			fprintf(saveTo,"%c",current);
		}else{
			fprintf(saveTo,"%s"," (numeric literal)\n");
			return current;
		}
	}while(current!=EOF);

	fprintf(saveTo,"%s"," (numeric literal)\n");
	return current;
}

char keyword(FILE *fp,char first,char**keywords,FILE *saveTo){
	char word[256];
	word[0]=first;
	char current;
	int count=1;
	do{
		current=fgetc(fp);
		if((current>=48&&current<=57)||(current>=65&&current<=90)||(current>=97&&current<=122)||(current==95)){
			word[count]=current;
			count++;
		}else break;
	}while(current!=EOF&&count!=256);
	word[count]='\0';
	fprintf(saveTo,"%s",word);
	for(int i=0;i<37;i++){
		if(strcmp(word,keywords[i])==0){
			fprintf(saveTo,"%s"," (keyword)\n");
			return current;
		}
	}
	fprintf(saveTo,"%s"," (identifier)\n");
	return current;
}