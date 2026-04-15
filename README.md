# Processador de Imagens

Um processador de imagens leve, interativo e de alto desempenho desenvolvido inteiramente em C. O projeto implementa algoritmos clássicos de Visão Computacional e processamento de sinais do zero, manipulando a memória diretamente via aritmética de ponteiros para máxima eficiência.

## Funcionalidades e Filtros

O sistema conta com um menu interativo e suporta leitura/escrita nativa de múltiplos formatos (JPG, PNG e PGM). Os seguintes filtros foram implementados:

* **Filtros Pontuais (In-place):**
    * **Negativo:** Inversão de cores.
    * **Brilho e Contraste:** Ajustes lineares com controle anti-overflow (Clamping).
    * **Limiarização (Threshold):** Binarização de imagem baseada em valor de corte.
* **Filtros de Vizinhança (Convolução):**
    * **Blur Médio (3x3):** Suavização básica.
    * **Blur Separável Parametrizado:** Implementação otimizada $O(2N)$ que separa o Kernel 2D em duas passagens 1D, reduzindo drasticamente a complexidade computacional para matrizes grandes (ex: 15x15).
    * **Filtro Sobel:** Detecção de bordas utilizando gradientes $G_x$ e $G_y$.
* **Filtros Não-Lineares:**
    * **Filtro Mediano:** Remoção de ruído "sal e pimenta", otimizado com um *Insertion Sort* embutido específico para arrays de 9 elementos.

## Destaques Técnicos

Este projeto foi desenhado para contornar limitações de hardware através de otimizações de software:
* **Gestão de Memória:** Manipulação direta de blocos de memória contíguos (`uint8_t*`) com ponteiros, evitando overhead de acesso a matrizes 2D por índices.
* **Independência de Frameworks Pesados:** Todo o processamento matemático foi escrito do zero.
* **Header-only Libraries:** Utiliza as consagradas bibliotecas `stb_image.h` e `stb_image_write.h` para decodificação de imagem, garantindo que o projeto seja portátil e não exija links complexos de dependências externas.
* **Compilação Modular:** Organização limpa com um `Makefile` que isola a compilação das bibliotecas STB do código fonte principal, garantindo builds instantâneos durante o desenvolvimento.

## Estrutura de Diretórios

```text
/image-filters-c
├── Makefile
├── include/
│   ├── image.h              # Structs e protótipos de leitura/escrita
│   ├── filters.h            # Protótipos dos filtros de processamento
│   ├── stb_image.h          # Decodificador (JPG, PNG, etc)
│   └── stb_image_write.h    # Codificador
├── src/
│   ├── main.c               # Ponto de entrada e Menu CLI
│   ├── stb_load.c           # Isolamento da implementação de leitura
│   ├── stb_write.c          # Isolamento da implementação de escrita
│   ├── pgm_io.c             # Leitor/Escritor proprietário PGM P5
│   └── filters.c            # Lógica matemática dos filtros
└── assets/                  # Imagens de teste e resultados
```

## Como Executar

**Pré-requisitos:** `gcc` e `make` instalados.

1. Clone o repositório:
   ```bash
   git clone [https://github.com/seu-usuario/nome-do-repositorio.git](https://github.com/seu-usuario/nome-do-repositorio.git)
   cd nome-do-repositorio
   ```

2. Compile e rode o projeto automaticamente usando o Makefile:
   ```bash
   make run
   ```

3. Siga as instruções do Menu CLI. Digite o caminho da sua imagem (ex: `assets/input.jpg`), aplique os filtros desejados em sequência e escolha a opção `0` para salvar. O arquivo final será exportado automaticamente com o sufixo `_edited.jpg` na mesma pasta da imagem original.

Para limpar os arquivos objetos gerados pela compilação:
```bash
make clean
```


