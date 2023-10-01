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

int comment(int CT,FILE *fp,FILE *saveTo);//

int string(FILE *fp,FILE *saveTo);

int CHAR(FILE *fp,FILE *saveTo);

char num(FILE *fp,char first,FILE *saveTo);

char keyword(FILE *fp,char first,char**keywords,FILE *saveTo);

#endif