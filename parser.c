/* front.c - um analisador léxico e analisador sintático
simples para expressões aritméticas simples */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int printed = 0;
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

#define NUM_TOKENS (sizeof(tokenStrings) / sizeof(tokenStrings[0]))

char *tokenStrings[] = {
    "LETTER", "DIGIT", "IDENT", "QUOTE", "DOT", "CHAR_STR", "UNKNOWN",
    "ASSIGN_OP", "ADD_OP", "SUB_OP", "MULT_OP", "DIV_OP",
    "LEFT_PAREN", "RIGHT_PAREN", "SEMICOLON", "COMMAND",
    "STRING", "FACTOR"};

int tokenNumbers[] =
    {0, 1, 2, 3, 4, 5, 99, 20, 21, 22, 23, 24, 25, 26, 27, 100, 200, 300};

/* Declarações de Funções */
int lookup(char ch);
void addChar();
void getChar();
void getNonBlank();
int lex();
void getString();
void error();

void program();
void stmt_list();
void stmt();
void command();
void expr();
void string();
void term();
void factor();
void printInTree(char str[]);

/******************************************************************/
/* função principal */
int main(int argc, char *argv[])
{
  if ((f = fopen(argv[1], "r")) == NULL)
    printf("ERROR - cannot open %s \n", argv[1]);
  else
  {
    getChar();
    do
    {
      lex();
      program();
    } while (nextToken != EOF);
  }
  return 0;
}

/******************************************************************/
/* lookup - uma função para processar operadores e parênteses
e retornar o token */
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

/******************************************************************/
/* addChar - uma função para adicionar nextChar ao vetor lexeme */
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

/******************************************************************/
/* getChar - uma função para obter o próximo caractere da entrada
e determinar sua classe de caracteres */
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

/******************************************************************/
/* getNonBlank - uma função para chamar getChar até que ela retorne um
caractere diferente de espaço em branco */
void getNonBlank()
{
  while (isspace(nextChar))
    getChar();
}

/******************************************************************/
/* lex - um analisador léxico simples para expressões aritméticas */
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
  /* Parênteses e operadores */
  case UNKNOWN:
    lookup(nextChar);
    getChar();
    break;
  /* Fim do arquivo */
  case EOF:
    nextToken = EOF;
    lexeme[0] = 'E';
    lexeme[1] = 'O';
    lexeme[2] = 'F';
    lexeme[3] = 0;
    break;
  } /* Fim do switch */
  // char *tokenString = getTokenString(nextToken);
  // printf("Token: %s --> \tLexeme: %s\n", tokenString, lexeme);
  return nextToken;
} /* Fim da função lex */

/******************************************************************/
/* getString - uma função para obter o próximo caractere da entrada
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

/******************************************************************/
/* Função para obter a string correspondente a um número de token */
char *getTokenString(int token)
{
  for (int i = 0; i < NUM_TOKENS; i++)
  {
    if (tokenNumbers[i] == token)
    {
      return tokenStrings[i];
    }
  }
  return "UNKNOWN";
}

/******************************************************************/
/* Função para retornar uma mensagem contendo "Erro" */
void error()
{
  printf("Erro!\n");
}

/******************************************************************/
/**********             ANALISADOR SINTATICO              **********/
/******************************************************************/

// Função <program> -> <stmt_list>
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

// Função <stmt_list> -> <stmt> {; <stmt_list>}
void stmt_list()
{
  indent++;
  printInTree("Enter <stmt_list>");
  stmt();

  while (nextToken == SEMICOLON)
  {
    lex();
    stmt();
  }
  printInTree("Exit <stmt_list>");
  indent--;
}

// Função <stmt> -> <command> | <string> | <expr>
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

// Função <expr> → <term> {(+ | -) <term>}
void expr()
{
  indent++;
  printInTree("Enter <expr>");

  term();

  while (nextToken == ADD_OP || nextToken == SUB_OP)
  {
    lex();
    term();
  }
  printInTree("Exit <expr>");
  indent--;
}

// Função <term> → <factor> {(* | /) <factor>}
void term()
{
  indent++;
  printInTree("Enter <term>");
  factor();

  while (nextToken == MULT_OP || nextToken == DIV_OP)
  {
    lex();
    factor();
  }
  printInTree("Exit <term>");
  indent--;
}

// Função <factor> → id | int_constant | (<expr>)
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
    } /* Fim do if (nextToken == ... */
    /* Não era um identificador, um literal inteiro ou um
    parêntese esquerdo */
    else
      error();
  } /* Fim do else */

  // if (nextToken == INT_LIT)
  // {
  //   lex();
  // }
  printInTree("Exit <factor>");
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
