# EDA Frequency Dictionary

Projeto final da disciplina de **Estrutura de Dados Avançada**, com o objetivo de contar a frequência de palavras em um arquivo `.txt` usando diferentes implementações de dicionários.

O programa lê um arquivo de texto, processa as palavras, ignora pontuações, converte letras maiúsculas para minúsculas e gera um arquivo `.csv` contendo cada palavra encontrada e sua respectiva frequência.

Além disso, o programa coleta métricas de desempenho das estruturas, como número de comparações de chave, colisões, sondagens, rotações, rehashes e tempo de montagem da tabela de frequências.

---

## Estruturas implementadas

O projeto implementa quatro tipos de dicionários:

1. Árvore AVL
2. Árvore Rubro-Negra
3. Tabela Hash com Encadeamento Exterior
4. Tabela Hash com Endereçamento Aberto

Todas as estruturas seguem uma interface comum definida em `Dictionary.hpp`.

---

## Estrutura do projeto

```txt
eda-frequency-dictionary/
├── src/
│   ├── AVLTree.hpp
│   ├── RedBlackTree.hpp
│   ├── Dictionary.hpp
│   ├── HashChaining.hpp
│   ├── HashOpenAddressing.hpp
│   └── main.cpp
├── teste.txt
├── README.md
└── LICENSE
```

---

## Descrição dos arquivos

### `Dictionary.hpp`

Define a interface base para todas as estruturas de dicionário.

Essa interface utiliza `templates`, permitindo que o dicionário trabalhe com diferentes tipos de chave e valor.

Principais operações definidas:

```cpp
insert(key, value)
update(key, value)
get(key)
contains(key)
remove(key)
clear()
size()
items()
metrics()
resetMetrics()
```

Também define a estrutura `DictionaryMetrics`, usada para armazenar métricas como:

```txt
- Comparações de chave
- Colisões
- Probes/sondagens
- Rotações
- Rehashes
- Fator de carga
```

---

### `HashChaining.hpp`

Implementa uma tabela hash com tratamento de colisões por **encadeamento exterior**.

Nessa abordagem, cada posição da tabela possui uma lista de elementos. Quando duas chaves caem no mesmo índice, elas são armazenadas na mesma lista.

Exemplo conceitual:

```txt
índice 0 -> vazio
índice 1 -> [casa: 3] -> [livro: 2]
índice 2 -> [amor: 1]
```

Métricas coletadas:

```txt
- Comparações de chave
- Colisões
- Rehashes
- Fator de carga
```

---

### `HashOpenAddressing.hpp`

Implementa uma tabela hash com tratamento de colisões por **endereçamento aberto**, utilizando sondagem linear.

Nessa abordagem, cada posição da tabela armazena no máximo um elemento. Quando ocorre colisão, o algoritmo procura a próxima posição disponível.

Exemplo conceitual:

```txt
hash("casa") -> índice 3

se índice 3 estiver ocupado:
tenta índice 4
se índice 4 estiver ocupado:
tenta índice 5
...
```

A estrutura utiliza três estados para cada posição:

```txt
EMPTY    -> posição vazia
OCCUPIED -> posição ocupada
DELETED  -> posição removida logicamente
```

Métricas coletadas:

```txt
- Comparações de chave
- Colisões
- Probes/sondagens
- Rehashes
- Fator de carga
```

---

### `AVLTree.hpp`

Implementa um dicionário baseado em **Árvore AVL**.

A AVL é uma árvore binária de busca balanceada. Após inserções e remoções, a árvore realiza rotações para manter o balanceamento.

Métricas coletadas:

```txt
- Comparações de chave
- Rotações
```

A listagem dos elementos é feita em ordem alfabética através do percurso em ordem da árvore.

---

### `RedBlackTree.hpp`

Implementa um dicionário baseado em **Árvore Rubro-Negra**.

A Rubro-Negra é uma árvore binária de busca balanceada que utiliza cores nos nós para manter suas propriedades de balanceamento.

Métricas coletadas:

```txt
- Comparações de chave
- Rotações
```

Assim como na AVL, os elementos podem ser percorridos em ordem alfabética.

---

### `main.cpp`

Arquivo principal do programa.

Responsabilidades:

```txt
- Ler os argumentos da linha de comando
- Escolher qual estrutura será usada
- Abrir e processar o arquivo .txt
- Separar e normalizar as palavras
- Inserir ou atualizar a frequência no dicionário
- Medir o tempo de execução
- Ordenar os resultados alfabeticamente
- Gerar o arquivo .csv
- Exibir métricas no terminal
```

---

## Como compilar

Na raiz do projeto, execute:

```bash
g++ -std=c++11 src/main.cpp -o freq.exe
```

No Linux, você pode compilar sem a extensão `.exe`:

```bash
g++ -std=c++11 src/main.cpp -o freq
```

---

## Como executar

O programa segue o seguinte formato:

```bash
./freq dictionary estrutura entrada.txt saida.csv
```

No Windows PowerShell:

```powershell
.\freq.exe dictionary estrutura entrada.txt saida.csv
```

---

## Estruturas disponíveis

### Hash com Encadeamento Exterior

```powershell
.\freq.exe dictionary hash-chaining teste.txt resultado_chaining.csv
```

### Hash com Endereçamento Aberto

```powershell
.\freq.exe dictionary hash-open teste.txt resultado_open.csv
```

### Árvore AVL

```powershell
.\freq.exe dictionary avl teste.txt resultado_avl.csv
```

### Árvore Rubro-Negra

```powershell
.\freq.exe dictionary rb teste.txt resultado_rb.csv
```

Também é possível usar:

```powershell
.\freq.exe dictionary red-black teste.txt resultado_rb.csv
```

---

## Ajuda do programa

Para exibir as opções disponíveis:

```powershell
.\freq.exe --help
```

Ou:

```powershell
.\freq.exe -h
```

---

## Exemplo de arquivo de entrada

Arquivo `teste.txt`:

```txt
Quatro mais quatro são oito.
Quatro, quatro! Casa casa casa.
Mostra-lo é diferente de mostrar.
```

---

## Saída esperada no CSV

O programa gera um arquivo `.csv` com duas colunas:

```csv
palavra,frequencia
casa,3
de,1
diferente,1
mais,1
mostrar,1
mostra-lo,1
oito,1
quatro,4
são,1
é,1
```

As palavras são ordenadas alfabeticamente antes da geração do CSV.

---

## Saída esperada no terminal

Ao executar o programa, o terminal exibe informações como:

```txt
Estrutura utilizada: Hash Chaining
Arquivo de entrada: teste.txt
Arquivo de saida: resultado_chaining.csv
Palavras distintas: 10
Tempo de montagem: 0.35 ms
Comparacoes de chave: 15
Colisoes: 1
Probes: 0
Rehashes: 0
Fator de carga: 0.09
```

Os valores podem variar dependendo da estrutura utilizada, do tamanho do texto e da quantidade de palavras distintas.

---

## Tratamento das palavras

Durante a leitura do texto, o programa:

```txt
- Ignora espaços em branco
- Ignora sinais de pontuação
- Converte letras maiúsculas para minúsculas
- Mantém caracteres acentuados
- Mantém hífen quando aparece dentro de uma palavra
```

Exemplo:

```txt
"Quatro, QUATRO!" -> "quatro", "quatro"
"Mostra-lo"       -> "mostra-lo"
```

---

## Arquivos gerados

Durante os testes, podem ser gerados arquivos como:

```txt
freq.exe
resultado_chaining.csv
resultado_open.csv
resultado_avl.csv
resultado_rb.csv
```

Esses arquivos são resultados de compilação ou execução e não precisam necessariamente ser versionados.

Sugestão de `.gitignore`:

```gitignore
*.exe
*.csv
*.gch
```

---

## Objetivo do projeto

O objetivo principal é comparar o comportamento de diferentes estruturas de dados usadas como dicionários na tarefa de contagem de frequência de palavras.

As estruturas são comparadas com base em métricas como:

```txt
- Tempo de montagem da tabela de frequência
- Número de comparações de chave
- Número de colisões nas tabelas hash
- Número de sondagens no endereçamento aberto
- Número de rotações nas árvores balanceadas
- Fator de carga nas tabelas hash
```

---

## Integrantes e divisão de tarefas

### Integrante 1

Responsável por:

```txt
- Interface Dictionary
- Hash com encadeamento exterior
- Hash com endereçamento aberto
- Integração inicial com main.cpp
- Leitura do arquivo .txt
- Geração do arquivo .csv
- Métricas das tabelas hash
```

### Integrante 2

Responsável por:

```txt
- Árvore AVL
- Árvore Rubro-Negra
- Métricas das árvores
- Rotações
- Testes das estruturas balanceadas
```

### Atividades em conjunto

```txt
- Testes finais
- Comparação das estruturas
- Documentação
- Relatório final
- Apresentação
```

---

## Observações

Este projeto foi desenvolvido em C++ utilizando estruturas genéricas com `templates`.

A aplicação é executada por linha de comando, sem menu interativo, seguindo a proposta do trabalho da disciplina.
