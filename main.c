/**
 * @file main.c
 * @author Tyler LaFleur
 * @brief This program takes a file and outputs a file of the same name postfixed with .lexer
 * 		  The file will be broken down in 8 types of lexemes: comments, strings, keywords, character literals, operators, numeric literals, identifiers, and UNK
 * 		  Each lexeme will appear on a separate line in the file with the lexeme followed by the type of lexeme in paranthesis afterwards.
 * @version 0.1
 * @date 2023-09-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "lexer.h"

int main(int argc, char* argv[]){
	//source and desination files
	FILE *fp,*saveTo;
	//set destination file's name
	char fileName[256];
	strncpy(fileName,argv[1],256);
	strncat(fileName,".lexer",7);
	//open files for reading and writing
	fp=fopen(argv[1],"r");
	saveTo=fopen(fileName,"w");
	//necessary variables
	char current, //where file pointer is currently reading and writing
		 peek; //one character ahead to check for necessary lexemes and to be used to read character that may not have been printed yet
	int usedPeek=0;	//flag to use peek char instead of reading new char
	char * lexemeType[8]={  " (comment)\n",			//0.	/* * /  or //
							" (string)\n",				//1.	" "
							" (keyword)\n",			/*2.	accessor and array begin bool case character constant else elsif end exit function
															if in integer interface is loop module mutator natural null of or other out
															positive procedure range return struct subtype then type when while */
							" (character literal)\n",	//3.	' '
							" (operator)\n",			/*4.	“.”   “<”   “>”   “(“   “)”   “+”   “-“   “*”   “/”   “|”   “&”   “;”   “,”   “:”
															“[“   “]”  “=”   “:=”   “..”   “<<”   “>>”   “<>”   “<=”   “>=”   “**”   “!=”   “=>” */
							" (numeric literal)\n",	/*5.	* decimal digits (0 through 9)
															* the special characters ‘.’ (only one) and ‘#’(can have multiple, like 3#5#) */
							" (identifier)\n",			//6.	starts with char, no more than 256 char long
							" (UNK)\n"	};				//7.	should not be any during actual runtime
	char * keywords[37]={"accessor","and","array","begin","bool","case","character","constant","else","elsif",
					   "end","exit","function","if","in","integer","interface","is","loop","module",
					   "mutator","natural","null","of","or","other","out","positive","procedure","range",
					   "return","struct","subtype","then","type","when","while"};
	do{
		if(usedPeek==0){ //normal instance; read new character from input file and analyze
			current=fgetc(fp);
		}else if(usedPeek==1){ //in case peek char was not printed to file yet and still needs to be analysed
			current=peek;
			usedPeek=0;
		}
		//covers any whitespace characters
		if(current==EOF){
			break;
		}
		else if(current==' '||current=='\n'){}
		//covers type 0 and part of type 4
		else if(current=='/'){
			peek=fgetc(fp);
			fprintf(saveTo,"%c",current);
			if(peek=='/'){ // "//"single line comment
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[comment(1,fp,saveTo)]);
			}else if (peek=='*'){ // "/*" multi line comment
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[comment(2,fp,saveTo)]);
			}else{ // '/' operator
				fprintf(saveTo,"%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		//covers type 1 
		else if(current=='"'){ //strings
			fprintf(saveTo,"%c",current);
			fprintf(saveTo,"%s",lexemeType[string(fp,saveTo)]);
		}
		//covers type 3
		else if(current=='\''){ //char literals
			fprintf(saveTo,"%c",current);
			fprintf(saveTo,"%s",lexemeType[CHAR(fp,saveTo)]);
		} 
		//covers type 4
		//single character operators
		else if(current=='('||current==')'||current=='+'||current=='-'||current=='|'||current=='&'||current==';'||current==','||current=='['||current==']'){
			fprintf(saveTo,"%c",current);
			fprintf(saveTo,"%s",lexemeType[4]);
		}
		else if(current==':'){
			peek=fgetc(fp);
			fprintf(saveTo,"%c",current);
			if(peek=='='){ // ":=" operator
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[4]);
			}else{ // ':' operator
				fprintf(saveTo,"%s",lexemeType[4]);
				usedPeek=1;
			}
			
		}
		else if(current=='.'){
			peek=fgetc(fp);
			fprintf(saveTo,"%c",current);
			if(peek=='.'){ // ".." operator
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[4]);
			}else{ // '.' operator
				fprintf(saveTo,"%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		else if(current=='<'){
			peek=fgetc(fp);
			fprintf(saveTo,"%c",current);
			if(peek=='<'){ // "<<" operator
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[4]);
			}else if(peek=='>'){ // "<>" operator
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[4]);
			}else if(peek=='='){ // "<=" operator
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[4]);
			}else{ // '<' operator
				fprintf(saveTo,"%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		else if(current=='>'){
			peek=fgetc(fp);
			fprintf(saveTo,"%c",current);
			if(peek=='>'){ //">>" operator
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[4]);
			}else if(peek=='='){ //">= operator"
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[4]);
			}else{ //'>' operator
				fprintf(saveTo,"%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		else if(current=='*'){
			peek=fgetc(fp);
			fprintf(saveTo,"%c",current);
			if(peek=='*'){ //"**" operator
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[4]);
			}else{ //'*' operator
				fprintf(saveTo,"%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		else if(current=='='){
			peek=fgetc(fp);
			fprintf(saveTo,"%c",current);
			if(peek=='>'){ //"=>" operator
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[4]);
			}else{ //'=' operator
				fprintf(saveTo,"%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		else if(current=='!'){
			peek=fgetc(fp);
			fprintf(saveTo,"%c",current);
			if(peek=='='){ //"!=" operator
				fprintf(saveTo,"%c",peek);
				fprintf(saveTo,"%s",lexemeType[4]);
			}else{ //'!' operator
				fprintf(saveTo,"%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		//covers type 5
		else if(current=='0'||current=='1'||current=='2'||current=='3'||current=='4'||current=='5'||current=='6'||current=='7'||current=='8'||current=='9'){
			peek=num(fp,current,saveTo);
			if(peek=='.'){ // in the case of ".."
				usedPeek=0;
			}else usedPeek=1;
		}
		//covers parts 2 and 6
		else if(current=='a'||current=='b'||current=='c'||current=='d'||current=='e'||current=='f'||current=='g'||current=='h'||current=='i'||current=='j'||current=='k'||current=='l'||current=='m'||
				current=='n'||current=='o'||current=='p'||current=='q'||current=='r'||current=='s'||current=='t'||current=='u'||current=='v'||current=='w'||current=='x'||current=='y'||current=='z'||
				current=='A'||current=='B'||current=='C'||current=='D'||current=='E'||current=='F'||current=='G'||current=='H'||current=='I'||current=='J'||current=='K'||current=='L'||current=='M'||
				current=='N'||current=='O'||current=='P'||current=='Q'||current=='R'||current=='S'||current=='T'||current=='U'||current=='V'||current=='W'||current=='X'||current=='Y'||current=='Z'){
			peek=keyword(fp,current,keywords,saveTo);
			usedPeek=1;
		}
		//covers part 7
		else {
			fprintf(saveTo,"%c",current);
			fprintf(saveTo,"%s",lexemeType[7]);
		}
	}while(current!=EOF);
	fclose(fp);
	fclose(saveTo);
	return 0;
}