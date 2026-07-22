# Dicionário: Contador de Frequência de Palavras

Este programa é parte de um projeto final da disciplina de Estruturas de Dados Avançada do Curso de Ciência da Computação da Universidade Federal do Ceará (UFC), Campus Quixadá, sob orientação do professor Atílio Gomes Luiz.

## Descrição

O programa consiste em um contador de frequência de palavras baseado em um dicionário que implementa quatro estruturas internas: Árvore AVL, Árvore Rubro-Negra, Tabela Hash com Encadeamento Externo e Tabela Hash com Endereçamento Aberto. O programa lê um arquivo de texto no formato `.txt` e gera dois arquivos `.csv`, um com a contagem de frequência de palavras em ordem alfabética e outro com as métricas de tempo e número de comparações necessários para a construção da estrutura.

## Dependências

### Obrigatórias

**g++ com suporte a C++17**

Linux:
```bash
sudo apt update
sudo apt install g++
```

Windows (MSYS2/MinGW):
```bash
pacman -S mingw-w64-x86_64-gcc
```

---

**Boost.locale**

Linux:
```bash
sudo apt install libboost-locale-dev
```

Windows (MSYS2/MinGW):
```bash
pacman -S mingw-w64-x86_64-boost
```

### Opcionais (para geração de gráficos)

**Python 3 e matplotlib**

Linux:
```bash
sudo apt install python3 python3-pip
pip install matplotlib
```

Windows:
```bash
pip install matplotlib
```

## Compilação

Linux:
```bash
g++ src/main.cpp -I./include -I./include/dictionary -std=c++17 -o freq -lboost_locale
```

Windows:
```bash
g++ src/main.cpp -I./include -I./include/dictionary -std=c++17 -o freq -LC:/msys64/mingw64/lib -lboost_locale-mt
```

> **Nota:** Caso o MinGW esteja instalado em outro diretório,
> substitua `C:/msys64/mingw64/lib` pelo caminho correto da
> sua instalação.

## Uso

```bash
./freq <estrutura> <caminho_do_livro>
```

| Argumento | Descrição |
|---|---|
| `<estrutura>` | Nome da estrutura de dados a ser utilizada |
| `<caminho_do_livro>` | Caminho para o arquivo `.txt` a ser processado |

### Estruturas disponíveis

| Comando | Estrutura |
|---|---|
| `dicionario_avl` | Árvore AVL |
| `dicionario_redblack` | Árvore Rubro-Negra |
| `dicionario_chaining` | Tabela Hash com Encadeamento Exterior |
| `dicionario_openadr` | Tabela Hash com Endereçamento Aberto |

Para exibir os comandos disponíveis:
```bash
./freq --help
```

## 📄 Documentação

- [Documentação técnica.pdf](https://github.com/user-attachments/files/30285517/Documentacao.tecnica.pdf) — Detalhes completos sobre arquitetura, funcionamento e uso do projeto.
