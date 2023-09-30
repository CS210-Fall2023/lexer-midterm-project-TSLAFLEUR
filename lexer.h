/**
 * @file lexer.h
 * @author Tyler LaFleur
 * @brief 
 * @version 0.1
 * @date 2023-09-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <string.h>

int comment(int CT,FILE *fp);//

int string(FILE *fp);

int CHAR(FILE *fp);

char num(FILE *fp);

char keyword(FILE *fp,char first,char**keywords);

#endif