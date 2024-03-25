# my-shell

![Language](https://img.shields.io/badge/language-C-blue)

## Descrição

O "my-shell" é um projeto desenvolvido por [Giovana Trevizan Barbosa](https://github.com/giovanatrevizanbarbosa/) e [Natan Salvador Ligabô](https://github.com/natansalvadorligabo/), criado com o propósito de fornecer uma experiência prática para entender os conceitos fundamentais de Sistemas Operacionais. Este shell básico permite explorar o funcionamento de processos, chamadas de sistema, gerenciamento de processos e outras funcionalidades do sistema operacional.

## Funcionalidades

- Implementação dos comandos internos "cd", "exit", "alias" e "history".
- Prompt no formato "user@host[hh:mm:ss] $", com o horário atualizado a cada novo comando.
- Comando "history" que exibe o histórico dos últimos 10 comandos, numerados de 1 a 10, permitindo repetir um comando digitando "!n", onde n é o número do comando no histórico.
- Os comandos externos são executados pela shell criando um processo filho.
- Utilização das chamadas de sistema "fork" e "execvp" para executar comandos externos.
- O processo pai (shell) aguarda a finalização do processo-filho (comando externo) usando a chamada "waitpid".
- Utilização da função "perror()" para mostrar mensagens de erro se a função "execvp()" retornar um código de erro.

## Contato

- Giovana Trevizan Barbosa: [LinkedIn](https://www.linkedin.com/in/giovanatrevizanbarbosa/)
- Natan Salvador Ligabô: [LinkedIn](https://www.linkedin.com/in/natansalvadorligabo/)

## Instalação

Para compilar e executar o projeto, utilize os seguintes comandos no terminal:

```bash
gcc -o my-shell.out *.c
```

```bash
./my-shell
```

## Uso

Após compilar o projeto, execute o arquivo gerado. O shell exibirá um prompt onde você pode digitar comandos internos ou externos.

## Demonstração

![01](https://i.imgur.com/NHUBKxY.png)

