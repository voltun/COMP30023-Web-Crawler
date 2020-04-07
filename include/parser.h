#ifndef PARSER_H
#define PARSER_H

char** parse_html(char*, int, int);
int rem_http(char*);
char* substring(char*, int, int);
char* split_hostname(char*);

#endif