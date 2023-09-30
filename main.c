/**
 * @file main.c
 * @author Tyler LaFleur
 * @brief 
 * @version 0.1
 * @date 2023-09-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "lexer.h"

int main(int argc, char* argv[]){
	/*
	Build your lexer incrementally. For example, start by reading a file and printing its context to the screen. 
	Then build a lexer that recognizes just comments and prints the other words (group of characters separated by white space) with UNK state. 
	Then add an ability to recognize the keywords. 
	Once it has been tested, add the ability to recognize CCX operators. 
	Continue in this fashion until your lexer is complete. 
	*/
	FILE *fp,*saveTo;
	char fileName[256];
	strncpy(fileName,argv[1],256);
	strncat(fileName,".lexer",6);
	fp=fopen(argv[1],"r");
	saveTo=fopen(fileName,"w");
	char current,
		 peek;

	int usedPeek=0;
		
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
	char * keywords[37]={"accessor","and","array","begin","bool","case","character","constant","else","elseif",
					   "end","exit","function","if","in","integer","interface","is","loop","module",
					   "mutator","natural","null","of","or","other","out","positive","procedure","range",
					   "return","struct","subtype","then","type","when","while"};
	do{
		if(usedPeek==0){
			current=fgetc(fp);
		}else if(usedPeek==1){
			current=peek;
			usedPeek=0;
		}
		//covers any whitespace characters
		if(current==EOF){
			break;
		}
		else if(current==' '||current=='\n'){}
		//covers part 0 and part of part 4
		else if(current=='/'){
			peek=fgetc(fp);
			printf("%c",current);
			if(peek=='/'){
				printf("%c",peek);
				printf("%s",lexemeType[comment(1,fp)]);
			}else if (peek=='*'){
				printf("%c",peek);
				printf("%s",lexemeType[comment(2,fp)]);
			}else{
				printf("%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		//covers part 1
		else if(current=='"'){
			printf("%c",current);
			printf("%s",lexemeType[string(fp)]);
		}
		//covers part 3
		else if(current=='\''){
			printf("%c",current);
			printf("%s",lexemeType[CHAR(fp)]);
		} 
		//covers part 4
		else if(current=='('||current==')'||current=='+'||current=='-'||current=='|'||current=='&'||current==';'||current==','||current=='['||current==']'){
			printf("%c",current);
			printf("%s",lexemeType[4]);
		}
		else if(current==':'){
			peek=fgetc(fp);
			printf("%c",current);
			if(peek=='='){
				printf("%c",peek);
				printf("%s",lexemeType[4]);
			}else{
				printf("%s",lexemeType[4]);
				usedPeek=1;
			}
			
		}
		else if(current=='.'){
			peek=fgetc(fp);
			printf("%c",current);
			if(peek=='.'){
				printf("%c",peek);
				printf("%s",lexemeType[4]);
			}else{
				printf("%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		else if(current=='<'){
			peek=fgetc(fp);
			printf("%c",current);
			if(peek=='<'){
				printf("%c",peek);
				printf("%s",lexemeType[4]);
			}else if(peek=='>'){
				printf("%c",peek);
				printf("%s",lexemeType[4]);
			}else if(peek=='='){
				printf("%c",peek);
				printf("%s",lexemeType[4]);
			}else{
				printf("%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		else if(current=='>'){
			peek=fgetc(fp);
			printf("%c",current);
			if(peek=='>'){
				printf("%c",peek);
				printf("%s",lexemeType[4]);
			}else if(peek=='='){
				printf("%c",peek);
				printf("%s",lexemeType[4]);
			}else{
				printf("%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		else if(current=='*'){
			peek=fgetc(fp);
			printf("%c",current);
			if(peek=='*'){
				printf("%c",peek);
				printf("%s",lexemeType[4]);
			}else{
				printf("%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		else if(current=='='){
			peek=fgetc(fp);
			printf("%c",current);
			if(peek=='='){
				printf("%c",peek);
				printf("%s",lexemeType[4]);
			}else{
				printf("%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		else if(current=='!'){
			peek=fgetc(fp);
			printf("%c",current);
			if(peek=='='){
				printf("%c",peek);
				printf("%s",lexemeType[4]);
			}else{
				printf("%s",lexemeType[4]);
				usedPeek=1;
			}
		}
		//covers part 5
		else if(current=='0'||current=='1'||current=='2'||current=='3'||current=='4'||current=='5'||current=='6'||current=='7'||current=='8'||current=='9'){
			printf("%c",current);
			peek=num(fp);
			usedPeek=1;
			printf("%s",lexemeType[5]);
		}
		//covers parts 2 and 6
		else if(current=='a'||current=='b'||current=='c'||current=='d'||current=='e'||current=='f'||current=='g'||current=='h'||current=='i'||current=='j'||current=='k'||current=='l'||current=='m'||
				current=='n'||current=='o'||current=='p'||current=='q'||current=='r'||current=='s'||current=='t'||current=='u'||current=='v'||current=='w'||current=='x'||current=='y'||current=='z'||
				current=='A'||current=='B'||current=='C'||current=='D'||current=='E'||current=='F'||current=='G'||current=='H'||current=='I'||current=='J'||current=='K'||current=='L'||current=='M'||
				current=='N'||current=='O'||current=='P'||current=='Q'||current=='R'||current=='S'||current=='T'||current=='U'||current=='V'||current=='W'||current=='X'||current=='Y'||current=='Z'){
			peek=keyword(fp,current,keywords);
			usedPeek=1;
		}
		//covers part 7
		else {
			printf("%c",current);
			printf("%s",lexemeType[7]);
		}
	}while(current!=EOF);
	// //while(fgetc(file)!=EOF){

	// 	fgets(string,256,file);
	// 	printf("%s",string);
	// }
	fclose(fp);
	fclose(saveTo);
	return 0;
}