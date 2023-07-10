/* parser.c - um analisador sintático simples */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int indent = -1;

/* Declarações globais */
/* Variáveis */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *f, *fopen();

char *getTokenString(int token);

/* Classes de caracteres */
#define LETTER 0
#define DIGIT 1
#define IDENT 2
#define QUOTE 3
#define DOT 4
#define CHAR_STR 5
#define INT_LIT 6
#define UNKNOWN 99

/* Códigos de tokens */
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define SEMICOLON 27

#define COMMAND 100
#define STRING 200
#define FACTOR 300

void reader(char file_name[]);
void writer();

/* Declarações de Funções Lexico */
int lookup(char ch);
void addChar();
void getChar();
void getNonBlank();
int lex();
void getString();
void error();

/* Declarações de Funções Sintatico */
void program();
void stmt_list();
void stmt();
void command();
void expr();
void string();
void term();
void factor();
void operator();
void printInTree(char str[]);

/*******************************************************************/
/* funcao principal */
int main(int argc, char *argv[])
{
  if (argc < 2)
    do
    {
      writer();
    } while (1);
  else
    reader(argv[1]);
  return 0;
}

void reader(char file_name[])
{
  if ((f = fopen(file_name, "r")) == NULL)
    printf("ERROR - cannot open %s.\n", file_name);
  else
  {
    getChar();
    do
    {
      lex();
      program();
    } while (nextToken != EOF);
  }
}

void writer()
{
  char file_name[] = "./input/writer.vb";
  const int stmt_size = 200;
  char statement[stmt_size];

  printf("\nPara sair digite --->   Application.Exit \n");
  printf("Exemplos de comandos [ obs: sem ; ]:     \n");
  printf("[Ex1] Console.Write(\"Hello, World!\")   \n");
  printf("[Ex2] Console.Write(4 + 4)               \n");
  printf("[Ex3] 1 + 2 * 3 / 4                      \n");
  printf("Digite um comando: ");
  fgets(statement, stmt_size, stdin);

  if (strstr(statement, "Application.Exit"))
    exit(0);

  f = fopen(file_name, "w");
  if (f == NULL) // testando criacao
    printf("Erro na abertura do arquivo!\n");
  fputs(statement, f);
  fclose(f);

  reader(file_name);
}

/*******************************************************************/
/* lookup - uma funcao para processar operadores, parenteses e ponto
e virgula, depois retornar o token */
int lookup(char ch)
{
  switch (ch)
  {
  case '(':
    addChar();
    nextToken = LEFT_PAREN;
    break;
  case ')':
    addChar();
    nextToken = RIGHT_PAREN;
    break;
  case '+':
    addChar();
    nextToken = ADD_OP;
    break;
  case '-':
    addChar();
    nextToken = SUB_OP;
    break;
  case '*':
    addChar();
    nextToken = MULT_OP;
    break;
  case '/':
    addChar();
    nextToken = DIV_OP;
    break;
  case ';':
    addChar();
    nextToken = SEMICOLON;
    break;
  default:
    addChar();
    nextToken = EOF;
    break;
  }
  return nextToken;
}

/*******************************************************************/
/* addChar - uma funcao para adicionar nextChar ao vetor lexeme    */
void addChar()
{
  if (lexLen <= 98)
  {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  }
  else
    printf("Error - lexeme is too long \n");
}

/*******************************************************************/
/* getChar - uma funcao para obter o proximo caractere da entrada e
determinar sua classe de caracteres */
void getChar()
{
  if ((nextChar = getc(f)) != EOF)
  {
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
    else if (nextChar == '=')
      charClass = IDENT;
    else if (nextChar == '"')
      charClass = QUOTE;
    else if (nextChar == '.')
      charClass = DOT;
    else
      charClass = UNKNOWN;
  }
  else
    charClass = EOF;
}

/*******************************************************************/
/* getNonBlank - uma funcao para chamar getChar ate que ela retorne
um caractere diferente de espaco em branco */
void getNonBlank()
{
  while (isspace(nextChar))
    getChar();
}

/*******************************************************************/
/* lex - um analisador lexico simples para expressoes aritmeticas  */
int lex()
{
  lexLen = 0;
  getNonBlank();
  switch (charClass)
  {
  /* Reconhecer comandos */
  case LETTER:
    addChar();
    getChar();
    while (charClass == LETTER || charClass == DOT)
    {
      addChar();
      getChar();
    }
    nextToken = COMMAND;
    break;
  /* Reconhecer fatores */
  case DIGIT:
    addChar();
    getChar();
    while (charClass == DIGIT)
    {
      addChar();
      getChar();
    }
    nextToken = INT_LIT;
    break;
  /* Reconhecer string */
  case QUOTE:
    addChar();
    getString();
    while (charClass == CHAR_STR)
    {
      addChar();
      getString();
    }
    if (charClass == QUOTE)
    {
      addChar();
      getChar();
      nextToken = STRING;
    }
    else
    {
      error();
    }
    break;
  /* Parenteses e operadores */
  case UNKNOWN:
    lookup(nextChar);
    getChar();
    break;
  case EOF:
    nextToken = EOF;
    lexeme[0] = 'E';
    lexeme[1] = 'O';
    lexeme[2] = 'F';
    lexeme[3] = 0;
    break;
  }
  return nextToken;
}

/*******************************************************************/
/* getString - uma funcao para obter o próximo caractere da entrada
para aceitar qualquer caractere em strings, com exceção de " e quebra de linha */
void getString()
{
  if ((nextChar = getc(f)) != EOF)
  {
    if (nextChar == '"')
      charClass = QUOTE;
    else if (nextChar == '\n')
    {
      error();
    }
    else
      charClass = CHAR_STR;
  }
  else
    charClass = EOF;
}

/*******************************************************************/
/*  Funcao para retornar uma mensagem contendo "Erro"              */
void error()
{
  printf("Erro!\n");
}

/*******************************************************************/
/**********             ANALISADOR SINTATICO              **********/
/*******************************************************************/

// Funcao <program> -> <stmt_list>
void program()
{
  indent++;
  printInTree("Enter <program>");

  while (nextToken != EOF)
  {
    stmt_list();
  }

  printInTree("Exit <program>");
  indent--;
}

// Funcao <stmt_list> -> <stmt> {; <stmt_list>}
void stmt_list()
{
  indent++;
  printInTree("Enter <stmt_list>");

  stmt();

  while (nextToken == SEMICOLON)
  {
    operator();
    lex();
    stmt();
  }

  printInTree("Exit <stmt_list>");
  indent--;
}

// Funcao <stmt> -> <command> | <string> | <expr>
void stmt()
{
  indent++;
  printInTree("Enter <stmt>");

  if (nextToken == COMMAND)
    command();
  else if (nextToken == STRING)
    string();
  else
    expr();

  printInTree("Exit <stmt>");
  indent--;
}

// Funcao <command> -> Console.Write(<expr> | <string>)
void command()
{
  indent++;
  printInTree("Enter <command>");

  if (nextToken == COMMAND)
  {
    printInTree(lexeme);
    lex();
  }
  if (nextToken == LEFT_PAREN)
  {
    printInTree(lexeme);
    lex();
  }
  if (nextToken == STRING)
    string();
  if (nextToken == INT_LIT)
    expr();
  if (nextToken == RIGHT_PAREN)
  {
    printInTree(lexeme);
    lex();
  }

  printInTree("Exit <command>");
  indent--;
}

// Funcao <string> -> "{a|b|...|y|z|A|B|...|Y|Z}"
void string()
{
  indent++;
  printInTree("Enter <string>");

  printInTree(lexeme);
  if (nextToken == STRING)
    lex();

  printInTree("Exit <string>");
  indent--;
}

// Funcao <expr> → <term> {(+ | -) <term>}
void expr()
{
  indent++;
  printInTree("Enter <expr>");

  term();

  while (nextToken == ADD_OP || nextToken == SUB_OP)
  {
    operator();
    lex();
    term();
  }

  printInTree("Exit <expr>");
  indent--;
}

// Funcao <term> → <factor> {(* | /) <factor>}
void term()
{
  indent++;
  printInTree("Enter <term>");

  factor();

  while (nextToken == MULT_OP || nextToken == DIV_OP)
  {
    operator();
    lex();
    factor();
  }

  printInTree("Exit <term>");
  indent--;
}

// Funcao <factor> → id | int_constant | (<expr>)
void factor()
{
  indent++;
  printInTree("Enter <factor>");

  printInTree(lexeme);

  /* Determina qual RHS */
  if (nextToken == IDENT || nextToken == INT_LIT)
    /* Obtém o próximo token */
    lex();
  /* Se a RHS é (<expr>), chame lex para passar o parêntese
  esquerdo, chame expr e verifique pelo parêntese
  direito */
  else
  {
    if (nextToken == LEFT_PAREN)
    {
      lex();
      expr();
      if (nextToken == RIGHT_PAREN)
        lex();
      else
        error();
    }
    /* Não era um identificador, um literal inteiro ou um
    parêntese esquerdo */
    else
      error();
  }

  printInTree("Exit <factor>");
  indent--;
}

// Funcao <operador> -> ( + | - | * | / | ;)
void operator()
{
  indent++;
  printInTree(lexeme);
  indent--;
}

void printInTree(char str[])
{
  for (int i = 0; i < indent; i++)
  {
    printf("  ");
  }
  printf("%s\n", str);
}
