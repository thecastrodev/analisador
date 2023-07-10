# InterpretadorPLP

Modelo de Analisador Lexico e Analisador Sintatico simples

### Execução em Windows

Basta clicar no arquivo _parser.cmd_ 

### Execução em Linux

Após clonar o repositório, na raiz do projeto, você precisará dar permissão de execução para o script. 

Para isso, faça:
```sh
  chmod +x parser.sh
```

Após dar a permissão, é só executar com o comando:
```sh
  ./parser.sh
```

Isso já deve ser suficiente para rodar seus comandos via terminal.

</br>

---

## Imprimindo seus próprios arquivos

Caso queira adicionar seus próprios arquivos, o esquema é simples. Estando na raiz do projeto, faça:

### Em Windows
```sh
  .\bin\parser.exe <file_path>
  .\bin\parser.exe .\input\reader.vb
```

### Em Linux
```sh
  ./bin/parser.out <file_path>
  ./bin/parser.out ./input/reader.vb
```

</br>

---

## Análise lexica

### Em Windows
```sh
  .\bin\lexer.exe <file_path>
  .\bin\lexer.exe .\input\reader.vb
```

### Em Linux
```sh
  ./bin/lexer.out <file_path>
  ./bin/lexer.out ./input/reader.vb
```
