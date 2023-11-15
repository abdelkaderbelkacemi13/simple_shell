#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* hsh.c */
int hsh(info_t *, char **);
int findbuiltin(info_t *inf);
void findcmd(info_t *inf);
void cmdfork(info_t *inf);

/* path.c */
int iscmd(info_t *inf, char *path);
char *dupChars(char *, int, int);
char *path_finder(info_t *info, char *pathstr, char *cmd);

/* loophsh.c */
int loophsh(char **);

/* err_string_functions.c */
void eputs(char *);
int eputchar(char);
int putfd(char ch, int ffd);
int puts_fd(char *str, int ffd);

/* string_functions.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* string_functions2.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* string_functions3.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* string_functions4.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* memory_functions */
char *memoset(char *, char, unsigned int);
void fr_free(char **);
void *_real_aloc(void *, unsigned int, unsigned int);

/* memory_functions2.c */
int bfree(void **);

/* more_functions.c */
int interactive(info_t *);
int isdelim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* more_functions2.c */
int erratoi(char *);
void error_perinter(info_t *info, char *estr);
int print_d(int, int);
char *convert_number(long int, int, int);
void comments_remover(char *);

/* builtin_emulators.c */
int my_exit(info_t *inf);
int my_dir(info_t *inf);
int _help(info_t *inf);

/* builtin_emulators2.c */
int _history(info_t *inf);
int my_alias(info_t *inf);

/* getline.c module */
ssize_t getinput(info_t *inf);
int _getline(info_t *, char **, size_t *);
void sigintHand(int);

/* inf.c module */
void clearinfo(info_t *inf);
void setinfo(info_t *inf, char **avr);
void freeinfo(info_t *inf, int all);

/* env.c module */
char *getenvi(info_t *infi, const char *name);
int myenvi(info_t *inf);
int stenvi(info_t *inf);
int unsetenvi(info_t *inf);
int env_list(info_t *inf);

/* env2.c module */
char **get_envi(info_t *inf);
int unset_envi(info_t *inf, char *va);
int set_envi(info_t *inf, char *va, char *val);

/* file_io_functions.c */
char *history_file(info_t *inf);
int writehistory(info_t *inf);
int readhistory(info_t *inf);
int build_history_list(info_t *inf, char *buff, int lcount);
int renumber_history(info_t *inf);

/* liststr.c module */
list_t *addnode(list_t **hed, const char *str, int nu);
list_t *node_end(list_t **hed, const char *str, int nu);
size_t str_list(const list_t *h);
int delete_node_index(list_t **hed, unsigned int index);
void freelist(list_t **hed_ptr);

/* liststr2.c module */
size_t list_leng(const list_t *h);
char **strings_list(list_t *hed);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t node_index(list_t *hed, list_t *nod);

/* chain.c */
int ischain(info_t *inf, char *buff, size_t *po);
void chaincheck(info_t *inf, char *buff, size_t *po, size_t a, size_t leng);
int replacealias(info_t *inf);
int vars_replace(info_t *inf);
int str_rplace(char **, char *);

#endif

