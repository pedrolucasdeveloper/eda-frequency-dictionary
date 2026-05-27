# EDA Frequency Dictionary

Projeto Final da disciplina de Estrutura de Dados Avançada (EDA) — UFC Campus Quixadá.

## 📚 Descrição

Este projeto consiste no desenvolvimento de um contador de frequência de palavras utilizando diferentes estruturas de dados implementadas manualmente em C++.

O sistema realiza a leitura de arquivos `.txt`, processa o conteúdo do texto e gera uma tabela de frequência das palavras em ordem alfabética.

As seguintes estruturas de dados são utilizadas como base para implementação de dicionários genéricos:

- AVL Tree (iterativa)
- Red-Black Tree (iterativa)
- Hash Table com Encadeamento Exterior
- Hash Table com Endereçamento Aberto

O objetivo principal é comparar desempenho, eficiência e comportamento das estruturas durante operações de inserção, busca e manipulação de dados.

---

# 🚀 Funcionalidades

- Leitura de arquivos `.txt`
- Normalização de palavras
- Remoção de pontuação
- Conversão para minúsculas
- Contagem de frequência de palavras
- Exportação para CSV
- Comparação entre estruturas de dados
- Interface via linha de comando (CLI)

---

# 🧠 Estruturas implementadas

## AVL Tree
Árvore AVL totalmente iterativa, sem uso de recursão.

## Red-Black Tree
Árvore Rubro-Negra totalmente iterativa, sem uso de recursão.

## Hash Table - Chaining
Tabela hash com tratamento de colisão por encadeamento exterior.

## Hash Table - Open Addressing
Tabela hash com tratamento de colisão por endereçamento aberto.

---

# 📂 Estrutura do Projeto

```txt
src/
│
├── main.cpp
├── Dictionary.hpp
├── AVLTree.hpp
├── RedBlackTree.hpp
├── HashChaining.hpp
├── HashOpenAddressing.hpp
│
├── utils/
│   ├── TextProcessor.hpp
│   └── CSVExporter.hpp
│
└── tests/
