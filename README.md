# Sistema_Eleitoral_CPP# Sistema de Apuração de Vereadores (C++17)

Projeto em C++ que lê os CSVs oficiais do TSE (candidatos e votação) para um município, processa os dados e imprime **os 10 relatórios** no **mesmo formato** do trabalho original em Java.

## Requisitos
- g++ com suporte a **C++17**
- `make`
- CSVs do TSE no **padrão 2024** (delimitador `;`, campos entre aspas, geralmente em `ISO-8859-1`)

## Como compilar e executar

```bash
make
./vereadores <cod_municipio> <candidatos.csv> <votacao.csv> <dd/MM/yyyy>
```

Exemplo:
```bash
./vereadores 01554 consulta_cand_2024_AC.csv votacao_secao_2024_AC.csv 06/10/2024
```

Alvos do Makefile:
- `make` / `make all` – compila tudo
- `make run` – executa com parâmetros de exemplo (ajuste no Makefile, alvo `run`)
- `make clean` – limpa objetos e binário

## Estrutura do projeto
```
cpp-vereadores/
├── Makefile
└── src/
    ├── main.cpp
    ├── helpers.h         # formatação (milhar/percentual) e utilitários simples
    ├── date.h            # parse de data dd/MM/yyyy e cálculo de idade
    ├── model/
    │   ├── Candidato.h  Candidato.cpp
    │   └── Partido.h    Partido.cpp
    ├── service/
    │   ├── Eleicao.h    Eleicao.cpp
    │   └── Relatorio.h  Relatorio.cpp
    └── util/
        ├── Leitor.h     Leitor.cpp
```

## Parâmetros de linha de comando
1. **`<cod_municipio>`**: código SG_UE (com zeros à esquerda). O programa **normaliza** internamente para comparar corretamente (ex.: `01554` ≡ `1554`).  
2. **`<candidatos.csv>`**: arquivo de candidatos (TSE).  
3. **`<votacao.csv>`**: arquivo de votação por seção (TSE).  
4. **`<dd/MM/yyyy>`**: data da eleição (usada para cálculo de idade).

## Regras de leitura e filtros (iguais ao Java)
- **Candidatos**: filtra por município, **CD_CARGO = 13** (vereador) e situação válida.  
- **Votos**: filtra por município e cargo. Ignora códigos especiais **95–98**.  
- **Voto nominal**: quando `NR_VOTAVEL` tem **5 dígitos** → soma em `Candidato`.  
- **Voto de legenda**: quando `NR_VOTAVEL` tem **2 dígitos** → soma em `Partido`.  
- **Associação**: cada `Candidato` guarda ponteiro para seu `Partido`. `Partido` mantém vetor de ponteiros para seus candidatos.

## Relatórios gerados (10)
1. Número de vagas (conta de eleitos).  
2. **Vereadores eleitos** pelo sistema proporcional (ordem: votos desc; empate: **mais velho primeiro**).  
3. **Top-N** mais votados nominalmente (N = número de vagas).  
4. “Teriam sido eleitos se fosse majoritário…” (top-N que **não** foram eleitos).  
5. “Eleitos que se beneficiaram do proporcional…” (eleitos **fora** do top-N), com **posição no ranking geral**.  
6. **Votação por partido**: total, nominais e legenda; desempate por **número do partido**.  
7. **Primeiro e último** de cada partido: dentro do partido ordena por votos desc e, em empate, idade desc; lista final ordenada pelo **voto do primeiro** desc, empate por número do partido asc e, depois, idade desc.  
8. **Faixa etária** dos eleitos (`<30`, `30–39`, `40–49`, `50–59`, `≥60`).  
9. **Gênero** dos eleitos (contagem e percentual).  
10. **Totais** de votos válidos, nominais e de legenda (com percentuais).

## Ordenações e desempates
- **Candidatos**: votos **desc** → idade **desc** (mais velho primeiro).  
- **Partidos (rel. 6)**: total de votos **desc** → número do partido **asc**.  
- **Rel. 7**: ver regra específica acima.  
- **Beneficiados/Prejudicados**: posição sempre tomada do **ranking global nominal**.

## Pluralização e formatação
- `voto/votos`, `nominal/nominais`, `candidato eleito/candidatos eleitos` via `if/else` simples (0 ou 1 no singular).  
- Milhar com **ponto** (ex.: `12.345`) e percentual com **vírgula** (ex.: `12,34%`).

## Considerações sobre codificação (acentos)
Os CSVs do TSE costumam estar em **ISO-8859-1**. O código-fonte está em **UTF-8**. A leitura é feita como texto “cru”; a exibição deve funcionar em terminais compatíveis. Se notar caracteres estranhos, ajuste o locale do terminal ou podemos adicionar conversão de encoding posteriormente.

## Qualidade e memória
- Uso de `std::unique_ptr` em `Eleicao` e vetores de ponteiros estáveis (armazenados nos mapas) para evitar **vazamentos**.  
- Teste com `valgrind` (opcional): `valgrind --leak-check=full ./vereadores ...`.

## Observações
- A implementação espelha a lógica já validada no projeto **Java**.  
- Qualquer divergência de saída formatada deve ser reportada com **trecho esperado vs. obtido** para ajuste fino de impressão.
